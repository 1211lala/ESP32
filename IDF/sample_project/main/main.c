#include "ledPin.h"

TaskFunction_t  task_led_blink = NULL;



void led_blink(void *arg)
{
    uint8_t led_state = 0;
    while (1)
    {
        led_state = !led_state;
        gpio_set_level(ledPin, led_state);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    led_init();
    // xTaskCreatePinnedToCore(led_blink, "led_blink", 1024*4, NULL, 3, &task_led_blink, 1);
    vTaskDelete(NULL);
}
