#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"
#include "p_gpttim.h"
#include "p_i2c_sht20.h"

void sht20_task(void *pvParameters)
{
    sht20_init(100000);
    float temp = 0;
    while (1)
    {
        sht20_get_temp(&temp);
        ESP_LOGI("TEMP", "%.2fC", temp);
        vTaskDelay(1000 / portTICK);
    }
}

void app_main()
{

    xTaskCreate(sht20_task, "sht20_task", 2048, NULL, 5, NULL);
}
