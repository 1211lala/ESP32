#include "st7701s_lvgl.h"
#include "touch.h"
#include <esp32-hal-dac.h> //DAC功能引用该库，可以不进行#include<>引用

#define st7701s_bl 38

#define HEIGHT 480
#define WIDTH 480
#define BUF_SIZE WIDTH * 200 /* 但是感觉在这里设置的不怎么对, 要在 lv_conf.h 中改才行 */

Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    11 /* R0 */, 12 /* R1 */, 13 /* R2 */, 14 /* R3 */, 0 /* R4 */,
    8 /* G0 */, 20 /* G1 */, 3 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    4 /* B0 */, 5 /* B1 */, 6 /* B2 */, 7 /* B3 */, 15 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    WIDTH /* width */, HEIGHT /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type1_init_operations, sizeof(st7701_type1_init_operations));

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/**************************************************************************************
 * 函数功能:
 **************************************************************************************/
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
#ifndef DIRECT_MODE
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif
#endif // #ifndef DIRECT_MODE

    lv_disp_flush_ready(disp);
}
/**************************************************************************************
 * 函数功能:
 **************************************************************************************/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    if (touch_has_signal())
    {
        if (touch_touched())
        {
            data->state = LV_INDEV_STATE_PR;

            /*Set the coordinates*/
            data->point.x = touch_last_x;
            data->point.y = touch_last_y;
        }
        else if (touch_released())
        {
            data->state = LV_INDEV_STATE_REL;
        }
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}
/**************************************************************************************
 * 函数功能:
 **************************************************************************************/

void st7701s_init(uint8_t level)
{
    pinMode(st7701s_bl, OUTPUT);
    analogWrite(st7701s_bl, 100);
}

void set_light(uint8_t level)
{
    level = 40 + level * 21;
    if (level >= 255)
    {
        level = 255;
    }
    else if (level <= 40)
    {
        level = 40;
    }
    loginfo("set_light = %d\r\n", level);
    analogWrite(st7701s_bl, level);
}
/**************************************************************************************
 * 函数功能:
 **************************************************************************************/
int st7701s_lvgl_init(void)
{
    if (!gfx->begin())
    {
        loginfo("gfx->begin() failed!");
        return ESP_FAIL;
    }
    gfx->fillScreen(BLACK);
    st7701s_init(1);

    touch_init();
    lv_init();

    screenWidth = gfx->width();
    screenHeight = gfx->height();

    /* 分配内存 类型为 8bit 从内部ram中分配 */
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!disp_draw_buf)
    {
        /* 如果分配失败，则取消 MALLOC_CAP_INTERNAL 属性，从 spiram中配置 */
        disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * BUF_SIZE, MALLOC_CAP_8BIT);
        loginfo("lvgl buf is allowed in psram\r\n");
    }
    if (!disp_draw_buf)
    {
        loginfo("disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * bufSize, MALLOC_CAP_8BIT)  failed!");
        return ESP_FAIL;
    }
    else
    {
        lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, BUF_SIZE);
        /* 初始化显示 */
        lv_disp_drv_init(&disp_drv);
        /* 设置屏幕分辨率,打点函数, 缓冲缓存 */
        disp_drv.hor_res = screenWidth;
        disp_drv.ver_res = screenHeight;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.draw_buf = &draw_buf;

        lv_disp_drv_register(&disp_drv);

        /* 初始化输入设备 */
        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = my_touchpad_read;
        lv_indev_drv_register(&indev_drv);
    }
    return ESP_OK;
}
// /**************************************************************************************
//  * 函数功能:
//  **************************************************************************************/
// static const uint32_t ST_HIGH = 480;
// static const uint32_t ST_WEIGHT = 480;
// lv_style_t style_btn;

// void lvgl_runing_app(void)
// {
//     lv_style_init(&style_btn);
//     lv_style_set_bg_color(&style_btn, lv_color_hex(0XDF0000));     /* 设置背景 */
//     lv_style_set_bg_opa(&style_btn, LV_OPA_80);                    /* 设置背景透明度 */
//     lv_style_set_border_width(&style_btn, 1);                      /* 设置边框的宽度 */
//     lv_style_set_border_color(&style_btn, lv_color_hex(0XDF0000)); /* 设置边框的颜色 */

//     lv_obj_t *Obj1 = lv_obj_create(lv_scr_act());
//     lv_obj_set_size(Obj1, ST_WEIGHT / 2.8, ST_HIGH / 6);
//     lv_obj_align(Obj1, LV_ALIGN_TOP_LEFT, ST_WEIGHT / 48, ST_HIGH / 48);
//     lv_obj_add_style(Obj1, &style_btn, LV_STATE_DEFAULT); /* 设置对象 1 的样式 */

//     lv_obj_t *Obj2 = lv_obj_create(lv_scr_act());
//     lv_obj_set_size(Obj2, ST_WEIGHT / 6, ST_HIGH / 2.8);
//     lv_obj_align(Obj2, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_add_style(Obj2, &style_btn, LV_STATE_PRESSED); /* 设置对象 1 的样式 */
// }

static lv_obj_t *bar;
static lv_obj_t *obj1;
static lv_obj_t *obj2;
static lv_obj_t *label;

uint32_t value = 4;

static void button_event_cb(lv_event_t *event)
{
    lv_obj_t *target = lv_event_get_target(event);   /*获取触发部件类型*/
    lv_event_code_t code = lv_event_get_code(event); /*获取触发事件类型*/

    if (target == obj1)
    {
        value += 1;
        lv_bar_set_value(bar, value, LV_ANIM_ON);
        set_light(value);
    }
    else if (target == obj2)
    {
        value -= 1;
        lv_bar_set_value(bar, value, LV_ANIM_ON);
        set_light(value);
    }
    loginfo("value = %d\r\n",value);
}

void lvgl_runing_app(void)
{

    obj1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(obj1, LV_PCT(10), LV_PCT(10));
    lv_obj_align(obj1, LV_ALIGN_LEFT_MID, 20, -40);
    lv_obj_t *label = lv_label_create(obj1);
    lv_label_set_text_fmt(label, "+");
    lv_obj_center(label);
    lv_obj_add_event_cb(obj1, button_event_cb, LV_EVENT_CLICKED, NULL);

    obj2 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(obj2, LV_PCT(10), LV_PCT(10));
    lv_obj_align(obj2, LV_ALIGN_LEFT_MID, 20, 40);
    label = lv_label_create(obj2);
    lv_label_set_text_fmt(label, "-");
    lv_obj_center(label);
    lv_obj_add_event_cb(obj2, button_event_cb, LV_EVENT_CLICKED, NULL);

    bar = lv_bar_create(lv_scr_act());
    lv_obj_set_size(bar, 10, 300);
    lv_obj_align(bar, LV_ALIGN_RIGHT_MID, -30, 0);

    lv_bar_set_range(bar, 1, 10);
    lv_bar_set_start_value(bar, 4, LV_ANIM_ON);

    lv_obj_set_style_anim_time(bar, 100, LV_STATE_DEFAULT);

    lv_bar_set_value(bar, value, LV_ANIM_ON);
}