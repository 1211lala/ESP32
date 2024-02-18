#include "ledPin.h"

void led_init(void)
{
    ESP_LOGI(TAG, "%d引脚被配置为LED", ledPin);
    gpio_reset_pin(ledPin);
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);
}