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

#include "cJSON.h"
#include <esp_http_server.h>
TaskHandle_t wifi_handle = NULL;

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

        ESP_LOGI("WIFI", "STA-IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI("WIFI", "STA-GATEWAY: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI("WIFI", "STA-SUBNET: " IPSTR, IP2STR(&event->ip_info.netmask));

        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < 10)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI("WIFI", "连接失败,开始重新连接...");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGE("WIFI", "连接失败!!!");
    }
}

static const char *TAG = "example";

static esp_err_t http_hello_get_handler(httpd_req_t *req)
{
    char *buf;
    size_t buf_len;

    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
        {

            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }
    const char *resp_str = (const char *)req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    if (httpd_req_get_hdr_value_len(req, "Host") == 0)
    {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}
static const httpd_uri_t hello = {
    .uri = "/hello",
    .method = HTTP_GET,
    .handler = http_hello_get_handler,
    .user_ctx = "Hello World!"};

void task_http_server(void *arg)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    ESP_LOGI("TAG", "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI("TAG", "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
    while (1)
    {
        vTaskDelay(1000 / portTICK);
        led_blink();
    }
}

void app_main()
{
    led_init();
    key_general_init();
    spiffs_mount();
    wifi_sta_init(&wp, wifi_event_handler);
    xTaskCreate(task_http_server, "task_http_server", 1024 * 4, NULL, 5, &wifi_handle);
}
