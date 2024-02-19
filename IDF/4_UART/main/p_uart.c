#include "p_uart.h"

void uart_init(void)
{
    uart_config_t uart_config = {};

    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;

    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 16, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    esp_err_t ret = uart_driver_install(UART_NUM_1, 256, 0, 0, NULL, 0);
    if (ret != ESP_OK)
    {
        printf("UART driver install failed\n");
        return;
    }
}