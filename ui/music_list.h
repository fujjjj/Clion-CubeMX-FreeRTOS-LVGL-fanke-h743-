/**
  ******************************************************************************
  * @file    music_list.h
  * @brief   Hand-written LVGL screen showing microSD file entries.
  ******************************************************************************
  */
#ifndef __MUSIC_LIST_H
#define __MUSIC_LIST_H

#include <stdint.h>

void music_list_create(void);
void music_list_set_status(const char *text);
void music_list_add_item(const char *text);
void music_list_clear(void);
uint16_t music_list_get_count(void);

#endif /* __MUSIC_LIST_H */
