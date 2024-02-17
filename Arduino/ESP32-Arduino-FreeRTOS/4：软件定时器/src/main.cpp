/**
 * 实验目的: 了解FreeRTOS的软件定时器
 * 实验现象: 软件定时器每触发一次，串口就输出一次。
 */

#include <Arduino.h>
#include "pincfg.h"
#include "esp_log.h"

void os_task_fun_1(void *arg);
void os_softtime(TimerHandle_t pxTimer);

TaskHandle_t os_handle_1 = NULL;
TimerHandle_t  os_tim = NULL;

int count = 0;

void setup()
{
    led_init();
    xTaskCreatePinnedToCore(os_task_fun_1, "os_task_fun_1", 1024 * 4, NULL, 3, &os_handle_1, 1);

    os_tim = xTimerCreate("os_softtime", 1000, pdTRUE, (void*)1, os_softtime);
    if(os_tim == NULL)
    {
        logoinfo("软件定时器创建失败\r\n");
    }
    if(xTimerStart(os_tim, 1000) != pdPASS)
    {
        logoinfo("软件定时器开启失败\r\n");
    }
    vTaskDelete(NULL);
}

/********************************************************************************************
 * 函数功能: 定时器回调函数
 ********************************************************************************************/
static const char* TAG = "MyModule";
void os_softtime(TimerHandle_t pxTimer)
{
    /**
     * 不能使用printf()
     * 1: 是不可重入函数
     * 2: 会发生阻塞
     * 如果使用printf()会不断重启
    */
    ESP_LOGI(TAG, "软件定时器触发");
}

/********************************************************************************************
 * 任务功能: 
 *********************************************************************************************/
void os_task_fun_1(void *args)
{
    while (1)
    {
        
        digitalWrite(LED, !digitalRead(KEY));
        vTaskDelay(500);
    }
}
void loop()
{

}
