#include "main.h"
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

#define MQTT_URL "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_PUB "liuao/ledStatus"
#define MQTT_SUB "liuao/setLed"

String clientId = "";
bool ledState = 0;

void connectMqttServer(const char *clientId)
{
   if (mqttClient.connect(clientId))
   {
      loginfo("mqtt Server: %s 连接成功.   clientId = %s\r\n", MQTT_URL, clientId);
   }
   else
   {
      loginfo("mqtt Server: %s 连接失败.    clientId = %s\r\n", MQTT_URL, clientId);
      loginfo("错误码: %d\r\n", mqttClient.state());
   }
}

void mqttPublishTask(void *arg)
{
   int32_t lastMills = 0;
   while (1)
   {
      if (mqttClient.connected())
      {
         if (millis() - lastMills >= 1000)
         {
            bool status = digitalRead(LED);
            String send = "{\"ledStatus\":" + String(status) + "}";

            if (!mqttClient.publish(MQTT_PUB, send.c_str()))
            {
               loginfo("%s报文发送失败\r\n", MQTT_PUB);
            }
         }
         mqttClient.loop();
      }
      else
      {
         connectMqttServer(clientId.c_str());
      }
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}

void mqttcallBack(char *topic, uint8_t *payload, unsigned int len)
{
   loginfo("topic: %s\r\n", topic);
   payload[len] = '\0';
   loginfo("data: %s\r\n", payload);
   loginfo("len: %d\r\n", len);
   DynamicJsonDocument doc(100);
   deserializeJson(doc, payload);
   ledState =  doc["status"].as<int>();
   
   digitalWrite(LED, ledState);
}

void mqttSubscrbeTask(void *arg)
{
   mqttClient.setCallback(mqttcallBack);
   if (!mqttClient.subscribe(MQTT_SUB))
   {
      loginfo("%s订阅失败\r\n", MQTT_SUB);
      vTaskDelete(NULL);
   }
   while (1)
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}
void setup()
{
   pinMode(LED, OUTPUT);
   Serial.begin(115200);

   clientId = "ESP32-" + WiFi.macAddress();

   bsp_wifi_sta_init(_wc);
   mqttClient.setServer(MQTT_URL, MQTT_PORT);
   connectMqttServer(clientId.c_str());

   xTaskCreatePinnedToCore(mqttPublishTask, "mqttPublishTask", 1024 * 4, NULL, 4, NULL, 1);
   xTaskCreatePinnedToCore(mqttSubscrbeTask, "mqttSubscrbeTask", 1024 * 4, NULL, 5, NULL, 1);
   vTaskDelete(NULL);
}

void loop(void)
{
}