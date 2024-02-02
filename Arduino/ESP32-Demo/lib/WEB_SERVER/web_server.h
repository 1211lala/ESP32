#ifndef _WEB_SERVER_H_
#define _WEB_SERVER_H_

#include "common.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h>
#include "LittleFS_fun.h"
#include "bsp_wifi.h"

void web_server_init(void);
void set_wifi_config_callback(AsyncWebServerRequest *request);
void get_wifi_config_callback(AsyncWebServerRequest *request);
#endif