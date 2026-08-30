/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "touch_800x480.h"
#include "lvgl.h"
#include "fatfs.h"
#include "music_list.h"
#include "i2c.h"
#include "es8311.h"
#include "sai.h"
#include <math.h>
#include <string.h>   /* strrchr / strcmp: 文件格式过滤要用 */
#include <strings.h>  /* strcasecmp: 大小写不敏感比较 */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void sd_scan_to_list(void);   /* mount SD + scan audio files into the LVGL list */
void es8311_hw_test(void);    /* temporary: verify ES8311 I2C link */
void es8311_sine_play(void);  /* temporary: play a 1 kHz sine over SAI1_B */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Show the file list screen, then mount + scan the SD card into it. */
  music_list_create();
  sd_scan_to_list();
  es8311_hw_test();                  /* temporary: verify ES8311 I2C link */
  es8311_sine_play();                /* temporary: sine wave out of the speaker */

/* Infinite loop */
uint32_t led_last = 0;
  for(;;)
  {
    lv_timer_handler();   /* let LVGL render (flush switches LTDC framebuffer)  LVGL 的“心跳轮询”*/
    Touch_Scan();         /* touch scan, must not be slower than 10 ms */
    if (osKernelGetTickCount() - led_last >= 500)   /* toggle every 500 ms (1 Hz cycle) */
    {
      led_last = osKernelGetTickCount();
      LED1_Toggle;        /* heartbeat */
    }
osDelay(5);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/*
 * SD card scanning helpers.
 *
 * NOTE: these functions were originally lost when CubeMX regenerated
 * freertos.c (the previous copy lived outside a USER CODE block). They have
 * been reconstructed 1:1 from the last successful build artifacts
 * (build/Debug/.../freertos.c.obj + Clion_h7_01.map) and are now kept inside
 * this USER CODE section so regeneration can never wipe them again.
 */

/* Mount path of the SD logical drive (UTF-16, FatFs _LFN_UNICODE = 1). */
static const uint16_t sd_path[] = { '0', ':', '/', 0 };   /* "0:/" */

/* UTF-16 -> UTF-8, with surrogate pair support. */
static void utf16_to_utf8(const uint16_t *in, uint8_t *out, int out_cap)
{
  int n = 0;

  while ((n + 3) < out_cap && *in != 0)
  {
    uint32_t cp = *in++;

    /* Surrogate pair -> one code point (U+10000..U+10FFFF). */
    if (cp >= 0xD800 && cp < 0xDC00 &&
        in[0] >= 0xDC00 && in[0] < 0xE000)
    {
      cp = ((cp - 0xD800) << 10) + (*in++ - 0xDC00) + 0x10000;
    }

    if (cp < 0x80)
    {
      out[n++] = (uint8_t)cp;
    }
    else if (cp < 0x800)
    {
      out[n++] = (uint8_t)(0xC0 | (cp >> 6));
      out[n++] = (uint8_t)(0x80 | (cp & 0x3F));
    }
    else if (cp < 0x10000)
    {
      out[n++] = (uint8_t)(0xE0 | (cp >> 12));
      out[n++] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
      out[n++] = (uint8_t)(0x80 | (cp & 0x3F));
    }
    else
    {
      out[n++] = (uint8_t)(0xF0 | (cp >> 18));
      out[n++] = (uint8_t)(0x80 | ((cp >> 12) & 0x3F));
      out[n++] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
      out[n++] = (uint8_t)(0x80 | (cp & 0x3F));
    }
  }

  out[n] = 0;
}

/* Join two UTF-16 path parts with a '/' separator (unless already there). */
static void tstr_join(uint16_t *dest, int cap, const uint16_t *src, const uint16_t *src2)
{
  int i = 0;
  int j = 0;

  for (i = 0; src[i] != 0 && (i + 1) < cap; i++)
  {
    dest[i] = src[i];
  }

  if (i > 0 && dest[i - 1] != '/' && (i + 1) < cap)
  {
    dest[i++] = '/';
  }

  for (j = 0; src2[j] != 0 && (i + 1) < cap; i++, j++)
  {
    dest[i] = src2[j];
  }

  dest[i] = 0;
}

/* Add one file to the list, but only if its extension is a supported audio type. */
static void scan_add_file(FILINFO *finfo, uint16_t *count)
{
  static const char *const audio_ext[] = {
    ".mp3", ".wav", ".flac", ".wma", ".aac", ".ogg", ".m4a"
  };
  char utf8[766];
  char line[773];
  uint8_t matched = 0;

  utf16_to_utf8((const uint16_t *)finfo->fname, (uint8_t *)utf8, (int)sizeof(utf8));

  char *dot = strrchr(utf8, '.');
  if (dot != NULL)
  {
    for (uint32_t i = 0; i < (sizeof(audio_ext) / sizeof(audio_ext[0])); i++)
    {
      if (strcasecmp(dot, audio_ext[i]) == 0)
      {
        matched = 1;
        break;
      }
    }
  }

  if (matched)
  {
    lv_snprintf(line, sizeof(line), "%s", utf8);
    music_list_add_item(line);
    (*count)++;
  }
}

/* Recursively scan a directory (sub-directories included), max 99 entries. */
static FRESULT scan_dir(const TCHAR *path, uint16_t *count)
{
  DIR dj;
  FILINFO finfo;
  FRESULT res;

  res = f_opendir(&dj, path);
  if (res != FR_OK)
  {
    return res;
  }

  for (;;)
  {
    res = f_readdir(&dj, &finfo);
    if (res != FR_OK)
    {
      break;
    }
    if (finfo.fname[0] == 0)      /* end of directory */
    {
      break;
    }
    if (finfo.fname[0] == '.')    /* skip "." and ".." */
    {
      continue;
    }
    if (*count > 99)              /* safety limit */
    {
      continue;
    }

    if (finfo.fattrib & AM_DIR)
    {
      TCHAR new_path[518];
      tstr_join((uint16_t *)new_path, 518, path, (const uint16_t *)finfo.fname);
      scan_dir(new_path, count);
    }
    else
    {
      scan_add_file(&finfo, count);
    }
  }

  f_closedir(&dj);
  return FR_OK;
}

/* Mount the SD card and scan it into the LVGL music list. */
void sd_scan_to_list(void)
{
  char buf[64];
  uint16_t count = 0;
  FRESULT res;

  music_list_set_status("正在挂载 SD 卡...");
  lv_timer_handler();

  res = f_mount(&SDFatFS, (const TCHAR *)sd_path, 1);
  if (res != FR_OK)
  {
    lv_snprintf(buf, sizeof(buf), "SD 挂载失败 (err=%d)", res);
    music_list_set_status(buf);
    return;
  }

  music_list_set_status("正在扫描文件...");
  lv_timer_handler();

  res = scan_dir((const TCHAR *)sd_path, &count);
  if (res != FR_OK)
  {
    lv_snprintf(buf, sizeof(buf), "打开目录失败 (err=%d)", res);
    music_list_set_status(buf);
    return;
  }

  lv_snprintf(buf, sizeof(buf), "共 %d 首", count);
  music_list_set_status(buf);
}

/* Temporary hardware check for the ES8311 codec module (I2C + basic init).
 * Result is shown in the status label of the music list screen. */
void es8311_hw_test(void)
{
  char buf[64];
  uint8_t id1 = 0, id2 = 0, ver = 0;
  es8311_handle_t dev;

  music_list_set_status("ES8311 I2C test...");
  lv_timer_handler();

  /* 1) Raw I2C probe: read the chip ID registers (0xFD / 0xFE / 0xFF). */
  if (HAL_I2C_Mem_Read(&hi2c2, (uint16_t)(ES8311_ADDRRES_0 << 1), 0xFD,
                       I2C_MEMADD_SIZE_8BIT, &id1, 1, 100) != HAL_OK)
  {
    music_list_set_status("ES8311 I2C FAIL: no ACK at 0x18");
    return;
  }
  HAL_I2C_Mem_Read(&hi2c2, (uint16_t)(ES8311_ADDRRES_0 << 1), 0xFE,
                   I2C_MEMADD_SIZE_8BIT, &id2, 1, 100);
  HAL_I2C_Mem_Read(&hi2c2, (uint16_t)(ES8311_ADDRRES_0 << 1), 0xFF,
                   I2C_MEMADD_SIZE_8BIT, &ver, 1, 100);

  lv_snprintf(buf, sizeof(buf), "ES8311 I2C OK: FD=%02X FE=%02X FF=%02X", id1, id2, ver);
  music_list_set_status(buf);
  lv_timer_handler();

  /* 2) Run the full ported driver init (48 kHz, MCLK 12.288 MHz, 16 bit). */
  dev = es8311_create(&hi2c2, ES8311_ADDRRES_0);
  if (dev == NULL)
  {
    music_list_set_status("ES8311 create failed");
    return;
  }

  const es8311_clock_config_t clk = {
    .mclk_inverted = false,
    .sclk_inverted = false,
    .mclk_from_mclk_pin = true,
    .mclk_frequency = 12288000,
    .sample_frequency = 48000
  };

  if (es8311_init(dev, &clk, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16) == ES8311_OK)
  {
    es8311_voice_volume_set(dev, 70, NULL);
    music_list_set_status("ES8311 init OK, vol=70");
  }
  else
  {
    music_list_set_status("ES8311 init FAIL");
  }
}

/* Temporary sine-wave generator: 1 kHz, 48 kHz, 16-bit stereo. */
#define SINE_SAMPLES 480   /* 10 ms @ 48 kHz -> loops seamlessly */
static int16_t sine_buf[SINE_SAMPLES * 2];

static void sine_fill(void)
{
  const float step = 2.0f * 3.14159265f * 1000.0f / 48000.0f;
  float phase = 0.0f;

  for (int i = 0; i < SINE_SAMPLES; i++)
  {
    int16_t v = (int16_t)(sinf(phase) * 32767.0f);
    sine_buf[i * 2] = v;      /* left */
    sine_buf[i * 2 + 1] = v;  /* right */
    phase += step;
  }
}

/* Start SAI1_B TX and stream the sine forever.
 * NOTE: blocking test only - it will be replaced by a DMA + FreeRTOS audio
 * task later, and the LVGL task stops updating while this runs. */
void es8311_sine_play(void)
{
  sine_fill();
  music_list_set_status("SAI sine 1kHz...");
  lv_timer_handler();

  for (;;)
  {
    /* Size = frames x slots (2 slots, 16-bit stereo) */
    if (HAL_SAI_Transmit(&hsai_BlockB1, (uint8_t *)sine_buf, SINE_SAMPLES * 2, 100) != HAL_OK)
    {
      music_list_set_status("SAI TX error");
      break;
    }
  }
}

/* USER CODE END Application */
