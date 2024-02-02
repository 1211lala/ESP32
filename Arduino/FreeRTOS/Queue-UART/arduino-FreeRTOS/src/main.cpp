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

void task1_get_uart_info(void *pt);
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

    vTaskSuspendAll();
    // Serial.printf("%d 关闭任务调度\r\n",millis());
    start_task();
    // Serial.printf("%d 开启任务调度\r\n",millis());
    xTaskResumeAll();
}

void loop()
{
    vTaskSuspend(NULL);
    Serial.printf("%d: loop\r\n", millis());
    vTaskDelay(1000);
}

void start_task(void)
{

    xMutex = xSemaphoreCreateMutex();

    xMsgQueue = xQueueCreate(8, 20);

    xTaskCreate(task1_get_uart_info, "task1_get_uart_info", 1024 * 2, NULL, 1, &task1_handle);
    xTaskCreate(task2_key, "task2_key", 1024 * 2, NULL, 2, &task2_handle);
    xTaskCreate(task3_uart, "task3_uart", 1024 * 2, NULL, 3, &task3_handle);
    xTaskCreate(task4_queue, "task4_queue", 1024 * 2, NULL, 4, &task4_handle);
}

void task1_get_uart_info(void *pt)
{
    char rec_buf[20];
    while (1)
    {
        uint16_t num = Serial.available();
        if (num)
        {
            Serial.readBytes(rec_buf, num);
            rec_buf[num] = '\0';
            xQueueSend(xMsgQueue, rec_buf, 100);
        }
        vTaskDelay(20);
    }
}

void task2_key(void *pt)
{
    uint8_t flag = 0;
    while (1)
    {
        if (get_key_value(30) == 1)
        {
            if (flag == 0)
            {
                flag = 1;
                digitalWrite(LED, !digitalRead(LED));
            }
            else
            {
                flag = 0;
                digitalWrite(LED, !digitalRead(LED));
            }
            Serial.printf("%d: task2_key\r\n", millis());
        }
        vTaskDelay(5);
    }
}

void task4_queue(void *pt)
{
    char buf[20];
    while (1)
    {
        if (xQueueReceive(xMsgQueue, buf, portMAX_DELAY) == pdTRUE)
        {
            Serial.printf("queue rec: %dchars  %s\r\n", strlen(buf), buf);
            for (uint8_t i = 0; i < strlen(buf); i++)
            {
                Serial.printf("buf[%d] = 0x%02x\r\n", i, buf[i]);
            }
            Serial.println();
        }
    }
}

void task3_uart(void *pt)
{

    while (1)
    {
        /* 喂狗 */
        rtc_wdt_feed();
        // Serial.printf("ESP.getHeapSize() = %dK\r\n", ESP.getHeapSize() / 1024);
        // Serial.printf("ESP.getFreeHeap() = %dK\r\n", ESP.getFreeHeap() / 1024);
        // Serial.printf("ESP.getFlashChipSize() = %dK\r\n", ESP.getFlashChipSize() / 1024);
        // Serial.printf("ESP.getFlashChipSpeed() = %dMhz\r\n", ESP.getFlashChipSpeed() / 1000000);
        // Serial.printf("task1 min free = %d char\r\n", uxTaskGetStackHighWaterMark(task1_handle));
        // Serial.printf("task2 min free = %d char\r\n", uxTaskGetStackHighWaterMark(task2_handle));
        // Serial.printf("task3 min free = %d char\r\n", uxTaskGetStackHighWaterMark(task3_handle));
        // Serial.printf("task4 min free = %d char\r\n", uxTaskGetStackHighWaterMark(task4_handle));

        // Serial.printf("task1 priority = %d \r\n", uxTaskPriorityGet(task1_handle));
        // Serial.printf("task2 priority = %d \r\n", uxTaskPriorityGet(task2_handle));
        // Serial.printf("task3 priority = %d \r\n", uxTaskPriorityGet(task3_handle));
        // Serial.printf("task4 priority = %d \r\n", uxTaskPriorityGet(task4_handle));

        vTaskDelay(2000);
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
