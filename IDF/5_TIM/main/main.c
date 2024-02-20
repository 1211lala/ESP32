/**
 * 官方wiki     https://docs.espressif.com/projects/esp-idf/zh_CN/release-v5.1/esp32/api-reference/peripherals/gpio.html
 * 更改工程名,主文件名      https://blog.csdn.net/mark_md/article/details/113884641
 */

#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"
#include "p_tim.h"

TaskHandle_t task_uart_handle = NULL;

bool timer_callback(void *arg)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    uint64_t value = timer_group_get_counter_value_in_isr(0, 0);
    xQueueSendFromISR(tim_queue, &value, &pxHigherPriorityTaskWoken);
    return pxHigherPriorityTaskWoken;
}

void task_tim0(void *arg)
{
    while (1)
    {
        uint64_t value = 0;
        if (xQueueReceive(tim_queue, &value, portMAX_DELAY))
        {
            ESP_LOGI("LOG", "tim0 value : %llu", value);
        }
    }
}

void app_main(void)
{

    tim0_init(timer_callback, NULL, 1000000);
    tim_queue = xQueueCreate(10, sizeof(uint64_t));
    if(tim_queue == NULL)
    {
        ESP_LOGI("LOG", "队列初始化失败");
    }
    xTaskCreatePinnedToCore(task_tim0, "task_tim0", 1024 * 4, NULL, 10, &task_uart_handle, 1);
    timer_start(0, 0);
    vTaskDelete(NULL);
}
