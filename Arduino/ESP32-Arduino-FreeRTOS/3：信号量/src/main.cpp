/**
 * 实验目的: 了解FreeRTOS的互斥信号量
 * 实验现象: 串口会输出全局变量 count 的值, 且 count 的写入和读取都是通过互斥锁保证变量被本任务独属
 *          
 */

#include <Arduino.h>
#include "pincfg.h"

void os_task_fun_1(void *arg);
void os_task_fun_2(void *arg);
void os_task_fun_3(void *arg);

TaskHandle_t os_handle_1 = NULL;
TaskHandle_t os_handle_2 = NULL;
TaskHandle_t os_handle_3 = NULL;

QueueHandle_t mutex = NULL;

int count = 0;
void setup()
{
    led_init();
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL)
    {
        logoinfo("互斥信号量创造失败!!!\r\n");
    }
    xTaskCreatePinnedToCore(os_task_fun_1, "os_task_fun_1", 1024 * 4, NULL, 3, &os_handle_1, 1);
    xTaskCreatePinnedToCore(os_task_fun_2, "os_task_fun_2", 1024 * 4, NULL, 2, &os_handle_2, 1);
    xTaskCreatePinnedToCore(os_task_fun_3, "os_task_fun_3", 1024 * 4, NULL, 3, &os_handle_3, 1);
    vTaskDelete(NULL);
}

/********************************************************************************************
 * 任务功能: 通过获取 mutex 的占有权改变全局变量 count  的值
 *********************************************************************************************/
void os_task_fun_3(void *args)
{
    while (1)
    {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
        {
            count++;
            xSemaphoreGive(mutex);
        }
        else
        {
            logoinfo("信号量获取失败\r\n");
        }
        vTaskDelay(100);
    }
}

/********************************************************************************************
 * 任务功能: 通过获取 mutex 的占有权确认变量 count 是更新后的值并打印
 *********************************************************************************************/
void os_task_fun_2(void *args)
{
    while (1)
    {
        if ((xSemaphoreTake(mutex, portMAX_DELAY)) == pdTRUE)
        {
            logoinfo("count = %d\r\n", count);
            xSemaphoreGive(mutex);
        }
        else{
            /* 释放失败 */
        }
        vTaskDelay(1000);
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
