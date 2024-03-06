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
    wifi_config_t config;
    esp_wifi_get_config(WIFI_IF_STA, &config);
    printf("ssid: %s  ", (char *)config.sta.ssid);
    printf("password: %s\r\n", (char *)config.sta.password);

    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    esp_netif_get_ip_info(netif, &ip_info);
    printf("IP: " IPSTR " ", IP2STR(&ip_info.ip));
    printf("Mask: " IPSTR " ", IP2STR(&ip_info.netmask));
    printf("Gw: " IPSTR "\n", IP2STR(&ip_info.gw));

    esp_netif_dns_info_t dsn_info;
    esp_netif_get_dns_info(netif, ESP_NETIF_DNS_MAIN, &dsn_info);
    printf("dns: " IPSTR "\n", IP2STR(&dsn_info.ip.u_addr.ip4));
    while (1)
    {

        vTaskDelay(1000 / portTICK);
        led_blink();
    }
}

char *writeBuff = "111111111111111111\r\n222222222222222222222\r\n3333333333333333333333\r\n";
void app_main()
{

    ESP_LOGE("RAM","初始内存: %ldKb\n", esp_get_free_heap_size() / 1024);
    led_init();
    spiffs_mount();
    ESP_LOGE("RAM","挂在SPIFFS后: %ldKb\n", esp_get_free_heap_size() / 1024);
    char *fs_buff = NULL;
    int len = fs_write("/spiffs/write.txt", writeBuff, strlen(writeBuff));

    len = fs_read("/spiffs/write.txt", &fs_buff, 1024 * 6);
    if (len > 0)
    {
        ESP_LOGE("RAM","读取文件后: %ldKb\n", esp_get_free_heap_size() / 1024);
        fs_buff[len] = '\0';
        printf("读取%d字节数据, => %s\r\n", len, fs_buff);
        free(fs_buff);
    }
    ESP_LOGE("RAM","释放后: %ldKb\n", esp_get_free_heap_size() / 1024);

    len = fs_read("/spiffs/p_uart.c", &fs_buff, 1024 * 6);
    if (len > 0)
    {
        ESP_LOGE("RAM","读取文件后: %ldKb\n", esp_get_free_heap_size() / 1024);
        fs_buff[len] = '\0';
        printf("读取%d字节数据, => %s\r\n", len, fs_buff);
        free(fs_buff);
    }
    ESP_LOGE("RAM","释放后: %ldKb\n", esp_get_free_heap_size() / 1024);

    len = fs_read("/spiffs/json.json", &fs_buff, 1024 * 6);
    if (len > 0)
    {
        ESP_LOGE("RAM","读取文件后: %ldKb\n", esp_get_free_heap_size() / 1024);
        fs_buff[len] = '\0';
        printf("读取%d字节数据, => %s\r\n", len, fs_buff);
        free(fs_buff);
    }
    ESP_LOGE("RAM","释放后: %ldKb\n", esp_get_free_heap_size() / 1024);
    wifi_sta_init(&wp, wifi_event_handler);
    ESP_LOGE("RAM","开启WiFi后: %ldKb\n", esp_get_free_heap_size() / 1024);
    xTaskCreate(task_wifi, "task_wifi", 1024 * 4, NULL, 5, &wifi_handle);
    ESP_LOGE("RAM","开启任务后后: %ldKb\n", esp_get_free_heap_size() / 1024);
}
