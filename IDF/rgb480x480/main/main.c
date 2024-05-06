#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_io.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "esp_heap_caps.h"

#include "lv_demos.h"
#include "lvgl.h"
#include "lvgl_interface.h"

void lvgl_hander(void *arg);
void system_info(void *arg);

static const char *TAG = "main";

void app_main(void)
{

    lvgl_disp_init();
    lvgl_indev_init();
    lv_demo_benchmark();

    xTaskCreatePinnedToCore(lvgl_hander, "lvgl_hander", 1024 * 4, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(system_info, "system_info", 1024 * 4, NULL, 3, NULL, 1);
}
void system_info(void *arg)
{
    while (1)
    {
        printf("psram free: %d kb\r\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        printf("sram free: %d kb\r\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void lvgl_hander(void *arg)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_timer_handler();
    }
}