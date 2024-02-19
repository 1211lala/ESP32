/**
 * 官方wiki     https://docs.espressif.com/projects/esp-idf/zh_CN/release-v5.1/esp32/api-reference/peripherals/gpio.html
 * 更改工程名,主文件名      https://blog.csdn.net/mark_md/article/details/113884641
 */

#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"

TaskHandle_t task_uart_handle = NULL;

void task_uart(void *arg)
{
    uint8_t *data = (uint8_t *)malloc(100);
    while (1)
    {
        int len = uart_read_bytes(UART_NUM_1, data, 100, 20 / portTICK);
        if (len)
        {
            uart_write_bytes(UART_NUM_1, data, len);
        }
    }
}

void app_main(void)
{
    uart_config_t uart_config = {};

    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;

    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 25, 26, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    esp_err_t ret = uart_driver_install(UART_NUM_1, 256, 0, 0, NULL, 0);
    if (ret != ESP_OK)
    {
        printf("UART driver install failed\n");
        return;
    }

    xTaskCreatePinnedToCore(task_uart, "task_uart", 1024 * 4, NULL, 2, &task_uart_handle, 1);
    vTaskDelete(NULL);
}
