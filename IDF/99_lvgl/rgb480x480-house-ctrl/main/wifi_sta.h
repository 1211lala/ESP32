#ifndef _WIFI_STA_H_
#define _WIFI_STA_H_

#include "datafile.h"
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_netif_ip_addr.h>
#include "esp_netif.h"
#include <arpa/inet.h>

#define WIFI_FAIL_BIT (1 << 0)
#define WIFI_CONNECTED_BIT (1 << 1)


void wifi_sta_init(struct WiFi_Param *WiFi_Config, esp_event_handler_t esp_event_callback);
void wifi_event_callback(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
#endif