#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"
#include "p_gpttim.h"
#include "p_i2c_sht20.h"
#include "spiffs.h"
#include "sd_spi_fat.h"
#include "st7789v2_spi.h"

void ws2812_spi(void *arg)
{
    st7789_init();
    while (1)
    {
        uint16_t color = 0xffff;
    
        for (int32_t x = 280; x >0; x -= 4)
        {
            color = (rand() % (65535 - 10000) + 10000);
            lcd_fill(0, 0, x, x, &color);
         
        }

        color = 0xffff;
        lcd_fill(0, 0, 240, 280, &color);
        vTaskDelay(100 / portTICK);
    }
}

void oteee(void *arg)
{
    while (1)
    {
        ESP_LOGI("5", "oteee");
        vTaskDelay(100 / portTICK);
    }
}
void app_main()
{
    led_init();
    xTaskCreate(ws2812_spi, "ws2812_spi", 1024 * 10, NULL, 5, NULL);
    xTaskCreate(oteee, "null", 1024 * 10, NULL, 5, NULL);
}
