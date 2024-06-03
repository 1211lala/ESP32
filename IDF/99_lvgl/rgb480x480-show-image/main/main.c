#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_io.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "esp_heap_caps.h"

#include "p_wifi_sta.h"
#include "sd_spi_fat.h"

#include "lv_demos.h"
#include "lvgl.h"
#include "lvgl_interface.h"
#include "lvglstart.h"
#include "p_mqtt.h"
// LV_FONT_DECLARE(myFont_20)

void lvgl_hander(void *arg);
void system_info(void *arg);

static const char *TAG = "main";

void wifi_event_callback(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {

        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        s_retry_num = 0;
        ESP_LOGI("wifi", "获取IP地址: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI("wifi", "获取网关: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI("wifi", "获取子网掩码: " IPSTR, IP2STR(&event->ip_info.netmask));

        // 为IP地址字符串分配正确大小的缓冲区 (最多15个字符 + 终止符)
        char temp[16];
        sprintf(temp, "%ld.%ld.%ld.%ld",
                (event->ip_info.ip.addr >> 0) & 0xFF,
                (event->ip_info.ip.addr >> 8) & 0xFF,
                (event->ip_info.ip.addr >> 16) & 0xFF,
                (event->ip_info.ip.addr >> 24) & 0xFF);
        const char *ssid = "Kean.2023";
        lv_label_set_text_fmt(wifi_label, "WIFI: %s\n\n  IP : %s", ssid, temp);

        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < 3)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGE("wifi", "重新开始连接AP %d/%d", s_retry_num, 1000);
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            ESP_LOGE("wifi", "连接失败");
        }
    }
}

void app_main(void)
{

    lvgl_disp_init();
    lvgl_indev_init();
    lv_png_init();

    xTaskCreatePinnedToCore(system_info, "system_info", 1024 * 10, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore(lvgl_hander, "lvgl_hander", 1024 * 10, NULL, 5, NULL, 1);
}

lv_obj_t *img = NULL;

void system_info(void *arg)
{
    sd_init_mount_fat();
    img = lv_img_create(lv_scr_act());
    while (1)
    {
        lv_img_set_src(img, "/sdcard/1image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard/2image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard/3image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard4/4image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard/5image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard/6image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard/7image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        lv_img_set_src(img, "/sdcard/8image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void lvgl_hander(void *arg)
{

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5));
        lv_timer_handler();
    }
}