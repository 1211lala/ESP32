#include "p_wifi_sta.h"

struct WiFi_Param wp = {
    .ssid = "Kean",
    .password = "Kean.2023",
    // .ssid = "Xiaomi_4C",
    // .password = "121314liuAO#",
};

void wifi_sta_init(struct WiFi_Param *WiFi_Config, esp_event_handler_t esp_event_callback)
{
    // 1: 初始化 NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    { // 如果 NVS 分区已满或者版本不匹配，需要擦除 NVS 分区并重新初始化
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // 2: 初始化网络接口
    ESP_ERROR_CHECK(esp_netif_init());

    // 3: 创建默认的事件循环
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 4: 创建默认的 Wi-Fi Station 网络接口
    esp_netif_create_default_wifi_sta();

    // 5: 初始化 Wi-Fi 栈，之后可以进行Wi-Fi 连接、启动 Wi-Fi 等操作。
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 6: 配置STA参数 如SSID, 密码等参数
    wifi_sta_config_t cfg_sta = {};
    strcpy((char *)cfg_sta.ssid, WiFi_Config->ssid);
    strcpy((char *)cfg_sta.password, WiFi_Config->password);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t *)&cfg_sta));
    /**************************************可选 配置静态IP**********************************************/
    ESP_ERROR_CHECK(esp_netif_dhcpc_stop(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF")));
    esp_netif_ip_info_t ip_inifo = {};
    inet_aton("192.168.8.188", &ip_inifo.ip);
    inet_aton("192.168.8.1", &ip_inifo.gw);
    inet_aton("255.255.255.0", &ip_inifo.netmask);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_inifo));
    /**************************************可选 配置静态IP**********************************************/
    /**************************************可选 配置DNS服务器**********************************************/
    esp_netif_dns_info_t dnsInfo = {};
    inet_aton("114.114.114.114", &dnsInfo.ip.u_addr.ip4);
    ESP_ERROR_CHECK(esp_netif_set_dns_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), ESP_NETIF_DNS_MAIN, &dnsInfo));
    /**************************************可选 配置DNS服务器**********************************************/
    // 7: 配置WiFi模式
    esp_wifi_set_mode(WIFI_MODE_STA);

    // 9: 设置WiFi事件
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, esp_event_callback, NULL, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, esp_event_callback, NULL, NULL);

    /*
    10: 开启WiFi，调用 esp_wifi_start() 函数后，ESP32 将开始扫描 Wi-Fi 网络并尝试连接。
        在这个过程中，系统会产生多种类型的 Wi-Fi 事件，例如扫描结果事件、连接事件等。
        这些事件会通过事件循环传递给注册的事件处理函数。
    */
    esp_wifi_start();
}