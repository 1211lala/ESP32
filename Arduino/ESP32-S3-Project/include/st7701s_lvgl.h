#ifndef _ST7701S_LVGL_H_
#define _ST7701S_LVGL_H_


#include "common.h"
#include <Arduino_GFX_Library.h>
#include <lvgl.h>

int st7701s_lvgl_init(void);
void lvgl_runing_app(void);

#define HEIGHT 480
#define WIDTH 480
#define st7701s_bl 38


#define BUF_SIZE WIDTH * 200 /* 但是感觉在这里设置的不怎么对, 要在 lv_conf.h 中改才行 */


#endif // !_ST7701S_LVGL_H_