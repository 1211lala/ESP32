#include "lvgl_bar.h"

static lv_obj_t *bar = NULL;
static lv_obj_t *bg = NULL;

static lv_obj_t *btn[3];
static int32_t value = 50;
void example_bar(void)
{
    bg = lv_obj_create(bgObj);
    lv_obj_set_size(bg, LCD_W, LCD_H);

    bar = lv_bar_create(bg);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, -100);
    lv_bar_set_range(bar, 0, 100);
    lv_obj_set_style_anim_time(bar, 500, LV_STATE_DEFAULT);
    lv_bar_set_value(bar, value, LV_ANIM_ON);

    for (uint8_t i = 0; i < 3; i++)
    {
        btn[i] = lv_btn_create(bg);
        if (i == 0)
        {
            lv_obj_align(btn[i], LV_ALIGN_LEFT_MID, 0, 50);
        }
        else if (i == 1)
        {
            lv_obj_align(btn[i], LV_ALIGN_CENTER, 0, 50);
        }
        else if (i == 2)
        {
            lv_obj_align(btn[i], LV_ALIGN_RIGHT_MID, 0, 50);
        }
        lv_obj_set_size(btn[i], 60, 30);
        lv_obj_add_event_cb(btn[i], btn_event_cb, LV_EVENT_CLICKED, NULL);
    }
}

void btn_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);

    if (target == btn[0])
    {
        value += 5;
        if (value >= 100)
            value = 100;
    }
    else if (target == btn[1])
    {
        lv_obj_add_flag(bg, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(tile, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_delayed(bg, 2000);
    }
    else if (target == btn[2])
    {
        value -= 5;
        if (value <= 0)
            value = 0;
    }
    lv_obj_set_style_anim_time(bar, 100, LV_STATE_DEFAULT);
    lv_bar_set_value(bar, value, LV_ANIM_ON);
}
