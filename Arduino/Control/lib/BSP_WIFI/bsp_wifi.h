#ifndef _BSP_WIFI_H_
#define _BSP_WIFI_H_

#include <Arduino.h>
#include <WiFi.h>
#include "common.h"
#include "key_cfg.h"
#include "littlefs_fun.h"


void bsp_wifi_ap_init(const char *ssd, const char *password, IPAddress ip, IPAddress getway, IPAddress subnet);
void bsp_wifi_sta_init(_wifi_config &config);
IPAddress ipaddress_ascll_to_int(const char *ipbuf);
vector<_wifi_params> wifi_scan_list(void);
#endif