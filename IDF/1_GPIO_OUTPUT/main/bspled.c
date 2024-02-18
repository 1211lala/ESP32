#include "bspled.h"

static const char *TAG = "BSPLED";

void bsp_led_init(void)
{
    ESP_LOGI(TAG, "%d被设置为LED引脚", ledPin);
    gpio_reset_pin(ledPin);
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
}

void bsp_led_blink(void)
{
    static uint8_t led_state = 0;

    led_state = !led_state;
    gpio_set_level(ledPin, led_state);

    ESP_LOGI(TAG, "led state: %s", led_state == true ? "ON" : "OFF");
}