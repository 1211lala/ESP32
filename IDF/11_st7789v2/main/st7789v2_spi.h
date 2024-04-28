#ifndef _ST7789V2_SPI_H_
#define _ST7789V2_SPI_H_


#include "common.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "dirent.h"


#define buffSize 26880

#define CMD 0
#define DATA 1

#define st7789v2_res_on() gpio_set_level(lcd.resPin, false)
#define st7789v2_res_off() gpio_set_level(lcd.resPin, true)

#define st7789v2_dc_cmd() gpio_set_level(lcd.dcPin, false)
#define st7789v2_dc_data() gpio_set_level(lcd.dcPin, true)

#define st7789v2_cs_on() gpio_set_level(lcd.resPin, false)
#define st7789v2_cs_off() gpio_set_level(lcd.resPin, true)

#define st7789v2_blk_on() gpio_set_level(lcd.blkPin, true)
#define st7789v2_blk_off() gpio_set_level(lcd.blkPin, false)


struct ST7789V2
{
    int8_t sckPin;
    int8_t sdaPin;
    int8_t resPin;
    int8_t dcPin;
    int8_t csPin;
    int8_t blkPin;
    uint32_t freq;
    spi_host_device_t spi;
    uint16_t width;
    uint16_t heigh;
    uint8_t horizonta;
    uint32_t spiSize;
};


extern struct ST7789V2 lcd;
void st7789_init(void);
void lcd_send_cmd(const uint8_t data, int16_t len);
void lcd_send_data(const uint8_t data, int16_t len);
void lcd_send_data16(const uint16_t data, int16_t len);
void lcd_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t *color);
#endif

