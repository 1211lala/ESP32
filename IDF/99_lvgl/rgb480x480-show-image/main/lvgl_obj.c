
// lv_obj_create(lv_scr_act());     以屏幕为父对象创建一个对象
// lv_obj_del(lv_obj_t * obj);      删除一个对象，那么该对象的子类一并删除(立刻删除)。
// lv_obj_clean(lv_obj_t * obj);    删除一个对象的全部子类(立刻删除)。
// lv_obj_del_delayed(lv_obj_t * obj, uint32_t delay_ms);   延时毫秒再删除对象(等待删除)。

#include "lvglstart.h"
#include "stdio.h"
extern void obj_example(void);
/********************************************************************************************************************/

static lv_obj_t *bg;

uint8_t selectCode = 0;

static lv_obj_t *leftObj[10];
static lv_obj_t *cnterObj[10];
static lv_obj_t *rightObj[10];
static void button_event_cb(lv_event_t *event)
{
    static int8_t flag = 0;
    lv_obj_t *target = lv_event_get_target(event);   /*获取触发部件类型*/
    lv_event_code_t code = lv_event_get_code(event); /*获取触发事件类型*/

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        lv_obj_set_style_bg_color(target, lv_color_hex(rand() % (65535 - 1) + 1), LV_STATE_DEFAULT);
    }
    else if (code == LV_EVENT_LONG_PRESSED)
    {
        if (target == leftObj[selectCode] || target == cnterObj[selectCode] || target == rightObj[selectCode])
        {
            lv_obj_add_flag(bg, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(tile, LV_OBJ_FLAG_HIDDEN);

            lv_obj_del(bg);
        }
    }
    selectCode = rand() % (8 - 0) + 0;
    printf("selectCode: %d\r\n", selectCode);
}

void obj_example(void)
{

    bg = lv_obj_create(bgObj);
    lv_obj_set_size(bg, LCD_W, LCD_H);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x00000000), LV_STATE_DEFAULT);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    for (uint8_t i = 0; i < 9; i++)
    {
        leftObj[i] = lv_obj_create(bg);
        lv_obj_set_size(leftObj[i], 145, LCD_H / 10);
        lv_obj_set_style_bg_color(leftObj[i], lv_color_hex(rand() % (65535 - 1) + 1), LV_STATE_PRESSED);
        if (i)
        {
            lv_obj_align_to(leftObj[i], leftObj[i - 1], LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
        }
        lv_obj_add_event_cb(leftObj[i], button_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_event_cb(leftObj[i], button_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    }

    for (uint8_t i = 0; i < 9; i++)
    {
        cnterObj[i] = lv_obj_create(bg);
        lv_obj_set_size(cnterObj[i], 145, LCD_H / 10);
        lv_obj_set_style_bg_color(cnterObj[i], lv_color_hex(rand() % (65535 - 1) + 1), LV_STATE_PRESSED);
        if (i == 0)
        {
            lv_obj_align_to(cnterObj[i], leftObj[i], LV_ALIGN_OUT_RIGHT_MID, 6, 0);
        }
        if (i)
        {
            lv_obj_align_to(cnterObj[i], cnterObj[i - 1], LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
        }

        lv_obj_add_event_cb(cnterObj[i], button_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_event_cb(cnterObj[i], button_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    }

    for (uint8_t i = 0; i < 9; i++)
    {
        rightObj[i] = lv_obj_create(bg);
        lv_obj_set_size(rightObj[i], 145, LCD_H / 10);
        lv_obj_set_style_bg_color(rightObj[i], lv_color_hex(rand() % (65535 - 1) + 1), LV_STATE_PRESSED);
        if (i == 0)
        {
            lv_obj_align_to(rightObj[i], cnterObj[i], LV_ALIGN_OUT_RIGHT_MID, 6, 0);
        }

        if (i)
        {
            lv_obj_align_to(rightObj[i], rightObj[i - 1], LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
        }

        lv_obj_add_event_cb(rightObj[i], button_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_event_cb(rightObj[i], button_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    }
}
