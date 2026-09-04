/**
  ******************************************************************************
  * @file    audio_player.c
  * @brief   MP3 playback pipeline:
  *          SD (FatFs) -> minimp3 stream decode (CPU) -> SAI1_B TX DMA -> ES8311
  *
  * Design notes (each one earned the hard way):
  *  - PCM DMA buffers live in AXI SRAM (section .audio_buf). DTCM is not
  *    reliably reachable by DMA2 and caused transfer errors before.
  *  - The DMA IRQ priority must be <= FreeRTOS
  *    configMAX_SYSCALL_INTERRUPT_PRIORITY (5) because the ISR uses
  *    xSemaphoreGiveFromISR (CubeMX generated priority 0, which is fine).
  *  - minimp3_ex needs heap; the C heap is 512 B so its malloc/realloc/free
  *    are routed to a bump allocator in SDRAM (0xC0800000, clear of the LVGL
  *    pool at 0xC0400000 and the framebuffers at 0xC0000000).
  *  - The audio task never touches LVGL; it only writes g_audio_status.
  ******************************************************************************
  */

#include "audio_player.h"
#include "main.h"
#include "fatfs.h"
#include "es8311.h"
#include "i2c.h"
#include "sai.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cmsis_os.h"
#include <string.h>

/* ---- minimp3 memory (SDRAM bump allocator) ------------------------------- */
#define AUDIO_POOL_BASE 0xC0800000UL
#define AUDIO_POOL_SIZE (64u * 1024u)
#define AUDIO_DEC_BASE  0xC0820000UL   /* mp3dec_ex_t, kept clear of the pool */

static uint32_t s_pool_used;

static void *audio_malloc(size_t size)
{
  uint32_t aligned = (uint32_t)((size + 3u) & ~3u);
  if (s_pool_used + aligned > AUDIO_POOL_SIZE)
  {
    return NULL;
  }
  void *p = (void *)(AUDIO_POOL_BASE + s_pool_used);
  s_pool_used += aligned;
  return p;
}

static void audio_free(void *ptr) { (void)ptr; }            /* bump: never frees */

static void *audio_realloc(void *ptr, size_t size)
{
  void *np = audio_malloc(size);
  if (np != NULL && ptr != NULL)
  {
    memcpy(np, ptr, size);
  }
  return np;
}

/* ---- minimp3 (single header, public domain) ------------------------------ */
#define MINIMP3_IMPLEMENTATION
#define MINIMP3_NO_STDIO
#define MINIMP3_IO_SIZE (32 * 1024)   /* stream buffer (> MINIMP3_BUF_SIZE) */
#include "minimp3.h"
#define malloc audio_malloc
#define free   audio_free
#define realloc audio_realloc
#include "minimp3_ex.h"
#undef malloc
#undef free
#undef realloc

/* ---- PCM double buffer in AXI SRAM (DMA-safe) ---------------------------- */
#define PCM_FRAME 1152   /* MPEG1 samples per channel per frame */
__attribute__((section(".audio_buf"), aligned(32)))
static int16_t pcm_buf[2][PCM_FRAME * 2];

/* ---- shared state (read by the LVGL task only) --------------------------- */
volatile char g_audio_status[80];
volatile uint8_t g_audio_status_new;

static void audio_status(const char *s)
{
  strncpy((char *)g_audio_status, s, sizeof(g_audio_status) - 1);
  g_audio_status[sizeof(g_audio_status) - 1] = 0;
  g_audio_status_new = 1;
}

/* ---- file handles (static: never on a task stack) ------------------------ */
static FIL g_mp3fil;
static mp3dec_io_t g_mp3io;
static mp3dec_ex_t *g_dec;
static SemaphoreHandle_t g_tx_sem;

/* Defined in sai.c (CubeMX HAL_SAI_MspInit); shared with stm32h7xx_it.c. */
extern DMA_HandleTypeDef hdma_sai1_b;

/* ---- FatFs callbacks for minimp3_ex -------------------------------------- */
static size_t audio_read_cb(void *buf, size_t size, void *user_data)
{
  UINT br = 0;
  FRESULT fr = f_read((FIL *)user_data, buf, (UINT)size, &br);
  if (fr != FR_OK)
  {
    return 0;
  }
  return (size_t)br;
}

static int audio_seek_cb(uint64_t pos, void *user_data)
{
  return (f_lseek((FIL *)user_data, (FSIZE_t)pos) == FR_OK) ? 0 : -1;
}

/* ---- 44.1 kHz clocking ---------------------------------------------------- */
static void audio_clock_44100(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  /* PLL2: (25MHz/25) x (225 + 6488/8192) = 225.792 MHz, /2 = 112.896 MHz.
   * SAI MCKDIV stays 10 -> MCLK 11.2896 MHz = 44100 x 256. */
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 225;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 6488.0;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

static int audio_es8311_init(void)
{
  es8311_handle_t dev = es8311_create(&hi2c2, ES8311_ADDRRES_0);
  if (dev == NULL)
  {
    return -1;
  }
  const es8311_clock_config_t clk = {
    .mclk_inverted = false,
    .sclk_inverted = false,
    .mclk_from_mclk_pin = true,
    .mclk_frequency = 11289600,
    .sample_frequency = 44100
  };
  if (es8311_init(dev, &clk, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16) != ES8311_OK)
  {
    return -1;
  }
  es8311_voice_volume_set(dev, 40, NULL);
  return 0;
}

/* Called from the DMA2_Stream3 ISR (via HAL_SAI_TxCpltCallback) when one
 * PCM buffer has been fully clocked out. */
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  (void)hsai;
  BaseType_t w = pdFALSE;
  xSemaphoreGiveFromISR(g_tx_sem, &w);
  portYIELD_FROM_ISR(w);
}

