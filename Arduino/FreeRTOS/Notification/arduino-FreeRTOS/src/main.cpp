#include <Arduino.h>
#include "soc/rtc_wdt.h"

#define LED 2
#define KEY 0

#define led_bit 1 << 0
#define uart_bit 1 << 1

TaskHandle_t task1_handle = NULL;
TaskHandle_t task2_handle = NULL;
TaskHandle_t task3_handle = NULL;

void start_task(void);
void task1_led(void *pt);
void task2_key(void *pt);
void task3_uart(void *pt);

uint8_t get_key_value(uint8_t waittime);

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    pinMode(KEY, INPUT_PULLUP);

    rtc_wdt_protect_off();
    rtc_wdt_enable();
    rtc_wdt_set_time(RTC_WDT_STAGE0, 5000);

    start_task();
    vTaskSuspend(NULL);
}

void loop()
{
}

void start_task(void)
{
    xTaskCreate(task1_led, "task1_led", 1024 * 2, NULL, 2, &task1_handle);
    xTaskCreate(task2_key, "task2_key", 1024 * 2, NULL, 3, &task2_handle);
    xTaskCreate(task3_uart, "task3_uart", 1024 * 2, NULL, 4, &task3_handle);
}

void task1_led(void *pt)
{
    uint32_t notification_value = 0;
    BaseType_t xResult;
    while (1)
    {
        xResult = xTaskNotifyWait(0x00, 0x00, &notification_value, portMAX_DELAY);
        if (xResult == pdTRUE)
        {
            Serial.printf("task1 rec_notification = %d \r\n", notification_value);
        }
    }
}
void task3_uart(void *pt)
{
    uint32_t notification_value = 0;
    BaseType_t xResult;
    while (1)
    {
        xResult = xTaskNotifyWait(0x00, 0x00, &notification_value, portMAX_DELAY);
        if (xResult == pdTRUE)
        {
            Serial.printf("task3 rec_notification = %d \r\n", notification_value);
        }
        // Serial.printf("ESP.getHeapSize() = %dK\r\n", ESP.getHeapSize() / 1024);
        // Serial.printf("ESP.getFreeHeap() = %dK\r\n", ESP.getFreeHeap() / 1024);
        // Serial.printf("ESP.getFlashChipSize() = %dK\r\n", ESP.getFlashChipSize() / 1024);
        // Serial.printf("ESP.getFlashChipSpeed() = %dMhz\r\n", ESP.getFlashChipSpeed() / 1000000);
        Serial.printf("task1 min free = %d word\r\n", uxTaskGetStackHighWaterMark(task1_handle));
        Serial.printf("task2 min free = %d word\r\n", uxTaskGetStackHighWaterMark(task2_handle));
        Serial.printf("task3 min free = %d word\r\n", uxTaskGetStackHighWaterMark(task3_handle));
        vTaskDelay(2000);
    }
}
void task2_key(void *pt)
{
    EventBits_t uxBits;
    uint8_t flag = 0;
    while (1)
    {
        /* 喂狗 */
        rtc_wdt_feed();

        if (get_key_value(30) == 1)
        {

            if (flag == 0)
            {
                flag = 1;
                xTaskNotify(task1_handle, 0, eNoAction);
                // xTaskNotify(task1_handle, 0, eIncrement);
                // xTaskNotify(task1_handle, 9, eSetValueWithOverwrite);
            }
            else
            {
                xTaskNotify(task3_handle, 0, eNoAction);
                // xTaskNotify(task3_handle, 0, eIncrement);
                // xTaskNotify(task3_handle, 4546, eSetValueWithOverwrite);
                flag = 0;
            }
            Serial.printf("%d: task2_key\r\n", millis());
        }

        vTaskDelay(5);
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
