/**
  ******************************************************************************
  * @file    audio_player.h
  * @brief   Public API for the MP3 player. UI code (SquareLine events, LVGL
  *          callbacks) calls the control functions; polling functions feed
  *          labels, icons and the progress bar.
  ******************************************************************************
  */
#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <stdint.h>

/* ---------------------------------------------------------------------------
 * 歌曲库 / Track library
 * Filled by the SD-card scan (freertos.c). The UI shows titles; the player
 * uses the internal UTF-16 paths - UI code never needs the paths.
 * ------------------------------------------------------------------------ */
uint16_t      music_get_count(void);              /* total tracks on the card */
const char   *music_get_title(uint16_t index);    /* UTF-8 display name      */
const uint16_t *music_get_path(uint16_t index);   /* UTF-16 FatFs path       */

/* ---------------------------------------------------------------------------
 * 播放控制 / Playback control
 * All are thread-safe: they post a command to the audio task. Safe to call
 * from LVGL event handlers.
 * ------------------------------------------------------------------------ */
void audio_player_start(void);              /* create task, play track 0 */
void audio_player_play(uint16_t index);     /* play a specific track     */
void audio_player_toggle(void);             /* play <-> pause            */
void audio_player_pause(void);
void audio_player_resume(void);
void audio_player_next(void);               /* next track (loops)        */
void audio_player_prev(void);               /* previous track (loops)    */
void audio_player_stop(void);               /* stop playback             */
void audio_player_set_volume(int volume);   /* 0..100                    */

/* ---------------------------------------------------------------------------
 * 状态查询 / Status polling
 * Called periodically by the UI task (e.g. every 100-200 ms).
 * ------------------------------------------------------------------------ */
uint16_t audio_get_current_index(void);     /* track now being played    */
uint8_t  audio_is_playing(void);            /* 1 = playing, 0 = paused   */
uint32_t audio_get_position_ms(void);       /* progress (milliseconds)   */
uint32_t audio_get_duration_ms(void);       /* total length (ms, CBR est.)*/

/* Status line shared with the UI task (audio task never touches LVGL). */
extern volatile char g_audio_status[80];
extern volatile uint8_t g_audio_status_new;

#endif /* AUDIO_PLAYER_H */
