#ifndef _ST789V2_LVGL_H_
#define _ST789V2_LVGL_H_

#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"
#include "lv_demos.h"

/* 设置LCD尺寸 */
#define EXAMPLE_LCD_H_RES (240)
#define EXAMPLE_LCD_V_RES (280)
/* LCD引脚 */
#define EXAMPLE_LCD_GPIO_SCLK (18)
#define EXAMPLE_LCD_GPIO_MOSI (17)
#define EXAMPLE_LCD_GPIO_RST (15)
#define EXAMPLE_LCD_GPIO_DC (19)
#define EXAMPLE_LCD_GPIO_CS (16)
#define EXAMPLE_LCD_GPIO_BL (21)
/* 设置接口参数 */
#define EXAMPLE_LCD_SPI_NUM (SPI3_HOST)
#define EXAMPLE_LCD_PIXEL_CLK_HZ (80 * 1000 * 1000)
#define EXAMPLE_LCD_CMD_BITS (8)
#define EXAMPLE_LCD_PARAM_BITS (8)
#define EXAMPLE_LCD_COLOR_SPACE (ESP_LCD_COLOR_SPACE_RGB)
#define EXAMPLE_LCD_BITS_PER_PIXEL (16)
#define EXAMPLE_LCD_DRAW_BUFF_DOUBLE (1)
#define EXAMPLE_LCD_DRAW_BUFF_HEIGHT (50)
#define EXAMPLE_LCD_BL_ON_LEVEL (1)
/* 按键IO */
#define PREV_BTN 35
#define NEXT_BTN 34
#define ENTER_BTN 36
#define ACTIVE_LEVEL 0
esp_err_t lcd_init(void);
esp_err_t lvgl_init(void);
void lvgl_indev_button_init(void);
#endif