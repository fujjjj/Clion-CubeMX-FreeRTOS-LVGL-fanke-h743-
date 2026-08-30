/**
  ******************************************************************************
  * @file    music_list.c
  * @brief   Hand-written LVGL screen showing microSD file entries.
  *
  *          Layout: status bar on top, scrollable list below (800x480 panel).
  ******************************************************************************
  */

#include "music_list.h"
#include "lvgl.h"

#define LIST_BG_COLOR   lv_color_hex(0x101418)
#define LIST_PANEL      lv_color_hex(0x1A1F26)
#define LIST_BORDER     lv_color_hex(0x2A2F36)
#define LIST_TEXT       lv_color_hex(0xFFFFFF)

static lv_obj_t *scr;
static lv_obj_t *status_label;
static lv_obj_t *file_list;
static uint16_t item_count;

void music_list_create(void)
{
  if (scr != NULL)
  {
    return;
  }

  scr = lv_obj_create(NULL);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(scr, LIST_BG_COLOR, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_STATE_DEFAULT);

  status_label = lv_label_create(scr);
  lv_obj_set_style_text_color(status_label, LIST_TEXT, LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(status_label, &lv_font_simsun_16_cjk, LV_STATE_DEFAULT);
  lv_obj_align(status_label, LV_ALIGN_TOP_MID, 0, 12);

  file_list = lv_list_create(scr);
  lv_obj_set_size(file_list, 760, 420);
  lv_obj_align(file_list, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(file_list, LIST_PANEL, LV_STATE_DEFAULT);
  /* Perf experiment (方案2): drop decorations that cost per-frame redraws. */
  lv_obj_set_style_bg_opa(file_list, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(file_list, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(file_list, 1, LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(file_list, LIST_BORDER, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(file_list, 2, LV_STATE_DEFAULT);

  item_count = 0;
  lv_scr_load(scr);
}

void music_list_set_status(const char *text)
{
  if (status_label == NULL)
  {
    return;
  }
  lv_label_set_text(status_label, text);
}

void music_list_add_item(const char *text)
{
  lv_obj_t *lbl;

  if (file_list == NULL)
  {
    return;
  }
  lbl = lv_list_add_text(file_list, text);
  /* lv_list_add_text defaults to LV_LABEL_LONG_SCROLL_CIRCULAR (single line,
     tail clipped until the marquee scrolls). Switch to WRAP so the whole
     song name is always visible; the item height grows automatically. */
  lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(lbl, LV_PCT(100));
  /* Theme paints every item with an opaque grey background; make items
     transparent so scrolling only draws the text (fewer pixel fills). */
  lv_obj_set_style_bg_opa(lbl, LV_OPA_TRANSP, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(lbl, LIST_TEXT, LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(lbl, &lv_font_simsun_16_cjk, LV_STATE_DEFAULT);
  item_count++;
}

void music_list_clear(void)
{
  if (file_list != NULL)
  {
    lv_obj_clean(file_list);
  }
  item_count = 0;
}

uint16_t music_list_get_count(void)
{
  return item_count;
}
