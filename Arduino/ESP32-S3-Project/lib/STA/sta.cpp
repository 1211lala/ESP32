#include "sta.h"

void bsp_wifi_sta_init(const char *ssd, const char *password, IPAddress ip, IPAddress getway, IPAddress subnet)
{
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssd, password);
    WiFi.config(ip, getway, subnet);
    WiFi.getAutoConnect(); /* 开启自动重连 */
    WiFi.setSleep(false);  /* 关闭睡眠模式 */
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(50);
    }
    Serial.printf("\r\n(%d) esp mode: %s\r\n", millis(), "sta");
    Serial.printf("(%d) ip: %s\r\n", millis(), WiFi.localIP().toString().c_str());
}