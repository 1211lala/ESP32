/**
 * 实验目的: 了解FreeRTOS的Queue
 * 实验现象: 通过串口发送信息，esp32会返回您输入的信息
 */

#include <Arduino.h>
#include "pincfg.h"

void os_task_fun_1(void *arg);
void os_task_fun_2(void *arg);
void os_task_fun_3(void *arg);

TaskHandle_t os_handle_1 = NULL;
TaskHandle_t os_handle_2 = NULL;
TaskHandle_t os_handle_3 = NULL;

QueueHandle_t msgquene = NULL;

void setup()
{
    led_init();
    button_init();
    Serial.begin(115200);
    /* 创建 msgqueue */
    msgquene = xQueueCreate(5, 10);
    if (msgquene == 0)
    {
        logoinfo("msgqueue创造失败\r\n");
    }
    xTaskCreatePinnedToCore(os_task_fun_1, "os_task_fun_1", 1024 * 4, NULL, 3, &os_handle_1, 1);
    xTaskCreatePinnedToCore(os_task_fun_2, "os_task_fun_2", 1024 * 4, NULL, 2, &os_handle_2, 1);
    xTaskCreatePinnedToCore(os_task_fun_3, "os_task_fun_3", 1024 * 4, NULL, 3, &os_handle_3, 1);
    vTaskDelete(NULL);
}

/********************************************************************************************
 * 任务功能: 读取 msgqueue的数据, 无数据则一直阻塞
 *********************************************************************************************/
void os_task_fun_3(void *args)
{
    while (1)
    {
        char buf[20];
        xQueueReceive(msgquene, buf, portMAX_DELAY);
        logoinfo("%s\r\n", buf);
    }
}

/********************************************************************************************
 * 任务功能: 读取串口的数据，通过 msgqueue 发送
 *********************************************************************************************/
void os_task_fun_2(void *args)
{
    while (1)
    {
        char buf[100];
        int num = Serial.available();
        if (num)
        {
            Serial.readBytes(buf, num);
            xQueueSend(msgquene, buf, portMAX_DELAY);
        }
        vTaskDelay(50);
    }
}

/********************************************************************************************
 * 任务功能: 为OneButton提供tick时钟
 *********************************************************************************************/
void os_task_fun_1(void *args)
{
    while (1)
    {
        button.tick();
        digitalWrite(LED, !digitalRead(KEY));
        vTaskDelay(20);
    }
}
void loop()
{
}
