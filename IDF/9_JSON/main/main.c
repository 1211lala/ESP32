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

void task_json(void *arg)
{
    cJSON *pRoot = cJSON_CreateObject();  // 创建JSON根部结构体
    cJSON *pValue = cJSON_CreateObject(); // 创建JSON子叶结构体

    cJSON_AddStringToObject(pRoot, "mac", "65:c6:3a:b2:33:c8"); // 添加字符串类型数据到根部结构体
    cJSON_AddItemToObject(pRoot, "value", pValue);
    cJSON_AddStringToObject(pValue, "day", "Sunday"); // 添加字符串类型数据到子叶结构体
    cJSON_AddNumberToObject(pRoot, "number", 2);      // 添加整型数据到根部结构体

    int hex[5] = {51, 15, 63, 22, 96};
    cJSON *pHex = cJSON_CreateIntArray(hex, 5); // 创建整型数组类型结构体
    cJSON_AddItemToObject(pRoot, "hex", pHex);  // 添加整型数组到数组类型结构体

    cJSON *pArray = cJSON_CreateArray();                  // 创建数组类型结构体
    cJSON_AddItemToObject(pRoot, "info", pArray);         // 添加数组到根部结构体
    cJSON *pArray_relay = cJSON_CreateObject();           // 创建JSON子叶结构体
    cJSON_AddItemToArray(pArray, pArray_relay);           // 添加子叶结构体到数组结构体
    cJSON_AddStringToObject(pArray_relay, "relay", "on"); // 添加字符串类型数据到子叶结构体

    char *sendData = cJSON_Print(pRoot); // 从cJSON对象中获取有格式的JSON对象
    printf("data:%s\n", sendData);       // 打印数据

    // receiveData是要剖析的数据
    // 首先整体判断是否为一个json格式的数据
    cJSON *pJsonRoot = cJSON_Parse(sendData);
    // 如果是否json格式数据
    if (pJsonRoot != NULL)
    {

        char bssid[23] = {0};
        cJSON *pMacAdress = cJSON_GetObjectItem(pJsonRoot, "mac"); // 解析mac字段字符串内容
        if (!pMacAdress)
            return; // 判断mac字段是否json格式
        else
        {
            if (cJSON_IsString(pMacAdress)) // 判断mac字段是否string类型
            {
                strcpy(bssid, pMacAdress->valuestring); // 拷贝内容到字符串数组
                printf("mac: %s", bssid);
            }
        }
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
    spiffs_mount();
    wifi_sta_init(&wp, wifi_event_handler);
    xTaskCreate(task_json, "task_json", 1024 * 4, NULL, 5, &wifi_handle);
}
