#include "st7701s_lvgl.h"

void setup()
{
    Serial.begin(115200);
    if(st7701s_lvgl_init() == ESP_OK)
    {
        loginfo("lvgl start ok\r\n");
    }
    lvgl_runing_app();
}

void loop()
{
    lv_timer_handler(); 
    delay(5);
}
