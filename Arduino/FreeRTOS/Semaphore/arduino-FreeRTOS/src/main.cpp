#include <Arduino.h>
#include "soc/rtc_wdt.h"

#define LED 2
#define KEY 0

TaskHandle_t task1_handle;
TaskHandle_t task2_handle;
TaskHandle_t task3_handle;
TaskHandle_t task4_handle;

SemaphoreHandle_t xMutex;

TimerHandle_t xTim;

QueueHandle_t xMsgQueue;
QueueHandle_t xSemaphor;

void task1_mutex(void *pt);
void task2_key(void *pt);
void task3_uart(void *pt);
void start_task(void);
void task4_queue(void *pt);

void Tim_callback(TimerHandle_t xTimer);
uint8_t get_key_value(uint8_t waittime);

uint32_t count = 0;

typedef struct
{
    int year;
    char name[10];
    int wight;
} QueueStruct;

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    pinMode(KEY, INPUT_PULLUP);

    rtc_wdt_protect_off();
    rtc_wdt_enable();
    rtc_wdt_set_time(RTC_WDT_STAGE0, 5000);

    start_task();
}

void loop()
{
    vTaskSuspend(NULL);
    Serial.printf("%d: loop\r\n", millis());
    vTaskDelay(1000);
}

void start_task(void)
{
    /**/
    xMutex = xSemaphoreCreateMutex();
    /* 二值量 */
    xSemaphor = xSemaphoreCreateBinary();
    /* 队列 */
    xMsgQueue = xQueueCreate(8, sizeof(QueueStruct));

    xTim = xTimerCreate("Tim", 50, 50, (void *)1, Tim_callback);
    xTaskCreate(task1_mutex, "task1_mutex", 1024 * 2, NULL, 2, &task1_handle);
    xTaskCreate(task2_key, "task2_key", 1024 * 2, NULL, 3, &task2_handle);
    xTaskCreate(task3_uart, "task3_uart", 1024 * 2, NULL, 4, &task3_handle);
    xTaskCreate(task4_queue, "task4_queue", 1024 * 2, NULL, 5, &task4_handle);
}

void Tim_callback(TimerHandle_t xTimer)
{
    /* 发送Queue信息 */
    QueueStruct queuestruct;

    strcpy(queuestruct.name, "liuao");
    queuestruct.wight = random(100, 200);
    queuestruct.year = random(2014, 2050);
    xQueueSend(xMsgQueue, &queuestruct, 100);
}

void task1_mutex(void *pt)
{
    while (1)
    {
        if (xSemaphoreTake(xSemaphor, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(LED, !digitalRead(LED));
        }
    }
}

void task2_key(void *pt)
{
    uint8_t flag = 0;
    while (1)
    {
        if (get_key_value(30) == 1)
        {
            xSemaphoreGive(xSemaphor);
            if (flag == 0)
            {
                flag = 1;
                xTimerStart(xTim, 100);
            }
            else
            {
                flag = 0;
                xTimerStop(xTim, 100);
            }
            Serial.printf("%d: task2_key\r\n", millis());
        }
        vTaskDelay(5);
    }
}

void task3_uart(void *pt)
{

    while (1)
    {
        /* 喂狗 */
        rtc_wdt_feed();
        Serial.printf("ESP32 Freq =%dMhz\r\n", getCpuFrequencyMhz());
        Serial.printf("ESP.getHeapSize() = %dK\r\n", ESP.getHeapSize() / 1024);
        Serial.printf("ESP.getFreeHeap() = %dK\r\n", ESP.getFreeHeap() / 1024);
        Serial.printf("ESP.getFlashChipSize() = %dK\r\n", ESP.getFlashChipSize() / 1024);
        Serial.printf("ESP.getFlashChipSpeed() = %dMhz\r\n", ESP.getFlashChipSpeed() / 1000000);
        Serial.printf("ESP.getFlashChipMode() = %d\r\n", ESP.getFlashChipMode());
        
        // Serial.printf("task1 min free = %d word\r\n", uxTaskGetStackHighWaterMark(task1_handle));
        // Serial.printf("task2 min free = %d word\r\n", uxTaskGetStackHighWaterMark(task2_handle));
        // Serial.printf("task3 min free = %d word\r\n", uxTaskGetStackHighWaterMark(task3_handle));
        // Serial.printf("task4 min free = %d word\r\n\r\n", uxTaskGetStackHighWaterMark(task4_handle));
        vTaskDelay(2000);
    }
}

void task4_queue(void *pt)
{
    while (1)
    {
        QueueStruct queuestruct;
        if (xQueueReceive(xMsgQueue, &queuestruct, portMAX_DELAY) == pdTRUE)
        {
            Serial.printf("%d queue rec: name = %s\r\n", millis(), queuestruct.name);
            Serial.printf("%d queue rec: wight = %d\r\n", millis(), queuestruct.wight);
            Serial.printf("%d queue rec: year = %d\r\n\r\n", millis(), queuestruct.year);
        }
    }
}

uint8_t get_key_value(uint8_t waittime)
{
    static uint64_t tick = 0;
    static uint8_t flag = 0;

    uint8_t key_status = 0;

    key_status = digitalRead(KEY);
    if (flag == 0 && key_status == 0)
    {
        tick = millis();
        flag = 1;
    }
    if (flag == 1 && ((millis() - tick) > waittime))
    {
        if (key_status == 0)
        {
            flag = 2;
            return 1;
        }
    }
    if (key_status == 1)
    {
        flag = 0;
    }
    return 0;
}
