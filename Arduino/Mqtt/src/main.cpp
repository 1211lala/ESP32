#include "main.h"
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

#define MQTT_URL "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_PUB "liuao/ledStatus"
#define MQTT_RetainPUB "liuao/retainedMsg"

#define MQTT_SUB "liuao/setLed"

String clientId = "";
bool ledState = 0;

const int subQoS = 1;            // 客户端订阅主题时使用的QoS级别（截止2020-10-07，仅支持QoS = 1，不支持QoS = 2）
const bool cleanSession = false; // 清除会话（如QoS>0必须要设为false）

#define willTopic "liuao/willMsg"      // 遗嘱主题
const char *willMsg = "esp32-offline"; // 遗嘱主题信息
const int willQos = 0;                 // 遗嘱QoS
const int willRetain = true;           // 遗嘱保留

void connectMqttServer(const char *clientId)
{
   /* cleanSession 设置为 false*/
   if (mqttClient.connect(clientId, NULL, NULL, willTopic, willQos, willRetain, willMsg, cleanSession))
   {
      loginfo("mqtt Server: %s 连接成功.   clientId = %s\r\n", MQTT_URL, clientId);
   }
   else
   {
      loginfo("mqtt Server: %s 连接失败.    clientId = %s\r\n", MQTT_URL, clientId);
      loginfo("错误码: %d\r\n", mqttClient.state());
   }
}

/******************************************************************************
 * 函数功能:演示Mqtt的发布消息
 ******************************************************************************/
void mqttPublishTask(void *arg)
{
   while (!mqttClient.connected())
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
   int32_t lastMills = 0;
   while (1)
   {
      if (millis() - lastMills >= 1000)
      {
         lastMills = millis();
         bool status = digitalRead(LED);
         String send = "{\"ledStatus\":" + String(status) + "}";

         if (!mqttClient.publish(MQTT_PUB, send.c_str()))
         {
            loginfo("%s报文发送失败\r\n", MQTT_PUB);
         }
      }
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}

/******************************************************************************
 * 函数功能:演示Mqtt的订阅消息(Qos == 1)
 ******************************************************************************/
void mqttcallBack(char *topic, uint8_t *payload, unsigned int len)
{
   loginfo("topic: %s\r\n", topic);
   payload[len] = '\0';
   loginfo("data: %s\r\n", payload);
   loginfo("len: %d\r\n", len);

   DynamicJsonDocument doc(100);
   deserializeJson(doc, payload);
   ledState = doc["status"].as<int>();

   digitalWrite(LED, ledState);
}

void mqttSubscrbeTask(void *arg)
{
   while (!mqttClient.connected())
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
   mqttClient.setCallback(mqttcallBack);
   if (!mqttClient.subscribe(MQTT_SUB, subQoS))
   {
      loginfo("%s订阅失败\r\n", MQTT_SUB);
      vTaskDelete(NULL);
   }
   while (1)
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}

/******************************************************************************
 * 函数功能:演示Mqtt的保留消息的发布
 ******************************************************************************/
void retainedPublishTask(void *arg)
{
   while (!mqttClient.connected())
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
   String send = "{\"msg\": \"this is retained msg\" }";
   /* 发送保留消息 */
   if (!mqttClient.publish(MQTT_RetainPUB, send.c_str(), true))
   {
      loginfo("%s报文发送失败\r\n", MQTT_PUB);
   }
   while (1)
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}
/******************************************************************************
 * 函数功能: w为Mqtt提供心跳节拍和时间处理
 ******************************************************************************/
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
         connectMqttServer(clientId.c_str());
      }
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
}

/******************************************************************************
 * 函数功能: Mqtt遗嘱机制演示
 ******************************************************************************/
void lastWillTask(void *arg)
{
   while (!mqttClient.connected())
   {
      vTaskDelay(50 / portTICK_PERIOD_MS);
   }
   mqttClient.setKeepAlive(10);
   while (1)
   {
      if (!mqttClient.publish(willTopic, "esp23-online", true))
      {
         loginfo("%s报文发送失败\r\n", willTopic);
      }
      vTaskDelay(5000 / portTICK_PERIOD_MS);
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

   xTaskCreatePinnedToCore(mqttLoop, "mqttLoop", 1024 * 4, NULL, 5, NULL, 1);
   xTaskCreatePinnedToCore(mqttPublishTask, "mqttPublishTask", 1024 * 4, NULL, 4, NULL, 1);
   xTaskCreatePinnedToCore(retainedPublishTask, "retainedPublishTask", 1024 * 4, NULL, 4, NULL, 1);
   xTaskCreatePinnedToCore(mqttSubscrbeTask, "mqttSubscrbeTask", 1024 * 4, NULL, 5, NULL, 1);
   xTaskCreatePinnedToCore(lastWillTask, "lastWillTask", 1024 * 4, NULL, 5, NULL, 1);
   vTaskDelete(NULL);
}

void loop(void)
{
}