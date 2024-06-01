#include "lvgl_arc.h"

static lv_obj_t *bg = NULL;

static lv_obj_t * SramArc;
static lv_obj_t * PramArc;

int32_t SramTotal = 300;
int32_t PramTotal = 80000;
int32_t SramUsed = 100;
int32_t PramUsed = 6000;
void arc_example(void)
{
    bg = lv_obj_create(bgObj);
    lv_obj_set_size(bg, LCD_W, LCD_H);

    SramArc = lv_arc_create(bg);
    lv_arc_set_range(SramArc, 0, SramTotal);
    lv_arc_set_value(SramArc, SramUsed);
    lv_obj_remove_style(SramArc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(SramArc, LV_OBJ_FLAG_CLICKABLE);
}
