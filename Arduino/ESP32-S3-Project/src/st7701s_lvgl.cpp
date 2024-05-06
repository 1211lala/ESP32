#include "st7701s_lvgl.h"
#include "touch.h"

Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(

    18 /* DE */,
    17 /* VSYNC */,
    16 /* HSYNC */,
    21 /* PCLK */,
    11 /* R0 */,
    12 /* R1 */,
    13 /* R2 */,
    14 /* R3 */,
    0 /* R4 */,
    8 /* G0 */,
    20 /* G1 */,
    3 /* G2 */,
    46 /* G3 */,
    9 /* G4 */,
    10 /* G5 */,
    4 /* B0 */,
    5 /* B1 */,
    6 /* B2 */,
    7 /* B3 */,
    15 /* B4 */,
    1 /* hsync_polarity */,
    10 /* hsync_front_porch */,
    8 /* hsync_pulse_width */,
    50 /* hsync_back_porch */,
    1 /* vsync_polarity */,
    10 /* vsync_front_porch */,
    8 /* vsync_pulse_width */,
    20 /* vsync_back_porch */);

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

/**************************************************************************************
 * 函数功能:
 **************************************************************************************/

static uint32_t bg_color[] = {0xff0000, 0x00ff00, 0x0000ff};
static lv_obj_t *objArray[3];
static lv_obj_t *label[3];
static lv_obj_t *btn;

static void button_event_cb(lv_event_t *event)
{
    lv_obj_t *target = lv_event_get_target(event);   /*获取触发部件类型*/
    lv_event_code_t code = lv_event_get_code(event); /*获取触发事件类型*/

    if (target == btn)
    {
        if (code == LV_EVENT_SHORT_CLICKED)
        {
            lv_obj_set_pos(objArray[0], Rand(), Rand());
            lv_obj_clear_flag(objArray[0], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objArray[1], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objArray[2], LV_OBJ_FLAG_HIDDEN);
        }
        else if (code == LV_EVENT_LONG_PRESSED)
        {
            lv_obj_set_pos(objArray[1], Rand(), Rand());
            lv_obj_add_flag(objArray[0], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objArray[1], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objArray[2], LV_OBJ_FLAG_HIDDEN);
            loginfo("LV_EVENT_LONG_PRESSED\r\n");
        }
    }
}

void lvgl_runing_app(void)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        objArray[i] = lv_obj_create(lv_scr_act());
        lv_obj_set_size(objArray[i], LV_PCT(30), LV_PCT(30));
        lv_obj_set_style_bg_color(objArray[i], lv_color_hex(bg_color[i]), LV_STATE_DEFAULT);
        lv_obj_align(objArray[i], LV_ALIGN_CENTER, 0, -100);

        lv_obj_add_flag(objArray[i], LV_OBJ_FLAG_HIDDEN);

        label[i] = lv_label_create(objArray[i]);
        lv_label_set_text_fmt(label[i], "OBJ%d", i);
        lv_obj_align(label[i], LV_ALIGN_CENTER, 0, 0);
    }

    btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, LV_PCT(15), LV_PCT(8));
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 200);
    lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}
