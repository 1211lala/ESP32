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
    uint32_t value = 0;
    while (1)
    {
        if (xTaskNotifyWait(0x00000000, 0xFFFFFFFF, &value, portMAX_DELAY) == pdTRUE)
        {
            logoinfo("任务通知: os_task_fun_3 !!!  value = %d\r\n", value);
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
        if (ulTaskNotifyTake(pdTRUE,portMAX_DELAY) == pdTRUE)
        {
            logoinfo("任务通知: os_task_fun_2 !!!\r\n");
        }
    }
}
/********************************************************************************************
 * 任务功能:
 *********************************************************************************************/
OneButton button(KEY, true);
/**
 * #define xTaskNotify( xTaskToNotify, ulValue, eAction ) \
    xTaskGenericNotify( ( xTaskToNotify ), ( tskDEFAULT_INDEX_TO_NOTIFY ), ( ulValue ), ( eAction ), NULL )
#define xTaskNotifyGive( xTaskToNotify ) \
    xTaskGenericNotify( ( xTaskToNotify ), ( tskDEFAULT_INDEX_TO_NOTIFY ), ( 0 ), eIncrement, NULL )
*/
void button_attachClick(void)
{
    // /* 当作二值信号量使用, 传入参数30无效 */
    // xTaskNotify(os_handle_2, 30, eNoAction);
    /* 当作计算信号量使用, 传入参数30无效, 每次会使任务通知的值加一 */
    xTaskNotify(os_handle_2, 0, eIncrement);
    // /* 当作事件组使用, 传入参数回和之前的参数进行或运算 */
    // xTaskNotify(os_handle_2, (1 << 3) | (1 << 0), eSetBits);
}
void button_attachDoubleClick(void)
{

}
void os_task_fun_1(void *args)
{
    button.attachClick(button_attachClick);
    button.attachDoubleClick(button_attachDoubleClick);
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
