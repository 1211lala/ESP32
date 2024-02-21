#include "p_i2c_sht20.h"

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21

#define SHT20_ADDR 0x40
#define SHT20_TEMP_CMD 0XF3
#define SHT20_HUM_CMD 0XF5

const char *TAG = "SHT20";

void sht20_init(uint32_t freq)
{
    i2c_config_t config = {};
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = I2C_MASTER_SDA_IO;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_io_num = I2C_MASTER_SCL_IO;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = freq;
    i2c_param_config(I2C_NUM_0, &config);

    i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0);
}

uint8_t sht20_get_temp(float *temp)
{
    uint8_t data_h = 0;
    uint8_t data_l = 0;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0xf3, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK);
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C master write failed: %d", err);
        return err;
    }

    vTaskDelay(100 / portTICK);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &data_h, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &data_l, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK);
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C master read failed: %d", err);
        return err;
    }
    *temp = (175.72 * ((data_h << 8) | data_l) / 65536 - 46.85); // 温度计算公式
    return ESP_OK;
}