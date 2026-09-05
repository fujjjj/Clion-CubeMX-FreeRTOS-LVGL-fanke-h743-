/**
  ******************************************************************************
  * @file    audio_player.c
  * @brief   Command-driven MP3 player:
  *          SD (FatFs) -> minimp3 decode (CPU) -> double-buffer SAI DMA
  *          -> ES8311. UI talks to it through audio_player.h.
  *
  * Key facts (each one learned the hard way - see README "Lessons Learned"):
  *  - PCM buffers live in AXI SRAM (.audio_buf); DTCM is not DMA-safe here.
  *  - DMA runs in NORMAL (single-shot) mode; the ISR restarts it on the
  *    other buffer, so the audio stream never stops.
  *  - Decoded PCM is flushed from the DCache before DMA reads it.
  *  - minimp3 heap is a free-list allocator in SDRAM so track switches can
  *    free/reuse memory (a bump allocator leaks 32KB per song).
  *  - The audio task never touches LVGL; UI polls g_audio_status instead.
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
#include "queue.h"
#include "cmsis_os.h"
#include <string.h>

/* ---- minimp3 heap in SDRAM (free-list, supports free) --------------------- */
#define AUDIO_HEAP_BASE 0xC0800000UL
#define AUDIO_HEAP_SIZE (128u * 1024u)
#define AUDIO_DEC_BASE  0xC0820000UL   /* mp3dec_ex_t, outside the heap */

typedef struct audio_heap_hdr
{
  uint32_t size;                 /* payload bytes (8-aligned) */
  struct audio_heap_hdr *next;   /* free-list link */
} audio_heap_hdr_t;

static audio_heap_hdr_t *s_heap_free;
static uint32_t s_heap_init_done;

static void audio_heap_init(void)
{
  if (s_heap_init_done)
  {
    return;
  }
  audio_heap_hdr_t *h = (audio_heap_hdr_t *)AUDIO_HEAP_BASE;
  h->size = AUDIO_HEAP_SIZE - sizeof(audio_heap_hdr_t);
  h->next = NULL;
  s_heap_free = h;
  s_heap_init_done = 1;
}

static void *audio_malloc(size_t size)
{
  uint32_t need = (uint32_t)((size + 7u) & ~7u);
  audio_heap_hdr_t *prev = NULL;
  audio_heap_hdr_t *cur = s_heap_free;

  while (cur != NULL)
  {
    if (cur->size >= need + sizeof(audio_heap_hdr_t))
    {
      /* split: keep the tail on the free list */
      audio_heap_hdr_t *rest = (audio_heap_hdr_t *)((uint8_t *)cur +
                                  sizeof(audio_heap_hdr_t) + need);
      rest->size = cur->size - need - sizeof(audio_heap_hdr_t);
      rest->next = cur->next;
      if (prev) { prev->next = rest; } else { s_heap_free = rest; }
      return (void *)((uint8_t *)cur + sizeof(audio_heap_hdr_t));
    }
    if (cur->size >= need)   /* exact fit */
    {
      if (prev) { prev->next = cur->next; } else { s_heap_free = cur->next; }
      return (void *)((uint8_t *)cur + sizeof(audio_heap_hdr_t));
    }
    prev = cur;
    cur = cur->next;
  }
  return NULL;
}

static void audio_free(void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }
  audio_heap_hdr_t *h = (audio_heap_hdr_t *)((uint8_t *)ptr -
                                            sizeof(audio_heap_hdr_t));
  h->next = s_heap_free;
  s_heap_free = h;
}

static void *audio_realloc(void *ptr, size_t size)
{
  void *np = audio_malloc(size);
  if (np != NULL && ptr != NULL)
  {
    memcpy(np, ptr, size);
    audio_free(ptr);
  }
  return np;
}

/* ---- minimp3 (single header, public domain) ------------------------------- */
#define MINIMP3_IMPLEMENTATION
#define MINIMP3_NO_STDIO
#define MINIMP3_IO_SIZE (32 * 1024)
#include "minimp3.h"
#define malloc audio_malloc
#define free   audio_free
#define realloc audio_realloc
#include "minimp3_ex.h"
#undef malloc
#undef free
#undef realloc

/* ---- PCM double buffer in AXI SRAM (DMA-safe) ----------------------------- */
#define PCM_FRAME 1152   /* MPEG1 samples per channel per frame */
__attribute__((section(".audio_buf"), aligned(32)))
static int16_t pcm_buf[2][PCM_FRAME * 2];

/* ---- shared state ---------------------------------------------------------- */
volatile char g_audio_status[80];
volatile uint8_t g_audio_status_new;

