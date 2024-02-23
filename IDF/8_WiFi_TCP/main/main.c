#include "common.h"
#include "bspled.h"
#include "bspkey.h"
#include "p_exti.h"
#include "p_adc.h"
#include "p_uart.h"
#include "p_gpttim.h"
#include "p_i2c_sht20.h"
#include "p_wifi_sta.h"
#include "lwip/sockets.h"

int flag = 1;
TaskHandle_t wifi_handle;
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
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
        flag = 0;
    }
}

void task_tcp(void *arg)
{

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd < 0)
    {
        ESP_LOGE("Socket", "创建失败\n");
    }
    // 定义一个sockaddr_in的地址结构体
    struct sockaddr_in serverAddress;
    // IPV4协议
    serverAddress.sin_family = AF_INET;
    // ip地址转换，将点分十进制转换成二进制整数，这里的"192.168.191.1"为所连接的主机的IP地址
    inet_pton(AF_INET, "192.168.8.100", &serverAddress.sin_addr.s_addr);
    // 端口
    serverAddress.sin_port = htons(8080);
    // 和TCP服务器建立连接，当TCP调用connect时就会调用一个三次握手过程
    int rc = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));
    if (rc != 0)
    {
        ESP_LOGE("Socket", "连接失败");
    }

    char *data = "Hello world\r\n"; // 要发送的数据

    while (1)
    {
        rc = send(sockfd, data, strlen(data), 0); // 发送数据
        ESP_LOGI("TAG", "send len: %d", rc);
        vTaskDelay(1000 / portTICK);
    }
    close(sockfd);
}

void app_main()
{
    wifi_sta_init(&wp, wifi_event_handler);
    while (flag)
    {
        vTaskDelay(100 / portTICK);
    }
    xTaskCreate(task_tcp, "task_tcp", 1024 * 4, NULL, 5, &wifi_handle);
    vTaskDelete(NULL);
}
