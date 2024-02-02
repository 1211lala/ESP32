#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LED 2
#define KEY 0

const char *ssid = "Kean";
const char *password = "Kean.2023";

AsyncWebServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    pinMode(KEY, INPUT_PULLUP);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! I am ESP32."); });

    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    Serial.printf("ota test 2\r\n");
    delay(1000);
}

uint8_t get_key_value(uint8_t waittime)
{
    static uint64_t tick = 0;
    static uint8_t flag = 0;

    uint8_t key_status = 0;

    key_status = digitalRead(KEY);
    if (flag == 0 && key_status == 0)
    {
        tick = millis();
        flag = 1;
    }
    if (flag == 1 && ((millis() - tick) > waittime))
    {
        if (key_status == 0)
        {
            flag = 2;
            return 1;
        }
    }
    if (key_status == 1)
    {
        flag = 0;
    }
    return 0;
}
