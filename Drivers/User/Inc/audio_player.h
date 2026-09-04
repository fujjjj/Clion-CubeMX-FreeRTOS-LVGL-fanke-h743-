/**
  ******************************************************************************
  * @file    audio_player.h
  * @brief   MP3 playback: minimp3 stream decode -> SAI1_B TX DMA -> ES8311.
  ******************************************************************************
  */
#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <stdint.h>

/* Full UTF-16 path of the file to play (filled by the SD scan in freertos.c). */
extern uint16_t g_play_path[300];

/* Status text + "new" flag shared with the LVGL task (audio task never
 * touches LVGL directly). */
extern volatile char g_audio_status[80];
extern volatile uint8_t g_audio_status_new;

/* Create the audio player task (safe to call more than once). */
void audio_player_start(void);

#endif /* AUDIO_PLAYER_H */
