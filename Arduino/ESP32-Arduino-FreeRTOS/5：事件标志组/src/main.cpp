/**
 * 实验目的: 了解FreeRTOS的软件定时器
 * 实验现象: 软件定时器每触发一次，串口就输出一次。
 */

#include <Arduino.h>
#include "pincfg.h"
#include "esp_log.h"

void os_task_fun_1(void *arg);
void os_task_fun_2(void *arg);
void os_task_fun_3(void *arg);

TaskHandle_t os_handle_1 = NULL;
TaskHandle_t os_handle_2 = NULL;
TaskHandle_t os_handle_3 = NULL;

EventGroupHandle_t group = NULL;

#define GROUPBIT0 1 << 0
#define GROUPBIT1 1 << 1
#define ALLBIT 0xffffff

void setup()
{
    led_init();
    group = xEventGroupCreate();
    if (group == NULL)
    {
        logoinfo("事件组初始化失败\r\n");
    }
    xTaskCreatePinnedToCore(os_task_fun_1, "os_task_fun_1", 1024 * 4, NULL, 3, &os_handle_1, 1);
    xTaskCreatePinnedToCore(os_task_fun_2, "os_task_fun_2", 1024 * 4, NULL, 3, &os_handle_2, 1);
    xTaskCreatePinnedToCore(os_task_fun_3, "os_task_fun_3", 1024 * 4, NULL, 3, &os_handle_3, 1);

    vTaskDelete(NULL);
}

/********************************************************************************************
 * 任务功能:
 *********************************************************************************************/
void os_task_fun_3(void *args)
{
    while (1)
    {
        EventBits_t getbits = xEventGroupWaitBits(group, GROUPBIT0 | GROUPBIT1, pdTRUE, pdTRUE, portMAX_DELAY);
        if ((getbits & GROUPBIT0 | GROUPBIT1) == GROUPBIT0 | GROUPBIT1)
        {
            logoinfo("事件组获取成功\r\n");
        }
    }
}

/********************************************************************************************
 * 任务功能:
 *********************************************************************************************/
void os_task_fun_2(void *args)
{
    while (1)
    {

        EventBits_t getbits = xEventGroupSetBits(group, GROUPBIT0);
        if (getbits & (GROUPBIT0))
        {
            logoinfo("事件组GROUPBIT0设置成功\r\n");
        }
        else
        {
        }

        vTaskDelay(100);
        getbits = xEventGroupSetBits(group, GROUPBIT0);
        if (getbits & (GROUPBIT0))
        {
            logoinfo("事件组GROUPBIT0设置成功\r\n");
        }
        else
        {
        }


        vTaskDelay(100);
        getbits = xEventGroupSetBits(group, GROUPBIT1);
        if (getbits & (GROUPBIT1))
        {
            logoinfo("事件组GROUPBIT1设置成功\r\n");
        }
        else
        {
        }
        vTaskDelay(400);
    }
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
