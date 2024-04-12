#include "main.h"
#include "common.h"
#include "st7701s_lvgl.h"
#include "wifi_fun.h"
#include "demos/lv_demos.h"
#include "fatfs_sd.h"

SemaphoreHandle_t xSemaphore = NULL;
void setup()
{

    Serial.begin(115200);
    vSemaphoreCreateBinary(xSemaphore);
    xSemaphoreTake(xSemaphore, 100);

    xTaskCreatePinnedToCore(lvgl_task, "lvgl_task", 1024 * 6, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(wifi_task, "wifi_task", 1024 * 16, NULL, 4, NULL, 1);

    vTaskDelete(NULL);
}
void loop()
{
}

lv_obj_t *wifiLabel = NULL;
lv_obj_t *sdLabel = NULL;

void lvgl_task(void *arg)
{
    if (st7701s_lvgl_init() == ESP_OK)
    {
        loginfo("lvgl start ok\r\n");
    }
    lv_obj_t *bgObj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bgObj, WIDTH, HEIGHT);
    lv_obj_align(bgObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(bgObj, lv_color_hex(0x1b1b1b), LV_STATE_DEFAULT);

    wifiLabel = lv_label_create(bgObj);
    lv_obj_align(wifiLabel, LV_ALIGN_CENTER, 0, -100);
    lv_label_set_text_fmt(wifiLabel, "Connect WiFi ...");
    lv_obj_set_style_text_font(wifiLabel, &lv_font_montserrat_30, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(wifiLabel, lv_color_hex(0xff0000), LV_STATE_DEFAULT);

    sdLabel = lv_label_create(bgObj);
    lv_obj_align(sdLabel, LV_ALIGN_CENTER, 0, 50);
    lv_label_set_text_fmt(sdLabel, "sd mount ...");
    lv_obj_set_style_text_font(sdLabel, &lv_font_montserrat_30, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(sdLabel, lv_color_hex(0xff0000), LV_STATE_DEFAULT);

    xSemaphoreGive(xSemaphore);
    while (1)
    {
        lv_timer_handler();
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void wifi_task(void *arg)
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);

    wifi_sta_init(_wc);
    lv_label_set_text_fmt(wifiLabel, "SSID: %s \n\nIP: %s", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

    sd_mount_fatfs();
    uint32_t total = (uint32_t)(SD.totalBytes() / (1024 * 1024));
    uint32_t used = (uint32_t)(SD.usedBytes() / (1024 * 1024));
    lv_label_set_text_fmt(sdLabel, "Total: %d MB\n\nUsed: %d MB", total, used);

    lv_obj_del_delayed(wifiLabel, 10000);
    lv_obj_del_delayed(sdLabel, 10000);
    while (1)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}