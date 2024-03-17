#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "OneButton.h"

#include "common.h"
#include "bsp_wifi.h"
#include "key_cfg.h"
#include "LittleFS_fun.h"
#include "web_server.h"
#include "minioled.h"

struct Control_Param def_cp;
struct Ws2812Param
{
    int count = 0;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    int Color;
};

Ws2812Param wp;
void get_rocker_value(void *arg);
void start_wifi_webserver(void *arg);
void ws2812_show(void *aeg);
void get_esp_info(void *arg);
void get_key_value(void *arg);
void oled_show(void *arg);
#endif