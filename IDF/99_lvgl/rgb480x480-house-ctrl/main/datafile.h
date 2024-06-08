#ifndef _DATAFILE_H
#define _DATAFILE_H

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "mqtt_client.h"
#include "lvgl.h"
/*******************************************WIFI********************************************************/
struct WiFi_Param
{
    char *ssid;
    char *password;
    char *ip;
    char *gateway;
    char *subnet;
    char *dns;
    bool isConnected;
};
extern struct WiFi_Param wp;
extern EventGroupHandle_t s_wifi_event_group;

/*******************************************WIFI********************************************************/
#define mqttUri "mqtt://mqtt.eclipseprojects.io"

// #define mqttUri "mqtt://192.168.10.40"

#define mqttRoomLedCtrl "/liuao/switch/roomLed/write"
#define mqttFanCtrl "/liuao/switch/fan/write"
#define mqttCurtainCtrl "/liuao/switch/curtain/write"
#define mqttWcLedCtrl "/liuao/switch/WcLed/write"

#define mqttRoomLedRead "/liuao/switch/roomLed/read"
#define mqttFanRead "/liuao/switch/fan/read"
#define mqttCurtainRead "/liuao/switch/curtain/read"
#define mqttWcLedRead "/liuao/switch/WcLed/read"

#define mqttLoop "/liuao/loop"
#define mqttLastWill "liuao/lastwill"
struct mqtt_dev
{
    TaskHandle_t taskHandle;
    esp_mqtt_client_handle_t client;
    esp_mqtt_client_config_t config;
    bool isConnected;
};
extern struct mqtt_dev mqttdev;

/*******************************************LVGL********************************************************/
extern uint8_t use_lvgl;

extern lv_obj_t *switch0;
extern lv_obj_t *switch1;
extern lv_obj_t *switch2;
extern lv_obj_t *switch3;
extern lv_obj_t *label00;
extern lv_obj_t *label11;
extern lv_obj_t *label22;
extern lv_obj_t *label33;



#endif