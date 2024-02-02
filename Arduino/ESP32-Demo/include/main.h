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


void get_rocker_value(void *arg);
void start_wifi_webserver(void *arg);
void ws2812_show(void *aeg);
void get_esp_info(void *arg);
void get_key_value(void *arg);
void oled_show(void* arg);
#endif