// https://blog.csdn.net/mark_md/article/details/113884641

#include "common.h"
#include "bspled.h"

TaskHandle_t task_led_handle = NULL;

void task_led(void *arg)
{
    bsp_led_init();
    while (1)
    {
        bsp_led_blink();
        vTaskDelay(1000 / portTICK);
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(task_led, "task_led", 1024 * 3, NULL, 2, &task_led_handle, 1);
    vTaskDelete(NULL);
}
