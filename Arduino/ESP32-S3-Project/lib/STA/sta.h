#ifndef _STA_H_
#define _STA_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

void bsp_wifi_sta_init(const char *ssd, const char *password, IPAddress ip, IPAddress getway, IPAddress subnet);
#endif