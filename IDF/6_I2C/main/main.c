#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO 22      // GPIO22
#define I2C_MASTER_SDA_IO 21      // GPIO21
#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number
#define I2C_MASTER_FREQ_HZ 100000 // I2C master clock frequency
#define SHT20_ADDR 0x40           // SHT20 I2C address

int portTICK = 10;
char *TAG = "SHT20";

esp_err_t sht20_read(uint8_t *data_h, uint8_t *data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0xf3, true); // Command to read temperature
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK);
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C master write failed: %d", err);
        return err;
    }
    vTaskDelay(100 / portTICK); // Wait for measurement
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, data_h, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, data_l, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK);
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C master read failed: %d", err);
        return err;
    }
    return ESP_OK;
}

void sht20_task(void *pvParameters)
{
    uint8_t data_h, data_l;
    float temperature, humidity;

    while (1)
    {
        esp_err_t err = sht20_read(&data_h, &data_l);
        if (err == ESP_OK)
        {
            uint16_t data = 0;
            data = (data_h << 8) | data_l;
            data = (175.72 * data / 65536 - 46.85) * 10; // 温度计算公式
            ESP_LOGI(TAG, "Temperature: %f°C", data/10.0);
        }
        vTaskDelay(1000 / portTICK);
    }
}

void app_main()
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    xTaskCreate(sht20_task, "sht20_task", 2048, NULL, 5, NULL);
}
