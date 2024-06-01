
//lv_obj_add_state(cb, LV_STATE_CHECKED); /* 选中复选框 */
//lv_obj_clear_state(cb, LV_STATE_CHECKED); /* 清除复选框 */
//lv_obj_add_state(cb, LV_STATE_CHECKED | LV_STATE_DISABLED); /* 禁用复选框 */


#include "lvgl_checkbox.h"

static lv_obj_t *bg = NULL;
static lv_obj_t *checkbox1 = NULL;
static lv_obj_t *checkbox2 = NULL;
static lv_obj_t *checkbox3 = NULL;
static lv_obj_t *checkbox4 = NULL;
static lv_obj_t *btn = NULL;
void checkbox_example(void)
{
    bg = lv_obj_create(bgObj);
    lv_obj_set_size(bg, LCD_W, LCD_H);

    lv_obj_t * label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_40, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -200);
    lv_label_set_text(label, "MENU");


    lv_obj_t * boxObj = lv_obj_create(bg);
    lv_obj_set_size(boxObj, 440, 250);
    lv_obj_align(boxObj, LV_ALIGN_CENTER, 0, -50);

    checkbox1 = lv_checkbox_create(boxObj);
    lv_obj_align(checkbox1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_font(checkbox1, &lv_font_montserrat_28, LV_STATE_DEFAULT);
    lv_checkbox_set_text(checkbox1, "apple-r");
    lv_obj_set_style_text_color(checkbox1, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(checkbox1, lv_color_hex(0x00ff00), LV_STATE_DEFAULT|LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(checkbox1, lv_color_hex(0x0000ff), LV_STATE_CHECKED|LV_PART_INDICATOR);


    checkbox2 = lv_checkbox_create(boxObj);
    lv_checkbox_set_text(checkbox2, "apple-g");
    lv_obj_align_to(checkbox2, checkbox1, LV_ALIGN_OUT_BOTTOM_MID, 0 , 30);

    lv_obj_add_state(checkbox2, LV_STATE_CHECKED | LV_STATE_DISABLED); /* 禁用复选框 */
//    lv_obj_clear_state(checkbox2, LV_STATE_CHECKED | LV_STATE_DISABLED); /* 禁用复选框 */

    checkbox3 = lv_checkbox_create(boxObj);
    lv_checkbox_set_text(checkbox3, "apple-b");
    lv_obj_align_to(checkbox3, checkbox2, LV_ALIGN_OUT_BOTTOM_MID, 0 , 30);
    lv_obj_add_state(checkbox3, LV_STATE_CHECKED);


    checkbox4 = lv_checkbox_create(boxObj);
    lv_checkbox_set_text(checkbox4, "apple-w");
    lv_obj_align_to(checkbox4, checkbox3, LV_ALIGN_OUT_BOTTOM_MID, 0 , 30);


    lv_obj_add_event_cb(checkbox1, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(checkbox2, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(checkbox3, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(checkbox4, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);


    btn = lv_btn_create(bg);
    lv_obj_align_to(btn, boxObj, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);
    lv_obj_set_size(btn, 70, 35);
    lv_obj_add_event_cb(btn, checkbox_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}




void checkbox_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    bool state = lv_obj_has_state(target, LV_STATE_CHECKED);
    if(target == checkbox1)
    {
        printf("checkbox1 is %s\r\n", state? "ON": "OFF");
    }
    if(target == checkbox2)
    {
        printf("checkbox2 is %s\r\n", state? "ON": "OFF");
    }
    if(target == checkbox3)
    {
        printf("checkbox3 is %s\r\n", state? "ON": "OFF");
    }
    if(target == checkbox4)
    {
        printf("checkbox4 is %s\r\n", state? "ON": "OFF");
    }
    if(target == btn)
    {
        lv_obj_add_flag(bg, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(tile, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_delayed(bg, 2000);
    }
}