/* Decode exactly one MPEG frame into out[]. Short/EOF frames are zero-padded. */
static int decode_frame(int16_t *out)
{
  size_t got = mp3dec_ex_read(g_dec, out, (size_t)(PCM_FRAME * 2));
  if (got < (size_t)(PCM_FRAME * 2))
  {
    for (size_t i = got; i < (size_t)(PCM_FRAME * 2); i++)
    {
      out[i] = 0;
    }
  }
  return (int)got;
}

/* ---- player task ---------------------------------------------------------- */
static void audio_player_task(void *arg)
{
  (void)arg;
  FRESULT fr;
  char buf[96];

  if (g_play_path[0] == 0)
  {
    audio_status("没有找到音频文件");
    vTaskDelete(NULL);
    return;
  }

  fr = f_open(&g_mp3fil, (const TCHAR *)g_play_path, FA_READ);
  if (fr != FR_OK)
  {
    audio_status("打开 MP3 失败");
    vTaskDelete(NULL);
    return;
  }

  /* Stop SAI from the boot state, retune PLL2 for 44.1 kHz, then point the
   * ES8311 at the new MCLK. Frame config of the SAI is untouched (it was
   * fixed in MX_SAI1_Init USER CODE and is identical for 44.1/48 kHz). */
  __HAL_SAI_DISABLE(&hsai_BlockB1);
  audio_clock_44100();
  if (audio_es8311_init() != 0)
  {
    audio_status("ES8311 初始化失败");
    f_close(&g_mp3fil);
    vTaskDelete(NULL);
    return;
  }

  g_dec = (mp3dec_ex_t *)AUDIO_DEC_BASE;
  g_mp3io.read = audio_read_cb;
  g_mp3io.read_data = &g_mp3fil;
  g_mp3io.seek = audio_seek_cb;
  g_mp3io.seek_data = &g_mp3fil;

  int n = mp3dec_ex_open_cb(g_dec, &g_mp3io, MP3D_DO_NOT_SCAN);
  if (n != 0)
  {
    lv_snprintf(buf, sizeof(buf), "解码器打开失败 err=%d", n);
    audio_status(buf);
    f_close(&g_mp3fil);
    vTaskDelete(NULL);
    return;
  }

  if (g_tx_sem == NULL)
  {
    g_tx_sem = xSemaphoreCreateBinary();
  }

  /* Prime the first buffer, then start DMA so the codec clocks it out. */
  if (decode_frame(pcm_buf[0]) <= 0)
  {
    audio_status("解码失败");
    mp3dec_ex_close(g_dec);
    f_close(&g_mp3fil);
    vTaskDelete(NULL);
    return;
  }

  lv_snprintf(buf, sizeof(buf), "播放 %dHz %dch",
              g_dec->info.hz, g_dec->info.channels);
  audio_status(buf);

  /* The ISR gives the semaphore slightly before the DMA handle is reset to
   * READY; starting the next transfer too early returns HAL_BUSY. */
  while (hdma_sai1_b.State != HAL_DMA_STATE_READY)
  {
    taskYIELD();
  }
  if (HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)pcm_buf[0],
                           (uint16_t)(PCM_FRAME * 2)) != HAL_OK)
  {
    audio_status("SAI DMA 启动失败");
    mp3dec_ex_close(g_dec);
    f_close(&g_mp3fil);
    vTaskDelete(NULL);
    return;
  }

  int cur = 1;
  for (;;)
  {
    if (xSemaphoreTake(g_tx_sem, pdMS_TO_TICKS(2000)) != pdTRUE)
    {
      audio_status("播放中断(超时)");
      break;
    }
    if (decode_frame(pcm_buf[cur]) <= 0)
    {
      audio_status("播放结束");
      break;
    }
    while (hdma_sai1_b.State != HAL_DMA_STATE_READY)
    {
      taskYIELD();
    }
    if (HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)pcm_buf[cur],
                             (uint16_t)(PCM_FRAME * 2)) != HAL_OK)
    {
      audio_status("SAI DMA 错误");
      break;
    }
    cur ^= 1;
  }

  HAL_SAI_DMAStop(&hsai_BlockB1);
  mp3dec_ex_close(g_dec);
  f_close(&g_mp3fil);
  vTaskDelete(NULL);
}

void audio_player_start(void)
{
  static const osThreadAttr_t audio_attr = {
    .name = "audio",
    .stack_size = 24 * 1024,
    .priority = osPriorityAboveNormal,
  };
  osThreadNew(audio_player_task, NULL, &audio_attr);
}
