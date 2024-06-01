
#ifndef _LVGLSTART_H_
#define _LVGLSTART_H_

#include "lvgl.h"
#include "stdio.h"
#define loginfo printf

#define LCD_W 480
#define LCD_H 480

#define LCD_BLACK 0x000000      // 黑色
#define LCD_WHITE 0xFFFFFF      // 白色
#define LCD_RED 0xFF0000        // 红色
#define LCD_GREEN 0x00FF00      // 绿色
#define LCD_BLUE 0x0000FF       // 蓝色
#define LCD_YELLOW 0xFFFF00     // 黄色
#define LCD_CYAN 0x00FFFF       // 青色（蓝绿）
#define LCD_MAGENTA 0xFF00FF    // 品红色（紫红）
#define LCD_GRAY 0x808080       // 灰色（50%）
#define LCD_LIGHT_GRAY 0xC0C0C0 // 浅灰色
#define LCD_DARK_GRAY 0x404040  // 深灰色

extern lv_obj_t *bgObj;

extern lv_obj_t *tile;

extern lv_obj_t *tile0;
extern lv_obj_t *tile1;

extern lv_timer_t *t1;

extern lv_obj_t *obj[];
extern lv_obj_t *lable[];
extern uint32_t color[];


extern lv_obj_t *timeObj;
extern lv_obj_t *wifiObj;
extern lv_obj_t *wifi_label;
extern lv_obj_t *sramObj;
extern lv_obj_t *pramObj;

void menu_slider(void);
void scroll_event_cb(lv_event_t *e);
void obj_event_cb(lv_event_t *e);
void timer_cb1(lv_timer_t *t);

#endif
