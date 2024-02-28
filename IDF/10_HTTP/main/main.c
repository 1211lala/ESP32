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
#include "esp_http_client.h"

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

        ESP_LOGI("WIFI", "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI("WIFI", "Got GW: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI("WIFI", "Got NT: " IPSTR, IP2STR(&event->ip_info.netmask));

        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < 10)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI("WIFI", "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI("WIFI", "connect to the AP fail");
    }
}
char local_response_buffer[1000] = {0};
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    char *TAG = "HTTP";

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGI(TAG, "错误事件");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "连接成功事件");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG, "发送头事件");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG, "接收头事件");
        printf("%.*s", evt->data_len, (char *)evt->data);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "接收数据事件, len=%d", evt->data_len);

        if (!esp_http_client_is_chunked_response(evt->client))
        {
            printf("%.*s", evt->data_len, (char *)evt->data);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP会话完成");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP已断开");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}

/*

Allow potentially insecure options

Skip server certificate verification by default (WARNING: ONLY FOR TESTING PURPOSE, READ HELP)

*/
void task_http(void *arg)
{

    while (1)
    {
        vTaskDelay(1000 / portTICK);
        led_blink();
    }
}

void http_request(char *url)
{

    esp_http_client_config_t config = {

        .method = HTTP_METHOD_GET,
        .url = url,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,
        .buffer_size = 2048,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    // GET
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI("TAG", "HTTP请求成功  CODE = %d, LEN = %llu", esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE("TAG", "HTTP请求失败: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}
void task_key(void *arg)
{
    int flag = 0;
    while (1)
    {
        int value = get_key_value(0);
        if (value)
        {
            if (flag == 0)
            {
                // http_request("http://v1.yiketianqi.com/api?unescape=1&version=v61&appid=88344185&appsecret=58ixdwXt");
                http_request("https://example.com/");
                flag = 1;
            }
            else if (flag == 1)
            {
                http_request("http://192.168.8.93:8080/get_software_version");
                flag = 0;
            }
        }
        vTaskDelay(50 / portTICK);
    }
}
void app_main()
{
    led_init();
    key_general_init();
    spiffs_mount();
    wifi_sta_init(&wp, wifi_event_handler);
    xTaskCreate(task_http, "task_http", 1024 * 4, NULL, 5, &wifi_handle);
    xTaskCreate(task_key, "task_key", 1024 * 8, NULL, 10, &wifi_handle);
}
