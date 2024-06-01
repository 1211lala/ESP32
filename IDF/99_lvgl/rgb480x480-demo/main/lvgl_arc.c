/*
 * 为圆弧添加颜色时，要使用arc_color而不是bg_color
 * 为手柄添加颜色使用bg_color

    lv_obj_set_style_arc_color(arc, lv_color_hex3(0xddc), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex3(0xff0), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(arc, lv_color_hex3(0xf60), LV_PART_KNOB);
 */

#include "lvgl_arc.h"

static lv_obj_t *bg = NULL;

static lv_obj_t *SramArc;
static lv_obj_t *PramArc;
int32_t cnt1 = 40;
int32_t cnt2 = 40;
static lv_obj_t *MatrixBtn;
static const char *MatrixMap[8] = {"ARC1:-", "ARC1:+", "\n", "ARC2:-", "ARC2:+", "\n", "return", ""};

static lv_obj_t *label1 = NULL;
static lv_obj_t *label2 = NULL;

int32_t temp = 0;
void arc_example(void)
{

    bg = lv_obj_create(bgObj);
    lv_obj_set_size(bg, LCD_W, LCD_H);

    SramArc = lv_arc_create(bg);
    lv_arc_set_range(SramArc, 0, 1000);
    lv_arc_set_value(SramArc, cnt1);
    lv_obj_align(SramArc, LV_ALIGN_CENTER, -100, -100);

    lv_obj_set_style_arc_color(SramArc, lv_color_hex(0x880000), LV_STATE_DEFAULT | LV_PART_INDICATOR);

    lv_obj_remove_style(SramArc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(SramArc, LV_OBJ_FLAG_CLICKABLE);

    label1 = lv_label_create(SramArc);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
    temp = cnt1 / 1000.0 * 100;
    lv_label_set_text_fmt(label1, "%ld%%", temp);

    PramArc = lv_arc_create(bg);
    lv_arc_set_range(PramArc, 0, 1000);
    lv_arc_set_value(PramArc, cnt2);
    /* 设置整圆 */
    lv_arc_set_bg_angles(PramArc, 0, 360);
    /* 设置开始地点 */
    lv_arc_set_rotation(PramArc, 270);
    /* 设置背景弧宽度 */
    lv_obj_set_style_arc_width(PramArc, 10, LV_PART_MAIN);
    /* 设置前景弧宽度 */
    lv_obj_set_style_arc_width(PramArc, 10, LV_PART_INDICATOR);

    lv_obj_align(PramArc, LV_ALIGN_CENTER, 100, -100);
    lv_obj_set_style_arc_color(PramArc, lv_color_hex(0x880000), LV_STATE_DEFAULT | LV_PART_INDICATOR);
    lv_obj_remove_style(PramArc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(PramArc, LV_OBJ_FLAG_CLICKABLE);
    label2 = lv_label_create(PramArc);
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);
    temp = cnt2 / 1000.0 * 100;
    lv_label_set_text_fmt(label2, "%ld%%", temp);

    MatrixBtn = lv_btnmatrix_create(bg);
    lv_btnmatrix_set_map(MatrixBtn, MatrixMap);
    lv_obj_align(MatrixBtn, LV_ALIGN_CENTER, 0, 100);
    lv_obj_set_size(MatrixBtn, 400, 200);
    lv_obj_set_style_border_width(MatrixBtn, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(MatrixBtn, matrixbtn_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void matrixbtn_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    uint16_t id = lv_btnmatrix_get_selected_btn(target); /* 获取按键索引 */
    const char *text = lv_btnmatrix_get_btn_text(target, id);
    printf("id: %d  text: %s\r\n", id, text);

    if (id == 0)
    {
        cnt1 -= 5;
    }
    if (id == 1)
    {
        cnt1 += 5;
    }
    if (id == 2)
    {
        cnt2 -= 5;
    }
    if (id == 3)
    {
        cnt2 += 5;
    }

    lv_arc_set_value(SramArc, cnt1);
    lv_arc_set_value(PramArc, cnt2);

    temp = cnt1 / 1000.0 * 100;
    lv_label_set_text_fmt(label1, "%ld%%", temp);
    temp = cnt2 / 1000.0 * 100;
    lv_label_set_text_fmt(label2, "%ld%%", temp);
    if (id == 4)
    {
        lv_obj_add_flag(bg, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(tile, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_delayed(bg, 2000);
    }
}
