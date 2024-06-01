#include "lvglstart.h"
#include "stdio.h"
#include "lvgl_obj.h"
#include "lvgl_bar.h"
#include "lvgl_arc.h"
#include "p_wifi_sta.h"
#define OBJ_H 250
#define OBJ_W 250

#define JIANGE OBJ_W + 50
#define objNum 7

lv_obj_t *bgObj;
lv_obj_t *tile;
lv_obj_t *tile0;
lv_obj_t *tile1;
lv_timer_t *t1;
int move = 0;

lv_obj_t *obj[objNum];
lv_obj_t *label[objNum];
char *label_str[objNum] = {"BTN", "BAR", "ARC", "NULL", "NULL", "NULL", "NULL"};
uint32_t color[objNum] = {0xff0020, 0xff00ff, 0x0000ff, 0x00ffff, 0x00ff00, 0xffff00, 0x000000};

lv_obj_t *timeObj;
lv_obj_t *wifiObj;
lv_obj_t *wifi_label;
lv_obj_t *sramObj;
lv_obj_t *pramObj;

void menu_slider(void)
{
    static lv_style_t style;

    lv_style_init(&style);
    lv_style_set_radius(&style, 20);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_color_hex(0x04dae2));
    lv_style_set_bg_grad_color(&style, lv_color_hex(0xfb04ff));
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_HOR);
    lv_style_set_bg_main_stop(&style, 0);
    lv_style_set_bg_grad_stop(&style, 255);

    // lv_obj_add_style(btn, &style, LV_PART_MAIN);

    /***********************************************/
    bgObj = lv_scr_act();
    lv_obj_clear_flag(bgObj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x8f8f8f), LV_PART_MAIN | LV_STATE_DEFAULT);

    tile = lv_tileview_create(bgObj);
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    tile0 = lv_tileview_add_tile(tile, 0, 0, LV_DIR_VER);
    lv_obj_set_style_bg_color(tile0, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    for (uint16_t i = 0; i < objNum; i++)
    {
        if (objNum == i + 1)
        {
            obj[i] = lv_obj_create(tile0);
            lv_obj_align(obj[i], LV_ALIGN_CENTER, i * 250, -50);
            lv_obj_set_size(obj[i], 80, 80);
            lv_obj_set_style_bg_color(obj[i], lv_color_hex(0x000000), LV_PART_MAIN);
            lv_obj_set_style_border_width(obj[i], 0, LV_STATE_DEFAULT);
        }
        else
        {
            obj[i] = lv_btn_create(tile0);
            lv_obj_set_style_radius(obj[i], 20, LV_PART_MAIN);
            lv_obj_align(obj[i], LV_ALIGN_CENTER, i * 250, -50);
            lv_obj_set_style_bg_color(obj[i], lv_color_hex(color[i]), LV_PART_MAIN);
            lv_obj_set_size(obj[i], 120, 120);
            lv_obj_add_event_cb(obj[i], obj_event_cb, LV_EVENT_LONG_PRESSED, NULL);
            label[i] = lv_label_create(obj[i]);
            lv_label_set_text_fmt(label[i], "%s", label_str[i]);
            lv_obj_set_style_text_color(label[i], lv_color_hex(0xffffffff), LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(label[i], &lv_font_montserrat_14, LV_STATE_DEFAULT);
            lv_obj_set_align(label[i], LV_ALIGN_CENTER);
        }
    }
    // lv_obj_add_event_cb(tile0,scroll_event_cb,LV_EVENT_SCROLL_END, NULL);
    // t1=lv_timer_create(timer_cb1,10, NULL);

    tile1 = lv_tileview_add_tile(tile, 0, 1, LV_DIR_VER);
    lv_obj_add_style(tile1, &style, LV_PART_MAIN);
    // lv_obj_set_style_bg_color(tile0, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    timeObj = lv_obj_create(tile1);
    lv_obj_set_size(timeObj, 320, 320);
    lv_obj_align(timeObj, LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_style_bg_color(timeObj, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(timeObj, 0, LV_STATE_DEFAULT);

    wifiObj = lv_obj_create(tile1);
    lv_obj_align_to(wifiObj, timeObj, LV_ALIGN_OUT_TOP_MID, 20, 20);
    lv_obj_align(wifiObj, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_set_size(wifiObj, 300, 100);
    lv_obj_set_style_bg_color(wifiObj, lv_color_hex(0x0ff000), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(wifiObj, 0, LV_STATE_DEFAULT);

    wifi_label = lv_label_create(wifiObj);
    lv_label_set_text_fmt(wifi_label, "WIFI: %s\n\n  IP : %s", "Connect Failed!!!", "NULL");
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_28, LV_STATE_DEFAULT);
    lv_obj_align(wifi_label, LV_ALIGN_CENTER, 0, 0);

    sramObj = lv_obj_create(tile1);
    lv_obj_align_to(sramObj, timeObj, LV_ALIGN_OUT_RIGHT_TOP, 5, -(20 + 100));
    lv_obj_set_size(sramObj, 120, 120);
    lv_obj_set_style_bg_color(sramObj, lv_color_hex(0x00FF00), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(sramObj, 0, LV_STATE_DEFAULT);

    pramObj = lv_obj_create(tile1);
    lv_obj_set_size(pramObj, 120, 120);
    lv_obj_align_to(pramObj, timeObj, LV_ALIGN_OUT_RIGHT_TOP, 5, 20);
    lv_obj_set_style_bg_color(pramObj, lv_color_hex(0x000FF0), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(pramObj, 0, LV_STATE_DEFAULT);
}

void obj_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (target == obj[0])
    {
        lv_obj_add_flag(tile, LV_OBJ_FLAG_HIDDEN);
        obj_example();
    }
    if (target == obj[1])
    {
        lv_obj_add_flag(tile, LV_OBJ_FLAG_HIDDEN);
        example_bar();
    }
    if (target == obj[2])
    {
        lv_obj_add_flag(tile, LV_OBJ_FLAG_HIDDEN);
        arc_example();
    }
    if (target == obj[3])
    {
        printf("obj4 long pressing \r\n");
    }
    if (target == obj[4])
    {
        printf("obj5 long pressing \r\n");
    }
    if (target == obj[5])
    {
        printf("obj6 long pressing \r\n");
    }
}

void scroll_event_cb(lv_event_t *e)
{
    move = lv_obj_get_scroll_x(tile0);

    if (move % 150 != 0)
    {
        if (move % 150 < 75)
        {
            lv_obj_scroll_to_x(tile0, move - (move % 150), LV_ANIM_ON);
        }
        if (move % 150 > 74)
        {
            lv_obj_scroll_to_x(tile0, move + 150 - (move % 150), LV_ANIM_ON);
        }
    }
}

void timer_cb1(lv_timer_t *t)
{
    int move;
    move = lv_obj_get_scroll_x(tile0);
    if (move <= 150 && move >= 0)
    {
        lv_obj_set_width(obj[0], 130 - move / 3);
        lv_obj_set_height(obj[0], 130 - move / 3);
        lv_obj_set_y(obj[0], -move / 3);

        lv_obj_set_width(obj[1], 80 + move / 3);
        lv_obj_set_height(obj[1], 80 + move / 3);
        lv_obj_set_y(obj[1], move / 3 - 50);
    }

    if (move <= 300 && move >= 150)
    {
        lv_obj_set_width(obj[1], 130 - (move - 150) / 3);
        lv_obj_set_height(obj[1], 130 - (move - 150) / 3);
        lv_obj_set_y(obj[1], -(move - 150) / 3);

        lv_obj_set_width(obj[2], 80 + (move - 150) / 3);
        lv_obj_set_height(obj[2], 80 + (move - 150) / 3);
        lv_obj_set_y(obj[2], (move - 150) / 3 - 50);
    }

    if (move <= 450 && move >= 300)
    {
        lv_obj_set_width(obj[2], 130 - (move - 300) / 3);
        lv_obj_set_height(obj[2], 130 - (move - 300) / 3);
        lv_obj_set_y(obj[2], -(move - 300) / 3);

        lv_obj_set_width(obj[3], 80 + (move - 300) / 3);
        lv_obj_set_height(obj[3], 80 + (move - 300) / 3);
        lv_obj_set_y(obj[3], (move - 300) / 3 - 50);
    }

    if (move <= 600 && move >= 450)
    {
        lv_obj_set_width(obj[3], 130 - (move - 450) / 3);
        lv_obj_set_height(obj[3], 130 - (move - 450) / 3);
        lv_obj_set_y(obj[3], -(move - 450) / 3);

        lv_obj_set_width(obj[4], 80 + (move - 450) / 3);
        lv_obj_set_height(obj[4], 80 + (move - 450) / 3);
        lv_obj_set_y(obj[4], (move - 450) / 3 - 50);
    }

    if (move <= 750 && move >= 600)
    {
        lv_obj_set_width(obj[4], 130 - (move - 600) / 3);
        lv_obj_set_height(obj[4], 130 - (move - 600) / 3);
        lv_obj_set_y(obj[4], -(move - 600) / 3);

        lv_obj_set_width(obj[5], 80 + (move - 600) / 3);
        lv_obj_set_height(obj[5], 80 + (move - 600) / 3);
        lv_obj_set_y(obj[5], (move - 600) / 3 - 50);
    }

    if (move <= 900 && move >= 750)
    {
        lv_obj_set_width(obj[5], 130 - (move - 750) / 3);
        lv_obj_set_height(obj[5], 130 - (move - 750) / 3);
        lv_obj_set_y(obj[5], -(move - 750) / 3);

        lv_obj_set_width(obj[6], 80 + (move - 750) / 3);
        lv_obj_set_height(obj[6], 80 + (move - 750) / 3);
        lv_obj_set_y(obj[6], (move - 750) / 3 - 50);
    }

    if (move <= 1050 && move >= 900)
    {
        lv_obj_set_width(obj[6], 130 - (move - 900) / 3);
        lv_obj_set_height(obj[6], 130 - (move - 900) / 3);
        lv_obj_set_y(obj[6], -(move - 900) / 3);
    }
}
