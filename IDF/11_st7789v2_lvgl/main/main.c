#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_idf_version.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_demos.h"
#include "st7789v2_spi.h"

lv_obj_t *obj = NULL;
lv_obj_t *btn = NULL;
void lvgl_task(void *arg)
{
    while (1)
    {
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x000f), LV_STATE_DEFAULT);
        vTaskDelay(30);
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x00f0), LV_STATE_DEFAULT);
        vTaskDelay(30);
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x0f00), LV_STATE_DEFAULT);
        vTaskDelay(30);
        lv_obj_set_style_bg_color(obj, lv_color_hex(0xf000), LV_STATE_DEFAULT);
        vTaskDelay(30);
    }
}
void app_main(void)
{
    lv_init();

    lv_port_disp_init();
    obj = lv_obj_create(lv_scr_act());
    btn = lv_btn_create(lv_scr_act());
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(obj, 120, 120);
    // lv_obj_set_style_bg_color(obj, lv_color_hex(0x00ff), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000f), LV_STATE_DEFAULT);

    xTaskCreate(lvgl_task, "lvgl", 1024 * 4, NULL, 3, NULL);
    while (1)
    {

        lv_task_handler();
        lv_tick_inc(10);
        vTaskDelay(1);
    }
}