static void audio_status(const char *s)
{
  strncpy((char *)g_audio_status, s, sizeof(g_audio_status) - 1);
  g_audio_status[sizeof(g_audio_status) - 1] = 0;
  g_audio_status_new = 1;
}

/* ---- commands from the UI -------------------------------------------------- */
typedef enum
{
  CMD_PLAY = 0,   /* arg = track index */
  CMD_PAUSE,
  CMD_RESUME,
  CMD_TOGGLE,
  CMD_NEXT,
  CMD_PREV,
  CMD_VOLUME,     /* arg = 0..100 */
  CMD_STOP
} audio_cmd_e;

typedef struct
{
  audio_cmd_e cmd;
  int arg;
} audio_msg_t;

static QueueHandle_t g_cmd_q;
static SemaphoreHandle_t g_tx_sem;

/* Public playback state, read by the UI. */
static volatile uint16_t s_cur_index;
static volatile uint8_t s_playing;      /* 1 playing, 0 paused/stopped */
static volatile uint32_t s_pos_ms;
static volatile uint32_t s_dur_ms;

/* Player internals. */
static FIL g_mp3fil;
static uint32_t g_file_size;
static mp3dec_io_t g_mp3io;
static mp3dec_ex_t *g_dec;
static es8311_handle_t g_es;
static int s_volume = 40;
static uint8_t s_es_init_done;

/* Double-buffer state, shared with the DMA ISR. */
static volatile int s_cur;          /* buffer the DMA is playing now */
static volatile uint8_t s_ready[2];

extern DMA_HandleTypeDef hdma_sai1_b;   /* defined in sai.c */

/* ---- public state accessors ------------------------------------------------ */
uint16_t audio_get_current_index(void) { return s_cur_index; }
uint8_t  audio_is_playing(void)        { return s_playing; }
uint32_t audio_get_position_ms(void)   { return s_pos_ms; }
uint32_t audio_get_duration_ms(void)   { return s_dur_ms; }

/* ---- command posting (safe from any task / LVGL event) -------------------- */
static void post_cmd(audio_cmd_e cmd, int arg)
{
  audio_msg_t m;
  m.cmd = cmd;
  m.arg = arg;
  if (g_cmd_q != NULL)
  {
    xQueueSend(g_cmd_q, &m, 0);
  }
}

void audio_player_play(uint16_t index)   { post_cmd(CMD_PLAY, index); }
void audio_player_pause(void)            { post_cmd(CMD_PAUSE, 0); }
void audio_player_resume(void)           { post_cmd(CMD_RESUME, 0); }
void audio_player_toggle(void)           { post_cmd(CMD_TOGGLE, 0); }
void audio_player_next(void)             { post_cmd(CMD_NEXT, 0); }
void audio_player_prev(void)             { post_cmd(CMD_PREV, 0); }
void audio_player_stop(void)             { post_cmd(CMD_STOP, 0); }
void audio_player_set_volume(int volume)
{
  if (volume < 0) { volume = 0; }
  if (volume > 100) { volume = 100; }
  post_cmd(CMD_VOLUME, volume);
}

/* ---- FatFs callbacks for minimp3_ex ---------------------------------------- */
static size_t audio_read_cb(void *buf, size_t size, void *user_data)
{
  UINT br = 0;
  FRESULT fr = f_read((FIL *)user_data, buf, (UINT)size, &br);
  return (fr == FR_OK) ? (size_t)br : 0;
}

static int audio_seek_cb(uint64_t pos, void *user_data)
{
  return (f_lseek((FIL *)user_data, (FSIZE_t)pos) == FR_OK) ? 0 : -1;
}

/* ---- 44.1 kHz clocking ------------------------------------------------------ */
static void audio_clock_44100(void)
{
  RCC_PeriphCLKInitTypeDef s = {0};
  s.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  s.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  /* PLL2: 25MHz/25 * (225 + 6488/8192) = 225.792MHz, /2 = 112.896MHz.
   * SAI MCKDIV stays 10 -> MCLK 11.2896MHz = 44100 * 256. */
  s.PLL2.PLL2M = 25;
  s.PLL2.PLL2N = 225;
  s.PLL2.PLL2FRACN = 6488.0;
  s.PLL2.PLL2P = 2;
  s.PLL2.PLL2Q = 2;
  s.PLL2.PLL2R = 2;
  s.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  s.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  HAL_RCCEx_PeriphCLKConfig(&s);
}

