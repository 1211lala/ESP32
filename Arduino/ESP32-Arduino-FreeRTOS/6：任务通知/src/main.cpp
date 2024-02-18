/**
 * 实验目的: 了解FreeRTOS的任务通知
 * 实验现象:
 */

#include <Arduino.h>
#include "pincfg.h"
#include "esp_log.h"
#include "OneButton.h"

void os_task_fun_1(void *arg);
void os_task_fun_2(void *arg);
void os_task_fun_3(void *arg);

TaskHandle_t os_handle_1 = NULL;
TaskHandle_t os_handle_2 = NULL;
TaskHandle_t os_handle_3 = NULL;

void setup()
{
    led_init();
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
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == pdTRUE)
        {
            logoinfo("任务通知: os_task_fun_3 !!!\r\n");
        }
    }
}

/********************************************************************************************
 * 任务功能:
 *********************************************************************************************/
void os_task_fun_2(void *args)
{
    uint32_t value = 0;
    uint32_t e_value = 0;
    while (1)
    {
        /* 当作事件组使用 */
        xTaskNotifyWait(0x00000000, 0xffffffff, &value, portMAX_DELAY);
        logoinfo("value = %d\r\n", value);
        if (value & (1 << 3))
        {
            e_value |= (1 << 3);
            logoinfo("任务通知: 1 << 3\r\n");
        }
        if (value & (1 << 4))
        {
            e_value |= (1 << 4);
            logoinfo("任务通知: 1 << 4\r\n");
            
        }
        if (e_value == ((1 << 3) | (1 << 4)))
        {
            e_value = 0;
            logoinfo("任务通知: 1 << 3   |   1 << 4\r\n");
        }
    }
}
/********************************************************************************************
 * 任务功能:
 *********************************************************************************************/
OneButton button(KEY, true);

void button_attachClick(void)
{
    xTaskNotify(os_handle_2, (1 << 3), eSetBits);
}
void button_attachDoubleClick(void)
{
    xTaskNotify(os_handle_2, (1 << 4), eSetBits);
}

void os_task_fun_1(void *args)
{
    button.attachClick(button_attachClick);
    button.attachDoubleClick(button_attachDoubleClick);
    button.setClickMs(300);
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
