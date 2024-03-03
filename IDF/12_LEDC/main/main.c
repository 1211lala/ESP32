#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"
#include "p_gpttim.h"
#include "p_i2c_sht20.h"
#include "p_wifi_sta.h"
#include "spiffs.h"
#include "driver/ledc.h"

/*
 * LEDC Chan to Group/Channel/Timer Mapping
 ** ledc: 0  => Group: 0, Channel: 0, Timer: 0
 ** ledc: 1  => Group: 0, Channel: 1, Timer: 0
 ** ledc: 2  => Group: 0, Channel: 2, Timer: 1
 ** ledc: 3  => Group: 0, Channel: 3, Timer: 1
 ** ledc: 4  => Group: 0, Channel: 4, Timer: 2
 ** ledc: 5  => Group: 0, Channel: 5, Timer: 2
 ** ledc: 6  => Group: 0, Channel: 6, Timer: 3
 ** ledc: 7  => Group: 0, Channel: 7, Timer: 3
 ** ledc: 8  => Group: 1, Channel: 0, Timer: 0
 ** ledc: 9  => Group: 1, Channel: 1, Timer: 0
 ** ledc: 10 => Group: 1, Channel: 2, Timer: 1
 ** ledc: 11 => Group: 1, Channel: 3, Timer: 1
 ** ledc: 12 => Group: 1, Channel: 4, Timer: 2
 ** ledc: 13 => Group: 1, Channel: 5, Timer: 2
 ** ledc: 14 => Group: 1, Channel: 6, Timer: 3
 ** ledc: 15 => Group: 1, Channel: 7, Timer: 3
 */

/*  PWM的通道，共16个(0-15)，分为高低速两组，
    高速通道(0-7): 80MHz时钟，低速通道(8-15): 1MHz时钟
    0-15都可以设置，只要不重复即可，参考上面的列表
    如果有定时器的使用，千万要避开!!! */

TaskHandle_t ledc_handle;
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_GPIO GPIO_NUM_2

void task_ledc(void *arg)
{
    while (1)
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 1000);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(1000 / portTICK);
        ledc_set_duty(LEDC_SPEED_MODE_MAX, LEDC_CHANNEL_0, 1000);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(1000 / portTICK);
    }
}

void app_main()
{
    spiffs_mount();

    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode = LEDC_MODE;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.timer_num = LEDC_TIMER;
    ledc_timer.freq_hz = 5000;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel =
        {
            .gpio_num = LEDC_GPIO,
            .channel = LEDC_CHANNEL,
            .speed_mode = LEDC_MODE,
            .duty = 0,
            .hpoint = 0,
            .timer_sel = LEDC_TIMER};
    ledc_channel_config(&ledc_channel);
    xTaskCreate(task_ledc, "task_ledc", 1024 * 4, NULL, 5, &ledc_handle);
}