static int audio_es8311_init(void)
{
  g_es = es8311_create(&hi2c2, ES8311_ADDRRES_0);
  if (g_es == NULL)
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
  if (es8311_init(g_es, &clk, ES8311_RESOLUTION_16,
                  ES8311_RESOLUTION_16) != ES8311_OK)
  {
    return -1;
  }
  es8311_voice_volume_set(g_es, s_volume, NULL);
  return 0;
}

/* DMA TX complete ISR: switch to the other, already decoded buffer. */
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  (void)hsai;
  BaseType_t w = pdFALSE;
  int done = s_cur;
  int nxt = done ^ 1;

  if (s_ready[nxt] != 0 &&
      HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)pcm_buf[nxt],
                           (uint16_t)(PCM_FRAME * 2)) == HAL_OK)
  {
    s_cur = nxt;
    s_ready[done] = 0;
    xSemaphoreGiveFromISR(g_tx_sem, &w);
    portYIELD_FROM_ISR(w);
    return;
  }
  /* under-run / EOF: stop at the frame boundary */
  __HAL_SAI_DISABLE(&hsai_BlockB1);
}

/* Decode one MPEG frame into out[]. Short/EOF frames are zero-padded. */
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

/* Start DMA on a freshly decoded buffer (called from task context only). */
static int start_playback(int buf_idx)
{
  s_cur = buf_idx;
  s_ready[0] = 1;
  s_ready[1] = 1;
  if (HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)pcm_buf[buf_idx],
                           (uint16_t)(PCM_FRAME * 2)) != HAL_OK)
  {
    return -1;
  }
  return 0;
}

/* Re-decode both buffers from the decoder's current position and restart. */
static int restart_buffers(void)
{
  if (decode_frame(pcm_buf[0]) <= 0 || decode_frame(pcm_buf[1]) <= 0)
  {
    return -1;
  }
  SCB_CleanDCache_by_Addr((uint32_t *)pcm_buf[0], sizeof(pcm_buf[0]));
  SCB_CleanDCache_by_Addr((uint32_t *)pcm_buf[1], sizeof(pcm_buf[1]));
  return start_playback(0);
}

/* ---- playing state machine ------------------------------------------------- */
enum { REQ_EOF = 0, REQ_NEXT, REQ_PREV, REQ_PLAY, REQ_STOP };

/* Play one track until EOF or a UI command. Returns the requested next
 * action; for REQ_PLAY *play_index_out receives the target index. */
