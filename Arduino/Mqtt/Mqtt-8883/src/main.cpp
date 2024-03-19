#include "main.h"
#include "mqtt.h"

/******************************************************************************
 * 函数功能:Mqtt回调函数
 ******************************************************************************/
void mqttcallBack(char *topic, uint8_t *payload, unsigned int len)
{
   loginfo("topic: %s\r\n", topic);
   payload[len] = '\0';
   loginfo("data: %s\r\n", payload);
   loginfo("len: %d\r\n", len);

   DynamicJsonDocument doc(100);
   deserializeJson(doc, payload);
   bool ledState = digitalRead(LED);
   ledState = doc["status"].as<int>();

   digitalWrite(LED, ledState);
}

void mqttLoop(void *arg)
{
   while (1)
   {
      if (mqttClient.connected())
      {
         mqttClient.loop();
      }
      else
      {
         connectMqttServer(MQTT_CLIENTID);
      }
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}

void keyScan(void *arg)
{
   while (1)
   {
      if (digitalRead(KEY) == LOW)
      {
         bool status = digitalRead(LED);
         String str = "{\"ledStatus\":" + String(status) + "}";
         mqttPublishTopic(MQTT_PUB, str.c_str(), 0);
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }
}
void setup()
{
   pinMode(LED, OUTPUT);
   pinMode(KEY, INPUT | PULLUP);
   Serial.begin(115200);
   bsp_wifi_sta_init(_wc);
   mqtt_clinet_init(mqttcallBack);

   xTaskCreatePinnedToCore(mqttLoop, "mqttLoop", 1024 * 10, NULL, 3, NULL, 1);
   xTaskCreatePinnedToCore(keyScan, "keyScan", 1024 * 4, NULL, 4, NULL, 1);
   vTaskDelete(NULL);
}

void loop(void)
{
}