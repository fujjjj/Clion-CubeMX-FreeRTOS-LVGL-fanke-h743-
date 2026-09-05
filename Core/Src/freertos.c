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
#include "audio_player.h"
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
/* Track library filled by the SD scan. Stored in AXI SRAM (.audio_buf) so the
 * arrays do not eat DTCM; the audio player and the UI read it through the
 * music_get_*() accessors declared in audio_player.h. */
#define MAX_TRACKS      99
#define TRACK_PATH_LEN  256   /* UTF-16 code units, incl. terminator */
#define TRACK_NAME_LEN  256   /* UTF-8 bytes, incl. terminator */

__attribute__((section(".audio_buf"), aligned(4)))
static uint16_t s_track_path[MAX_TRACKS][TRACK_PATH_LEN];
__attribute__((section(".audio_buf"), aligned(4)))
static char s_track_name[MAX_TRACKS][TRACK_NAME_LEN];
static uint16_t s_track_count;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 24 * 1024,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void sd_scan_to_list(void);   /* mount SD + scan audio files into the LVGL list */

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
  audio_player_start();              /* decode + play the first MP3 found */

/* Infinite loop */
uint32_t led_last = 0;
  for(;;)
  {
    lv_timer_handler();   /* let LVGL render (flush switches LTDC framebuffer)  LVGL 的“心跳轮询”*/
    Touch_Scan();         /* touch scan, must not be slower than 10 ms */
    /* The audio task only writes to a shared buffer (LVGL is not
     * thread-safe); display it here in the LVGL task. */
    if (g_audio_status_new != 0)
    {
      g_audio_status_new = 0;
      music_list_set_status((const char *)g_audio_status);
    }
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

/* Add one file to the list, but only if its extension is a supported audio type.
 * Also remembers the full path of the first .mp3 for playback. */
static void scan_add_file(FILINFO *finfo, const uint16_t *dir_path, uint16_t *count)
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

    /* Save the full path + display name into the track library so the
     * player can open any track by index. */
    if (*count < MAX_TRACKS)
    {
      tstr_join(s_track_path[*count], TRACK_PATH_LEN, dir_path,
                (const uint16_t *)finfo->fname);
      strncpy(s_track_name[*count], utf8, TRACK_NAME_LEN - 1);
      s_track_name[*count][TRACK_NAME_LEN - 1] = 0;
    }
    (*count)++;
  }
}

/* ---- Track library accessors (declared in audio_player.h) ----------------- */
uint16_t music_get_count(void)
{
  return s_track_count;
}

const char *music_get_title(uint16_t index)
{
  if (index < s_track_count)
  {
    return s_track_name[index];
  }
  return "";
}

const uint16_t *music_get_path(uint16_t index)
{
  if (index < s_track_count)
  {
    return s_track_path[index];
  }
  return NULL;
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
      scan_add_file(&finfo, path, count);
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

  s_track_count = 0;              /* start a fresh library */
  music_list_set_status("正在挂载 SD 卡");
  lv_timer_handler();

  res = f_mount(&SDFatFS, (const TCHAR *)sd_path, 1);
  if (res != FR_OK)
  {
    lv_snprintf(buf, sizeof(buf), "SD 挂载失败 (err=%d)", res);
    music_list_set_status(buf);
    return;
  }

  music_list_set_status("正在扫描文件");
  lv_timer_handler();

  res = scan_dir((const TCHAR *)sd_path, &count);
  if (res != FR_OK)
  {
    lv_snprintf(buf, sizeof(buf), "打开目录失败 (err=%d)", res);
    music_list_set_status(buf);
    return;
  }

  s_track_count = count;
  lv_snprintf(buf, sizeof(buf), "共 %u 首", (unsigned)count);
  music_list_set_status(buf);
}

/* FreeRTOS stack-overflow hook (configCHECK_FOR_STACK_OVERFLOW=2): shows the
 * offending task name on screen so a crash can be blamed correctly. */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  (void)xTask;
  music_list_set_status("STACK OVERFLOW!");
  music_list_add_item(pcTaskName);
  for (;;)
  {
    lv_timer_handler();
    osDelay(50);
  }
}

/* USER CODE END Application */
