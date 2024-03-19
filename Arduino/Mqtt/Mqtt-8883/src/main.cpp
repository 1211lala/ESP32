#include "main.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#define MQTT_URL "v392e1e3.ala.cn-hangzhou.emqxsl.cn"
#define MQTT_PORT 8883
#define MQTT_PUB "liuao/ledStatus"
#define MQTT_SUB "liuao/ledSet"
#define MQTT_NAME "liuao"
#define MQTT_PWD "121314"

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

const char *ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";
String clientId = "";
bool ledState = 0;

void connectMqttServer(const char *clientId)
{
   while (!mqttClient.connected())
   {
      if (mqttClient.connect(clientId, MQTT_NAME, MQTT_PWD))
      {
         mqttClient.subscribe(MQTT_SUB);
         mqttClient.publish(MQTT_PUB, "Hi EMQX I'm ESP32 ^^"); 
         loginfo("mqtt Server: %s 连接成功.   clientId = %s\r\n", MQTT_URL, clientId);
      }
      else
      {
         loginfo("mqtt Server: %s 连接失败.    clientId = %s\r\n", MQTT_URL, clientId);
         loginfo("错误码: %d\r\n", mqttClient.state());
      }
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
      if (millis() - lastMills >= 4000)
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
 * 函数功能:演示Mqtt的订阅消息
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


void setup()
{
   pinMode(LED, OUTPUT);
   Serial.begin(115200);

   clientId = "ESP32-" + WiFi.macAddress();

   bsp_wifi_sta_init(_wc);
   espClient.setCACert(ca_cert);
   mqttClient.setServer(MQTT_URL, MQTT_PORT);
   mqttClient.setKeepAlive(60);
   mqttClient.setCallback(mqttcallBack);

   connectMqttServer(clientId.c_str());

   xTaskCreatePinnedToCore(mqttLoop, "mqttLoop", 1024 * 10, NULL, 3, NULL, 1);
   xTaskCreatePinnedToCore(mqttPublishTask, "mqttPublishTask", 1024 * 4, NULL, 4, NULL, 1);
   // xTaskCreatePinnedToCore(retainedPublishTask, "retainedPublishTask", 1024 * 4, NULL, 4, NULL, 1);
   xTaskCreatePinnedToCore(mqttSubscrbeTask, "mqttSubscrbeTask", 1024 * 4, NULL, 5, NULL, 1);
   // xTaskCreatePinnedToCore(lastWillTask, "lastWillTask", 1024 * 4, NULL, 5, NULL, 1);
   vTaskDelete(NULL);
}

void loop(void)
{
}