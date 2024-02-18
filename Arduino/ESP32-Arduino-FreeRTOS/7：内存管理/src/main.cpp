/**
 * 实验目的: 了解FreeRTOS的任务通知
 * 实验现象:
 */

#include <Arduino.h>
#include "pincfg.h"
#include "esp_log.h"
#include "OneButton.h"

void os_task_fun_1(void *arg);

TaskHandle_t os_handle_3 = NULL;

void setup()
{
    led_init();
    xTaskCreatePinnedToCore(os_task_fun_1, "os_task_fun_1", 1024 * 4, NULL, 3, &os_handle_3, 1);

    vTaskDelete(NULL);
}

/********************************************************************************************
 * 任务功能:
 *********************************************************************************************/
void os_task_fun_1(void *args)
{
    char *buf = NULL;
    while (1)
    {
        buf = (char *)malloc(100);
        sprintf(buf, "11111111\r\n");
        logoinfo("%s", buf);
        free(buf);
        vTaskDelay(100);
    }
}

void loop()
{
}
