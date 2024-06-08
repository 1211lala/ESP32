#include "datafile.h"

#include "wifi_sta.h"
#include "wifi_mqtt.h"
#include "sd_spi_fat.h"
#include "lvgl.h"
#include "lv_demos.h"
#include "lvgl_interface.h"
#include "./lvgl_app/lvglstart.h"

void app_main(void)
{
    lvgl_start_task();
    menu_demo();
    wifi_sta_init(&wp, wifi_event_callback);
    mqtt_start_task();

    while (mqttdev.isConnected == false)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
