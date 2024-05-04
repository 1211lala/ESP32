#include "st7789v2_lvgl.h"

static const char *TAG = "st7789v2";

/* io句柄 */
static esp_lcd_panel_io_handle_t lcd_io = NULL;
/* 屏幕句柄 */
static esp_lcd_panel_handle_t lcd_panel = NULL;

static lv_disp_t *lvgl_disp = NULL;

esp_err_t lcd_init(void)
{
    esp_err_t ret = ESP_OK;

    /* lcd -> bl 初始化，设置熄灭 */
    gpio_config_t bk_gpio_config = {};
    bk_gpio_config.mode = GPIO_MODE_OUTPUT,
    bk_gpio_config.pin_bit_mask = 1ULL << EXAMPLE_LCD_GPIO_BL;
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    /* lcd -> spi初始化 */
    ESP_LOGD(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = {
        .sclk_io_num = EXAMPLE_LCD_GPIO_SCLK,
        .mosi_io_num = EXAMPLE_LCD_GPIO_MOSI,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(EXAMPLE_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    /* lcd -> params 初始化 */
    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = EXAMPLE_LCD_GPIO_DC,
        .cs_gpio_num = EXAMPLE_LCD_GPIO_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_SPI_NUM, &io_config, &lcd_io), err, TAG, "New panel IO failed");

    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_GPIO_RST,
        .color_space = EXAMPLE_LCD_COLOR_SPACE,
        .bits_per_pixel = EXAMPLE_LCD_BITS_PER_PIXEL,
    };

    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7789(lcd_io, &panel_config, &lcd_panel), err, TAG, "New panel failed");
    esp_lcd_panel_reset(lcd_panel);
    esp_lcd_panel_init(lcd_panel);
    esp_lcd_panel_mirror(lcd_panel, true, true);
    esp_lcd_panel_disp_on_off(lcd_panel, true);
    esp_lcd_panel_invert_color(lcd_panel, true);
    esp_lcd_panel_set_gap(lcd_panel, 0, 20);
    /* 打开背光 */
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_LCD_GPIO_BL, EXAMPLE_LCD_BL_ON_LEVEL));

    return ret;
err:
    if (lcd_panel)
    {
        esp_lcd_panel_del(lcd_panel);
    }
    if (lcd_io)
    {
        esp_lcd_panel_io_del(lcd_io);
    }
    spi_bus_free(EXAMPLE_LCD_SPI_NUM);
    return ret;
}

esp_err_t lvgl_init(void)
{
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,       /* LVGL task priority */
        .task_stack = 4096,       /* LVGL task stack size */
        .task_affinity = -1,      /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500, /* Maximum sleep in LVGL task */
        .timer_period_ms = 5      /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
        .double_buffer = EXAMPLE_LCD_DRAW_BUFF_DOUBLE,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = false,
        /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
        .rotation = {
            .swap_xy = false,
            .mirror_x = true,
            .mirror_y = true,
        },
        .flags = {
            /* 如果开启psram则申请buf到psram，反之申请buf到自身内存里面 */
            // #if CONFIG_SPIRAM
            //             .buff_spiram = true,
            // #else
            .buff_dma = true,
        }};
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    lv_disp_set_rotation(lvgl_disp, LV_DISP_ROT_180);
    return ESP_OK;
}

void lvgl_indev_button_init(void)
{
    const button_config_t lv_button_config[3] = {
        {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = 1500,
            .short_press_time = 180,
            .gpio_button_config = {
                .gpio_num = PREV_BTN,
                .active_level = ACTIVE_LEVEL,
            },
        },
        {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = 1500,
            .short_press_time = 180,
            .gpio_button_config = {
                .gpio_num = NEXT_BTN,
                .active_level = ACTIVE_LEVEL,
            },
        },
        {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = 1500,
            .short_press_time = 180,
            .gpio_button_config = {
                .gpio_num = ENTER_BTN,
                .active_level = ACTIVE_LEVEL,
            },
        },
    };

    lv_disp_t *disp = lv_disp_get_default();
    const lvgl_port_nav_btns_cfg_t btns = {
        .disp = disp,
        .button_prev = &lv_button_config[0],
        .button_next = &lv_button_config[1],
        .button_enter = &lv_button_config[2],
    };
    lvgl_port_add_navigation_buttons(&btns);
    lv_group_t *g = lv_group_get_default();
    if (g == NULL)
    {
        g = lv_group_create();
        lv_group_set_default(g);
    }

    lv_indev_t *cur_drv = NULL;
    for (;;)
    {
        cur_drv = lv_indev_get_next(cur_drv);
        if (!cur_drv)
        {
            break;
        }

        if (cur_drv->driver->type == LV_INDEV_TYPE_KEYPAD)
        {
            lv_indev_set_group(cur_drv, g);
        }

        if (cur_drv->driver->type == LV_INDEV_TYPE_ENCODER)
        {
            lv_indev_set_group(cur_drv, g);
        }
    }
}