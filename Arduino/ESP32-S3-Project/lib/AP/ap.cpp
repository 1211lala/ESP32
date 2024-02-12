#include "ap.h"

void bsp_wifi_ap_init(const char *ssd, const char *password, IPAddress ip, IPAddress getway, IPAddress subnet)
{
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAPConfig(ip, getway, subnet);
    while (!WiFi.softAP(ssd, password))
    {
        Serial.print('.');
        delay(50);
    }
    Serial.printf("(%d) esp mode: %s\r\n", millis(), "ap");
    Serial.printf("(%d) wifi ssd: %s\r\n", millis(), WiFi.softAPSSID().c_str());
    Serial.printf("(%d) wifi password : %s\r\n", millis(), password);
    Serial.printf("(%d) ip: %s\r\n", millis(), WiFi.softAPIP().toString().c_str());
}