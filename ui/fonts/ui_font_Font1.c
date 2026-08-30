/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font D:/SquareLine_Studio/test/assets/HarmonyOS_Sans_SC_Light.ttf -o D:/SquareLine_Studio/test/assets\ui_font_Font1.c --format lvgl -r 0x20-0x7f --symbols 我是杨鑫愿大王 --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_FONT1
#define UI_FONT_FONT1 1
#endif

#if UI_FONT_FONT1

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0x88,

    /* U+0022 "\"" */
    0xb6, 0xd0,

    /* U+0023 "#" */
    0x8, 0x82, 0x41, 0x11, 0xff, 0x11, 0x4, 0x82,
    0x20, 0x88, 0xff, 0x88, 0x82, 0x41, 0x10,

    /* U+0024 "$" */
    0x10, 0x20, 0xe2, 0xa9, 0x32, 0x24, 0x28, 0x38,
    0x28, 0x48, 0x99, 0x3a, 0xdf, 0x8, 0x10, 0x20,

    /* U+0025 "%" */
    0x70, 0x91, 0x22, 0x24, 0x45, 0x7, 0x20, 0x8,
    0x1, 0x0, 0x4e, 0xa, 0x22, 0x44, 0x48, 0x90,
    0xe0,

    /* U+0026 "&" */
    0x3c, 0x19, 0x84, 0x21, 0x8, 0x64, 0xe, 0x3,
    0x9, 0x22, 0x85, 0xa0, 0xc8, 0x13, 0x1a, 0x3c,
    0xc0,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x12, 0x44, 0x48, 0x88, 0x88, 0x88, 0x44, 0x22,

    /* U+0029 ")" */
    0x84, 0x22, 0x21, 0x11, 0x11, 0x11, 0x22, 0x44,

    /* U+002A "*" */
    0x25, 0x5c, 0xea, 0x90,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0xd8,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x8, 0x20, 0x84, 0x10, 0x42, 0x8, 0x21, 0x4,
    0x20,

    /* U+0030 "0" */
    0x38, 0x8b, 0x1c, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x89, 0x11, 0xc0,

    /* U+0031 "1" */
    0x35, 0x91, 0x11, 0x11, 0x11, 0x11,

    /* U+0032 "2" */
    0x3c, 0x8e, 0x8, 0x10, 0x20, 0x82, 0xc, 0x30,
    0x41, 0x7, 0xf0,

    /* U+0033 "3" */
    0x3c, 0x8c, 0x8, 0x10, 0x63, 0x1, 0x81, 0x3,
    0x5, 0x11, 0xc0,

    /* U+0034 "4" */
    0x8, 0x8, 0x10, 0x30, 0x20, 0x44, 0x44, 0x84,
    0xff, 0x4, 0x4, 0x4,

    /* U+0035 "5" */
    0x7c, 0x81, 0x2, 0x7, 0x98, 0x80, 0x81, 0x3,
    0x5, 0x11, 0xc0,

    /* U+0036 "6" */
    0x8, 0x20, 0x41, 0x7, 0x88, 0xa0, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+0037 "7" */
    0xfe, 0x4, 0x10, 0x20, 0xc1, 0x2, 0x8, 0x10,
    0x60, 0x81, 0x0,

    /* U+0038 "8" */
    0x3d, 0x8e, 0xc, 0x14, 0x67, 0x11, 0x41, 0x83,
    0x5, 0x11, 0xc0,

    /* U+0039 "9" */
    0x38, 0x8a, 0xc, 0x18, 0x28, 0xdf, 0x6, 0x8,
    0x20, 0x41, 0x0,

    /* U+003A ":" */
    0x80, 0x80,

    /* U+003B ";" */
    0xc0, 0x0, 0xd8,

    /* U+003C "<" */
    0x2, 0x18, 0xc6, 0xe, 0x3, 0x1, 0x80,

    /* U+003D "=" */
    0xfe, 0x0, 0x7, 0xf0,

    /* U+003E ">" */
    0x80, 0xc0, 0x60, 0x30, 0xe6, 0x30, 0x0,

    /* U+003F "?" */
    0x7a, 0x38, 0x41, 0xc, 0x21, 0x8, 0x20, 0x80,
    0x0, 0x20,

    /* U+0040 "@" */
    0x7, 0xc0, 0x60, 0xc2, 0x1, 0x10, 0x2, 0x47,
    0x4e, 0x13, 0x18, 0x84, 0x62, 0x11, 0x88, 0x46,
    0x21, 0x18, 0x4c, 0x91, 0xce, 0x40, 0x0, 0x80,
    0x1, 0x86, 0x3, 0xe0,

    /* U+0041 "A" */
    0x4, 0x3, 0x0, 0xc0, 0x48, 0x12, 0x8, 0x42,
    0x10, 0xfc, 0x40, 0x90, 0x24, 0x6, 0x1,

    /* U+0042 "B" */
    0xfc, 0x86, 0x82, 0x82, 0x86, 0xfc, 0x82, 0x81,
    0x81, 0x81, 0x82, 0xfc,

    /* U+0043 "C" */
    0x1e, 0x10, 0x90, 0x30, 0x8, 0x4, 0x2, 0x1,
    0x0, 0x80, 0x20, 0x48, 0x43, 0xc0,

    /* U+0044 "D" */
    0xfc, 0x43, 0x20, 0x50, 0x38, 0xc, 0x6, 0x3,
    0x1, 0x81, 0xc0, 0xa1, 0x9f, 0x80,

    /* U+0045 "E" */
    0xff, 0x2, 0x4, 0x8, 0x1f, 0xa0, 0x40, 0x81,
    0x2, 0x7, 0xf0,

    /* U+0046 "F" */
    0xff, 0x2, 0x4, 0x8, 0x1f, 0xa0, 0x40, 0x81,
    0x2, 0x4, 0x0,

    /* U+0047 "G" */
    0x1e, 0x10, 0xd0, 0x18, 0x8, 0x4, 0x3e, 0x3,
    0x1, 0xc0, 0xa0, 0x48, 0x63, 0xc0,

    /* U+0048 "H" */
    0x80, 0xc0, 0x60, 0x30, 0x18, 0xf, 0xfe, 0x3,
    0x1, 0x80, 0xc0, 0x60, 0x30, 0x10,

    /* U+0049 "I" */
    0xff, 0xf0,

    /* U+004A "J" */
    0x8, 0x42, 0x10, 0x84, 0x21, 0x8, 0x67, 0xe0,

    /* U+004B "K" */
    0x81, 0x41, 0x21, 0x11, 0x9, 0x5, 0x83, 0x21,
    0x10, 0x84, 0x41, 0x20, 0xd0, 0x20,

    /* U+004C "L" */
    0x81, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81,
    0x2, 0x7, 0xf0,

    /* U+004D "M" */
    0x80, 0x38, 0xf, 0x1, 0xd0, 0x59, 0x13, 0x22,
    0x62, 0x8c, 0x21, 0x84, 0x30, 0x6, 0x0, 0xc0,
    0x10,

    /* U+004E "N" */
    0x80, 0xe0, 0x68, 0x34, 0x19, 0xc, 0x46, 0x23,
    0x9, 0x82, 0xc1, 0x60, 0x70, 0x10,

    /* U+004F "O" */
    0x1e, 0x8, 0x44, 0xa, 0x1, 0x80, 0x60, 0x18,
    0x6, 0x1, 0x80, 0x50, 0x22, 0x10, 0x78,

    /* U+0050 "P" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0xbe, 0x40, 0x81,
    0x2, 0x4, 0x0,

    /* U+0051 "Q" */
    0x1e, 0x4, 0x21, 0x2, 0x40, 0x28, 0x5, 0x0,
    0xa0, 0x14, 0x2, 0x80, 0x48, 0x10, 0x84, 0xf,
    0x0, 0x10, 0x1, 0x80, 0x18,

    /* U+0052 "R" */
    0xf8, 0x84, 0x82, 0x82, 0x82, 0x84, 0xf8, 0x88,
    0x84, 0x84, 0x82, 0x81,

    /* U+0053 "S" */
    0x79, 0x8a, 0xc, 0xc, 0x6, 0x3, 0x1, 0x3,
    0x7, 0x1b, 0xe0,

    /* U+0054 "T" */
    0xff, 0x84, 0x2, 0x1, 0x0, 0x80, 0x40, 0x20,
    0x10, 0x8, 0x4, 0x2, 0x1, 0x0,

    /* U+0055 "U" */
    0x80, 0xc0, 0x60, 0x30, 0x18, 0xc, 0x6, 0x3,
    0x1, 0x80, 0xe0, 0xd8, 0xc7, 0xc0,

    /* U+0056 "V" */
    0x80, 0x50, 0x24, 0x8, 0x82, 0x21, 0x8, 0x41,
    0x10, 0x48, 0x12, 0x2, 0x80, 0xc0, 0x10,

    /* U+0057 "W" */
    0x81, 0x2, 0x83, 0x5, 0xa, 0x12, 0x14, 0x22,
    0x28, 0x44, 0x49, 0x9, 0x12, 0x12, 0x24, 0x14,
    0x28, 0x30, 0x60, 0x60, 0xc0, 0x40, 0x80,

    /* U+0058 "X" */
    0x40, 0xd8, 0x22, 0x10, 0x48, 0xa, 0x3, 0x0,
    0xc0, 0x68, 0x12, 0x8, 0x44, 0x9, 0x3,

    /* U+0059 "Y" */
    0x80, 0xa0, 0x90, 0x44, 0x41, 0x40, 0xa0, 0x20,
    0x10, 0x8, 0x4, 0x2, 0x1, 0x0,

    /* U+005A "Z" */
    0xff, 0x2, 0x2, 0x4, 0x8, 0x8, 0x10, 0x30,
    0x20, 0x40, 0x40, 0xff,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x49, 0x27,

    /* U+005C "\\" */
    0x81, 0x4, 0x8, 0x20, 0x81, 0x4, 0x10, 0x20,
    0x82,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0x92, 0x4f,

    /* U+005E "^" */
    0x21, 0x45, 0x12, 0x8a, 0x20,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0xa4,

    /* U+0061 "a" */
    0x3c, 0x8c, 0x9, 0xfc, 0x30, 0x60, 0xe3, 0x7a,

    /* U+0062 "b" */
    0x81, 0x2, 0x4, 0xb, 0x98, 0xa0, 0xc1, 0x83,
    0x6, 0xe, 0x2b, 0x80,

    /* U+0063 "c" */
    0x3c, 0x8e, 0x4, 0x8, 0x10, 0x20, 0x23, 0x3c,

    /* U+0064 "d" */
    0x2, 0x4, 0x8, 0x13, 0xa8, 0xe0, 0xc1, 0x83,
    0x6, 0xa, 0x33, 0xa0,

    /* U+0065 "e" */
    0x38, 0x8a, 0xc, 0x1f, 0xf0, 0x20, 0x23, 0x3c,

    /* U+0066 "f" */
    0x3a, 0x10, 0x8f, 0x21, 0x8, 0x42, 0x10, 0x84,
    0x0,

    /* U+0067 "g" */
    0x3a, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xa3, 0x3a,
    0x4, 0xa, 0x23, 0x80,

    /* U+0068 "h" */
    0x81, 0x2, 0x4, 0xb, 0xd8, 0xe0, 0xc1, 0x83,
    0x6, 0xc, 0x18, 0x20,

    /* U+0069 "i" */
    0x9f, 0xf0,

    /* U+006A "j" */
    0x10, 0x1, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1e,

    /* U+006B "k" */
    0x81, 0x2, 0x4, 0x8, 0x51, 0x24, 0x50, 0xf1,
    0xa2, 0x24, 0x28, 0x40,

    /* U+006C "l" */
    0xff, 0xf8,

    /* U+006D "m" */
    0xb9, 0xd9, 0xce, 0x10, 0xc2, 0x18, 0x43, 0x8,
    0x61, 0xc, 0x21, 0x84, 0x20,

    /* U+006E "n" */
    0xbd, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x82,

    /* U+006F "o" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x83, 0x42,
    0x3c,

    /* U+0070 "p" */
    0xb9, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xe2, 0xb9,
    0x2, 0x4, 0x8, 0x0,

    /* U+0071 "q" */
    0x3a, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xa3, 0x3a,
    0x4, 0x8, 0x10, 0x20,

    /* U+0072 "r" */
    0xbc, 0x88, 0x88, 0x88, 0x80,

    /* U+0073 "s" */
    0x7a, 0x38, 0x30, 0x38, 0x10, 0x61, 0x78,

    /* U+0074 "t" */
    0x42, 0x11, 0xe4, 0x21, 0x8, 0x42, 0x10, 0x70,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xe3, 0x7a,

    /* U+0076 "v" */
    0x81, 0x42, 0x42, 0x24, 0x24, 0x24, 0x18, 0x18,
    0x18,

    /* U+0077 "w" */
    0x82, 0x14, 0x61, 0x46, 0x24, 0x52, 0x29, 0x22,
    0x94, 0x28, 0xc1, 0xc, 0x10, 0x80,

    /* U+0078 "x" */
    0x42, 0x88, 0x90, 0xc1, 0x3, 0x9, 0x22, 0x42,

    /* U+0079 "y" */
    0x81, 0x42, 0x42, 0x44, 0x24, 0x24, 0x18, 0x18,
    0x18, 0x10, 0x10, 0x20, 0xe0,

    /* U+007A "z" */
    0xfc, 0x20, 0x84, 0x30, 0x84, 0x10, 0xfc,

    /* U+007B "{" */
    0x19, 0x8, 0x42, 0x10, 0x84, 0xc1, 0x8, 0x42,
    0x10, 0x83,

    /* U+007C "|" */
    0xff, 0xff,

    /* U+007D "}" */
    0xc1, 0x8, 0x42, 0x10, 0x84, 0x19, 0x8, 0x42,
    0x10, 0x98,

    /* U+007E "~" */
    0x65, 0x2a, 0x30,

    /* U+5927 "大" */
    0x2, 0x0, 0x8, 0x0, 0x20, 0x0, 0x80, 0xff,
    0xfc, 0x8, 0x0, 0x30, 0x0, 0xc0, 0x2, 0x80,
    0x12, 0x0, 0x84, 0x4, 0x8, 0x20, 0x13, 0x0,
    0x30, 0x0, 0x0,

    /* U+613F "愿" */
    0x3f, 0xfe, 0x41, 0x0, 0x9f, 0xf1, 0x20, 0x22,
    0x7f, 0xc4, 0x80, 0x89, 0xff, 0x11, 0x20, 0x24,
    0x58, 0x53, 0x8d, 0x82, 0x2, 0x52, 0xa5, 0x21,
    0x32, 0x3e, 0x0,

    /* U+6211 "我" */
    0x0, 0x80, 0x3a, 0x8f, 0x9, 0x4, 0x22, 0x10,
    0x83, 0xff, 0xf1, 0x8, 0x4, 0x22, 0x12, 0x91,
    0xf2, 0xc1, 0x6, 0x4, 0x31, 0x13, 0x44, 0x50,
    0xa7, 0x1, 0x80,

    /* U+662F "是" */
    0x3f, 0xf0, 0x80, 0x42, 0x1, 0xf, 0xfc, 0x20,
    0x10, 0xff, 0xc0, 0x0, 0x3f, 0xff, 0x2, 0x0,
    0x88, 0x2, 0x1f, 0x8c, 0x80, 0x4a, 0x2, 0x1f,
    0xf0,

    /* U+6768 "杨" */
    0x20, 0x0, 0x8f, 0xe2, 0x3, 0x3e, 0x10, 0x21,
    0x80, 0x8f, 0xf7, 0x12, 0x6a, 0x49, 0xa2, 0x26,
    0x89, 0x12, 0x44, 0x4a, 0x21, 0x21, 0xc, 0x88,
    0xe0,

    /* U+738B "王" */
    0x7f, 0xf8, 0x8, 0x0, 0x20, 0x0, 0x80, 0x2,
    0x1, 0xff, 0xe0, 0x20, 0x0, 0x80, 0x2, 0x0,
    0x8, 0x0, 0x20, 0x3f, 0xff,

    /* U+946B "鑫" */
    0x3, 0xc0, 0x1c, 0x38, 0x67, 0xc6, 0x3f, 0xfc,
    0x9, 0x10, 0x9, 0x10, 0x7f, 0xfe, 0x18, 0x28,
    0x26, 0xc6, 0x58, 0x38, 0x7e, 0xfe, 0xa, 0x14,
    0x2c, 0x54, 0x7a, 0xfe
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 69, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 58, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 78, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 5, .adv_w = 159, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 142, .box_w = 7, .box_h = 18, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 36, .adv_w = 193, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 176, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 43, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 71, .adv_w = 82, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 79, .adv_w = 82, .box_w = 4, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 87, .adv_w = 105, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 91, .adv_w = 142, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 98, .adv_w = 58, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 99, .adv_w = 126, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 100, .adv_w = 56, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 94, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 142, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 142, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 62, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 64, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 221, .adv_w = 142, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 228, .adv_w = 142, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 232, .adv_w = 142, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 239, .adv_w = 107, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 253, .box_w = 14, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 277, .adv_w = 166, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 161, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 304, .adv_w = 164, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 318, .adv_w = 181, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 332, .adv_w = 148, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 176, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 186, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 63, .box_w = 1, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 384, .adv_w = 111, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 166, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 406, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 218, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 186, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 193, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 147, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 474, .adv_w = 193, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 495, .adv_w = 156, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 140, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 532, .adv_w = 181, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 546, .adv_w = 164, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 244, .box_w = 15, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 584, .adv_w = 163, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 599, .adv_w = 153, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 625, .adv_w = 80, .box_w = 3, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 631, .adv_w = 94, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 80, .box_w = 3, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 646, .adv_w = 114, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 651, .adv_w = 102, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 652, .adv_w = 67, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 653, .adv_w = 138, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 661, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 673, .adv_w = 125, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 681, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 693, .adv_w = 137, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 701, .adv_w = 84, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 710, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 722, .adv_w = 146, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 734, .adv_w = 59, .box_w = 1, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 59, .box_w = 4, .box_h = 16, .ofs_x = -2, .ofs_y = -4},
    {.bitmap_index = 744, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 756, .adv_w = 58, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 758, .adv_w = 216, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 771, .adv_w = 146, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 779, .adv_w = 151, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 788, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 800, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 812, .adv_w = 91, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 817, .adv_w = 114, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 824, .adv_w = 89, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 832, .adv_w = 146, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 840, .adv_w = 126, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 849, .adv_w = 195, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 863, .adv_w = 122, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 871, .adv_w = 127, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 884, .adv_w = 119, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 891, .adv_w = 87, .box_w = 5, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 901, .adv_w = 40, .box_w = 1, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 903, .adv_w = 87, .box_w = 5, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 913, .adv_w = 142, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 916, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 943, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 970, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 997, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1022, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1047, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1068, .adv_w = 256, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x818, 0x8ea, 0xd08, 0xe41, 0x1a64, 0x3b44
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 22823, .range_length = 15173, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 7, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 0,
    1, 2, 3, 0, 4, 0, 4, 0,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4,
    6, 7, 8, 9, 10, 7, 11, 12,
    13, 14, 14, 15, 16, 17, 14, 14,
    7, 18, 0, 19, 20, 21, 15, 5,
    22, 23, 24, 25, 2, 8, 3, 0,
    0, 0, 26, 27, 28, 29, 30, 31,
    32, 26, 0, 33, 34, 29, 26, 26,
    27, 27, 0, 35, 36, 37, 32, 38,
    38, 39, 38, 40, 2, 0, 3, 4,
    0, 0, 0, 0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 1, 0, 2, 0, 0, 0, 0,
    2, 0, 3, 0, 4, 5, 4, 5,
    6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0,
    7, 8, 6, 9, 8, 9, 9, 9,
    8, 9, 9, 10, 9, 9, 9, 9,
    8, 9, 8, 9, 11, 12, 13, 14,
    15, 16, 17, 18, 0, 14, 3, 0,
    5, 0, 19, 20, 21, 21, 21, 22,
    21, 20, 0, 23, 20, 20, 24, 24,
    21, 0, 21, 24, 25, 26, 27, 28,
    28, 29, 28, 30, 0, 0, 3, 4,
    0, 0, 0, 0, 0, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, 0, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 5, 0, 5, 5, 3,
    0, 3, 0, 0, 13, 0, 0, 4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -9, 6, 5, -10, -33, -21, 7, -7,
    0, -33, -1, 5, 0, 0, 0, 0,
    0, 0, -18, 0, -17, -5, 0, -12,
    -14, -1, -11, -8, -11, -7, 0, 0,
    0, -7, -25, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, 0,
    -11, -7, 0, 0, 0, -10, 0, -8,
    0, -7, -4, -7, -12, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -9, -27, 0, -14, 6, 0,
    -17, -7, 0, 0, 0, -21, -3, -21,
    -15, 0, -24, 5, 0, 0, -2, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, -2, 0, 0, 0, -3, 0, 0,
    0, 4, 0, -9, 0, -9, -3, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 6,
    0, -6, 5, 0, 8, -4, 0, 0,
    0, 1, 0, 1, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    2, 0, 2, -3, 0, 2, 0, 0,
    0, -3, 0, 0, -3, 0, -4, 0,
    -3, -4, 0, 0, -3, -3, -3, -4,
    -2, 0, -3, 6, 0, 1, -34, -16,
    8, -3, 0, -41, 0, 4, 0, 0,
    0, 0, 0, 0, -14, 0, -10, -5,
    0, -8, 0, -5, 0, -6, -10, -9,
    0, 0, 0, 0, 4, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, -3, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -9, 0, 0, -26, 5, 0,
    2, -13, -2, 0, -2, 0, -4, 0,
    0, 0, 0, 0, -6, 0, -7, -9,
    0, -3, -3, -7, -8, -12, -6, 0,
    -12, -25, 0, -23, 6, 0, -19, -13,
    0, 4, -4, -34, -12, -38, -27, 0,
    -45, 0, -4, 0, -7, -7, 0, -1,
    -2, -8, -8, -23, 0, 0, -3, 5,
    0, 2, -35, -17, 4, 0, 0, -40,
    0, 0, 0, 1, 1, -3, 0, -8,
    -9, 0, -9, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, -1, 0, -2,
    10, 0, -1, -3, 0, 0, 2, -2,
    -3, -5, -3, 0, -10, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 5, 0, 0, -3,
    0, 0, -3, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, 7, 0, -22, -26, -21, 8, -8,
    0, -35, 0, 4, 0, 5, 5, 0,
    0, 0, -30, 0, -25, -11, 0, -23,
    -28, -12, -22, -23, -25, -23, -3, 4,
    0, -5, -18, -15, 0, -4, 0, -17,
    0, 5, 0, 0, 0, 0, 0, 0,
    -18, 0, -15, -3, 0, -11, -10, 0,
    -6, -4, -6, -8, 0, 0, 5, -21,
    3, 0, 6, -7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    -7, 0, 0, -3, -3, -5, -6, -10,
    0, 0, -8, 5, 5, -17, -30, -24,
    3, -12, 0, -35, -3, 0, 0, 0,
    0, 0, 0, 0, -27, 0, -24, -11,
    0, -19, -22, -7, -17, -15, -16, -18,
    0, 0, 3, -12, 5, 0, 2, -8,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, -2, 0, -4, 0, 0, 0,
    0, 0, 0, -8, 0, 0, 0, -10,
    0, 0, 0, 0, -7, 0, 0, 0,
    0, -21, 0, -17, -15, -2, -23, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, -12, 0, -3,
    -8, 0, -9, 0, 0, 0, 0, -25,
    0, -17, -15, -7, -24, 0, -3, 0,
    0, -2, 0, 0, 0, -1, 0, -4,
    -5, -6, 0, 1, 0, 6, 6, 0,
    -3, 0, 0, 0, 0, -17, 0, -10,
    -5, 5, -16, 0, 0, 0, -1, 2,
    0, 0, 0, 5, 0, 0, 3, 3,
    0, 0, 3, 0, 0, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    0, 4, 1, 0, -6, 0, 0, 0,
    0, -16, 0, -13, -9, -2, -18, 0,
    0, 0, 0, 0, 0, 0, 3, 0,
    0, 2, 1, 0, 0, 12, 0, -2,
    -18, 0, 10, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    -5, 0, 0, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, -23, 0, -12,
    -11, 0, -19, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 6, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, -4, 7, 0, -10, 0, 0, 0,
    0, -20, 0, -12, -8, 0, -16, 0,
    -6, 0, -5, 0, 0, 0, -3, 0,
    -1, 0, 0, 0, 0, 7, 0, 2,
    -24, -8, -7, 0, 0, -26, 0, 0,
    0, -8, 0, -8, -14, 0, -11, 0,
    -8, 0, 0, 0, -2, 6, 0, 0,
    0, 0, 0, -8, 0, 0, 0, 0,
    -8, 0, 0, 0, 0, -23, 0, -16,
    -13, 0, -24, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 0, -1, 0, 3,
    0, -1, 1, -3, 6, 0, -7, 0,
    0, 0, 0, -20, 0, -10, 0, 0,
    -15, 0, 0, 0, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -18, -8, -5, 0, 0, -18,
    0, -23, 0, -8, -4, -10, -15, 0,
    -3, 0, -4, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    3, 0, -9, 0, 0, 0, 0, -25,
    0, -11, -6, 0, -16, 0, -4, 0,
    -5, 0, 0, 0, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0,
    -9, 0, 0, 0, 0, -25, 0, -8,
    -6, 0, -19, 0, -5, 0, -6, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 40,
    .right_class_cnt     = 30,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Font1 = {
#else
lv_font_t ui_font_Font1 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 19,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_FONT1*/

