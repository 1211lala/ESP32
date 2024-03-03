#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"
#include "p_gpttim.h"
#include "p_i2c_sht20.h"
#include "p_wifi_sta.h"
#include "spiffs.h"

TaskHandle_t wifi_handle;
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        ESP_LOGI("TEST_ESP32", "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI("TEST_ESP32", "Got GW: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI("TEST_ESP32", "Got NT: " IPSTR, IP2STR(&event->ip_info.netmask));

        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < 10)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI("TAG", "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI("TAG", "connect to the AP fail");
    }
}

void task_wifi(void *arg)
{
    // wifi_config_t config;
    // esp_wifi_get_config(WIFI_IF_STA, &config);
    // printf("ssid: %s  ", (char *)config.sta.ssid);
    // printf("password: %s\r\n", (char *)config.sta.password);

    // esp_netif_ip_info_t ip_info;
    // esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    // esp_netif_get_ip_info(netif, &ip_info);
    // printf("IP: " IPSTR " ", IP2STR(&ip_info.ip));
    // printf("Mask: " IPSTR " ", IP2STR(&ip_info.netmask));
    // printf("Gw: " IPSTR "\n", IP2STR(&ip_info.gw));

    // esp_netif_dns_info_t dsn_info;
    // esp_netif_get_dns_info(netif, ESP_NETIF_DNS_MAIN, &dsn_info);
    // printf("dns: " IPSTR "\n", IP2STR(&dsn_info.ip.u_addr.ip4));
    while (1)
    {

        vTaskDelay(1000 / portTICK);
        led_blink();
    }
}

void app_main()
{
    led_init();
    spiffs_mount();
    int len = isExist("/spiffs/p_uart.c");
    char *fs_buff = (char *)malloc(len + 1);
    printf("申请了%d字节\r\n", len + 1);
    len = fs_read("/spiffs/p_uart.c", fs_buff, len);

    fs_buff[len] = '\0';

    printf("%s\r\n", fs_buff);

    free(fs_buff);

    xTaskCreate(task_wifi, "task_wifi", 1024 * 4, NULL, 5, &wifi_handle);
}
