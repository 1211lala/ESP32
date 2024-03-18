#include "bsp_wifi.h"

_wifi_config _wc = {
    // .ssid = "Xiaomi_4C",
    // .password = "121314liuAO#",
    .ssid = "Kean",
    .password = "Kean.2023",
    // .ssid = "ESP32- WiFi",
    // .password = "123456789",
};

/******************************************************************************
 * 函数功能:设置wifi模式为ap模式
 ******************************************************************************/
void bsp_wifi_ap_init(_wifi_config &config)
{
    IPAddress ip = ipaddress_ascll_to_int(config.ip.c_str());
    IPAddress gateway = ipaddress_ascll_to_int(config.gateway.c_str());
    IPAddress subnet = ipaddress_ascll_to_int(config.subnet.c_str());

    WiFi.mode(WIFI_MODE_AP);
    // WiFi.softAPConfig(ip, gateway, subnet);
    while (!WiFi.softAP(config.ssid, config.password))
    {
        ledToogle();
        delay(100);
    }
    ledOn();
    Serial.printf("\r\n");
    loginfo("WiFi AP: %s\r\n", WiFi.softAPSSID().c_str());
    loginfo("IP: %s\r\n", WiFi.softAPIP().toString().c_str());
}

/******************************************************************************
 * 函数功能:设置wifi模式为sta模式
 * config:需要,连接的wifi信息
 ******************************************************************************/
void bsp_wifi_sta_init(_wifi_config &config)
{
    IPAddress ip = ipaddress_ascll_to_int(config.ip.c_str());
    IPAddress gateway = ipaddress_ascll_to_int(config.gateway.c_str());
    IPAddress subnet = ipaddress_ascll_to_int(config.subnet.c_str());
    IPAddress dns = ipaddress_ascll_to_int(config.dns.c_str());

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(config.ssid, config.password);
    // WiFi.config(ip, gateway, subnet);
    /* 开启自动重连 */
    WiFi.getAutoReconnect();
    /* 关闭睡眠模式 */
    WiFi.setSleep(false);
    while (WiFi.status() != WL_CONNECTED)
    {
        ledToogle();
        delay(100);
    }
    ledOn();
    loginfo("WIFI STA: %s\r\n", WiFi.SSID().c_str());
    loginfo("IP: %s\r\n\r\n", WiFi.localIP().toString().c_str());
}

/******************************************************************************
 * 函数功能:将字符型的 IPAddress 数据转换成 Int型数据
 * ipbuf:需要转换的字符型 IPAddress 数据
 * return:转换成功的 IPAddress数据
 ******************************************************************************/
IPAddress ipaddress_ascll_to_int(const char *ipbuf)
{
    uint8_t flag = 1;
    uint8_t count = 0;
    char ip1[5] = {0};
    char ip2[5] = {0};
    char ip3[5] = {0};
    char ip4[5] = {0};

    for (uint8_t i = 0; i < strlen(ipbuf); i++)
    {
        if (ipbuf[i] == '.')
        {
            flag += 1;
            count = 0;
        }
        else
        {
            if (flag == 1)
            {
                ip1[count] = ipbuf[i];
                count += 1;
            }
            if (flag == 2)
            {
                ip2[count] = ipbuf[i];
                count += 1;
            }
            if (flag == 3)
            {
                ip3[count] = ipbuf[i];
                count += 1;
            }
            if (flag == 4)
            {
                ip4[count] = ipbuf[i];
                count += 1;
            }
        }
    }
    ip1[strlen(ip1)] = '\0';
    ip2[strlen(ip2)] = '\0';
    ip3[strlen(ip3)] = '\0';
    ip4[strlen(ip4)] = '\0';

    IPAddress address(atoi(ip1), atoi(ip2), atoi(ip3), atoi(ip4));
    return address;
}

String get_wifi_encryption_type(int32_t i)
{
    switch (WiFi.encryptionType(i))
    {
    case WIFI_AUTH_OPEN:
        return "open";
    case WIFI_AUTH_WEP:
        return "WEP";
    case WIFI_AUTH_WPA_PSK:
        return "WPA";
    case WIFI_AUTH_WPA2_PSK:
        return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:
        return "WPA+WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:
        return "WPA2-EAP";
    case WIFI_AUTH_WPA3_PSK:
        return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK:
        return "WPA2+WPA3";
    case WIFI_AUTH_WAPI_PSK:
        return "WAPI";
    }
    return "unknown";
}

vector<_wifi_params> wifi_scan_list(void)
{
    vector<_wifi_params> list;

    list.clear();
    int16_t scan_count = WiFi.scanNetworks();
    if (scan_count == 0)
    {
        _wifi_params _wp;
        _wp.ssid = "NULL";
        list.push_back(_wp);
    }
    else
    {
        for (int i = 0; i < scan_count; ++i)
        {
            _wifi_params _wp;
            _wp.ssid = WiFi.SSID(i);
            _wp.rssi = WiFi.RSSI(i);
            _wp.channel = WiFi.channel(i);
            _wp.encryption = get_wifi_encryption_type(i);
            list.push_back(_wp);
        }
    }
    WiFi.scanDelete();
    return list;
}