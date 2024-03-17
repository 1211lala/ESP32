#ifndef _BSP_WIFI_H_
#define _BSP_WIFI_H_


#include "common.h"
#include <WiFi.h>

struct _wifi_config
{
    String ssid;
    String password;
    String ip;
    String subnet;
    String gateway;
    String dns;
};
extern _wifi_config _wc;

struct _wifi_params
{
    String ssid;
    int8_t rssi;
    int8_t channel;
    String encryption;
};

void bsp_wifi_ap_init(_wifi_config &config);
void bsp_wifi_sta_init(_wifi_config &config);
IPAddress ipaddress_ascll_to_int(const char *ipbuf);
vector<_wifi_params> wifi_scan_list(void);
#endif