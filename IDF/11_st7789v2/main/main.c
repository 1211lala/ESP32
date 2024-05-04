
#include "st7789v2_lvgl.h"

void system_led(void *arg);

void app_main(void)
{
    ESP_ERROR_CHECK(lcd_init());
    ESP_ERROR_CHECK(lvgl_init());
    lvgl_indev_button_init();
    lv_demo_widgets();
    xTaskCreate(system_led, "led", 1024 * 3, NULL, 3, NULL);
}

void system_led(void *arg)
{
    static const uint8_t ledPin = 2;
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
    while (1)
    {
        gpio_set_level(ledPin, true);
        vTaskDelay(50);
        gpio_set_level(ledPin, false);
        vTaskDelay(50);

        ESP_LOGI("led", "time");
    }
}