static int play_track(uint16_t index, uint16_t *play_index_out)
{
  char buf[96];
  FRESULT fr;
  const uint16_t *path = music_get_path(index);
  int req = REQ_EOF;

  if (path == NULL)
  {
    return REQ_STOP;
  }

  fr = f_open(&g_mp3fil, (const TCHAR *)path, FA_READ);
  if (fr != FR_OK)
  {
    audio_status("打开 MP3 失败");
    return REQ_STOP;
  }
  g_file_size = (uint32_t)f_size(&g_mp3fil);

  if (s_es_init_done == 0)
  {
    /* One-time hardware setup. */
    __HAL_SAI_DISABLE(&hsai_BlockB1);
    audio_clock_44100();
    if (audio_es8311_init() != 0)
    {
      audio_status("ES8311 初始化失败");
      f_close(&g_mp3fil);
      return REQ_STOP;
    }
    s_es_init_done = 1;
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
    return REQ_STOP;
  }

  /* CBR duration estimate: file_bytes * 8 / kbps -> ms. */
  uint32_t kbps = (uint32_t)g_dec->info.bitrate_kbps;
  s_dur_ms = (kbps > 0) ? (g_file_size * 8u) / kbps : 0u;
  s_pos_ms = 0;
  s_cur_index = index;

  if (restart_buffers() != 0)
  {
    audio_status("解码失败");
    goto exit_track;
  }

  lv_snprintf(buf, sizeof(buf), "%s", music_get_title(index));
  audio_status(buf);
  s_playing = 1;

playing:
  for (;;)
  {
    audio_msg_t m;

    if (xSemaphoreTake(g_tx_sem, pdMS_TO_TICKS(50)) == pdTRUE)
    {
      int fill = s_cur ^ 1;
      if (decode_frame(pcm_buf[fill]) <= 0)
      {
        /* EOF: let the ISR play out the last buffer, then auto-next. */
        s_ready[fill] = 0;
        xSemaphoreTake(g_tx_sem, pdMS_TO_TICKS(100));
        req = REQ_EOF;
        goto exit_track;
      }
      SCB_CleanDCache_by_Addr((uint32_t *)pcm_buf[fill], sizeof(pcm_buf[fill]));
      s_ready[fill] = 1;

      if (g_dec->info.hz > 0)
      {
        s_pos_ms = (uint32_t)((g_dec->cur_sample * 1000u) /
                              (uint64_t)g_dec->info.hz);
      }
    }

    while (xQueueReceive(g_cmd_q, &m, 0) == pdTRUE)
    {
      switch (m.cmd)
      {
        case CMD_PAUSE:
        case CMD_TOGGLE:
          s_playing = 0;
          HAL_SAI_DMAStop(&hsai_BlockB1);
          goto paused;
        case CMD_VOLUME:
          s_volume = m.arg;
          if (g_es != NULL) { es8311_voice_volume_set(g_es, s_volume, NULL); }
          break;
        case CMD_NEXT:  req = REQ_NEXT;  goto exit_track;
        case CMD_PREV:  req = REQ_PREV;  goto exit_track;
        case CMD_PLAY:  req = REQ_PLAY;
                        *play_index_out = (uint16_t)m.arg;
                        goto exit_track;
        case CMD_STOP:  req = REQ_STOP;  goto exit_track;
        default:        break;
      }
    }
  }

paused:
  /* DMA is stopped. Wait for resume / skip / stop commands. */
  for (;;)
  {
    audio_msg_t m;
    if (xQueueReceive(g_cmd_q, &m, portMAX_DELAY) != pdTRUE)
    {
      continue;
    }
    switch (m.cmd)
    {
      case CMD_RESUME:
      case CMD_TOGGLE:
        /* Drain any stale "buffer done" events, refill both buffers from the
         * decoder's current position and resume. */
        while (xSemaphoreTake(g_tx_sem, 0) == pdTRUE) { }
        if (restart_buffers() != 0)
        {
          req = REQ_STOP;
          goto exit_track;
        }
        s_playing = 1;
        goto playing;
      case CMD_VOLUME:
        s_volume = m.arg;
        if (g_es != NULL) { es8311_voice_volume_set(g_es, s_volume, NULL); }
        break;
      case CMD_NEXT:  req = REQ_NEXT;  goto exit_track;
      case CMD_PREV:  req = REQ_PREV;  goto exit_track;
      case CMD_PLAY:  req = REQ_PLAY;
                      *play_index_out = (uint16_t)m.arg;
                      goto exit_track;
      case CMD_STOP:  req = REQ_STOP;  goto exit_track;
      default:        break;
    }
  }

exit_track:
  HAL_SAI_DMAStop(&hsai_BlockB1);
  s_playing = 0;
  mp3dec_ex_close(g_dec);
  f_close(&g_mp3fil);
  return req;
}

/* ---- player task ------------------------------------------------------------ */
static void audio_player_task(void *arg)
{
  (void)arg;
  audio_heap_init();
  g_cmd_q = xQueueCreate(8, sizeof(audio_msg_t));
  g_tx_sem = xSemaphoreCreateBinary();

  int next_index = 0;              /* auto-play the first track */

  for (;;)
  {
    uint16_t count = music_get_count();
    if (count == 0)
    {
      audio_status("没有音频文件");
      s_playing = 0;
      audio_msg_t m;
      if (xQueueReceive(g_cmd_q, &m, portMAX_DELAY) == pdTRUE &&
          m.cmd == CMD_PLAY)
      {
        next_index = m.arg;
      }
      continue;
    }

    if (next_index < 0) { next_index = 0; }
    if (next_index >= (int)count) { next_index = 0; }

    uint16_t play_index = (uint16_t)next_index;
    int req = play_track(play_index, &play_index);

    switch (req)
    {
      case REQ_EOF:
      case REQ_NEXT:
        next_index = (int)play_index + 1;
        break;
      case REQ_PREV:
        next_index = (int)play_index - 1;
        break;
      case REQ_PLAY:
        next_index = (int)play_index;
        break;
      case REQ_STOP:
      default:
        /* Wait for the UI to ask for something. */
        audio_msg_t m;
        if (xQueueReceive(g_cmd_q, &m, portMAX_DELAY) == pdTRUE)
        {
          if (m.cmd == CMD_PLAY)      { next_index = m.arg; }
          else if (m.cmd == CMD_NEXT) { next_index = (int)play_index + 1; }
          else if (m.cmd == CMD_PREV) { next_index = (int)play_index - 1; }
          else                        { next_index = 0; }
        }
        break;
    }

    count = music_get_count();
    if (count > 0)
    {
      next_index %= (int)count;
      if (next_index < 0) { next_index += (int)count; }
    }
  }
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
