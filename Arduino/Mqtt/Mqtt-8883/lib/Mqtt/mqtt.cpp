#include "mqtt.h"

#define MQTT_PORT 8883
#define MQTT_URL "v392e1e3.ala.cn-hangzhou.emqxsl.cn"
#define MQTT_NAME "liuao"
#define MQTT_PWD "121314"
#define MTQQ_KEEPALIVE 60
#define MQTT_PUB "liuao/ledStatus"
#define MQTT_SUB "liuao/ledSet"

typedef void (*MqttCallback)(char *topic, uint8_t *payload, unsigned int len);

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

String clientId = "";

void mqtt_clinet_init(MqttCallback mqttcallback)
{
    while (!WiFi.isConnected())
    {
        loginfo("WiFi未连接\r\n");
        vTaskDelay(1000);
    }
    espClient.setCACert(ca_cert);
    mqttClient.setServer(MQTT_URL, MQTT_PORT);
    mqttClient.setKeepAlive(MTQQ_KEEPALIVE);
    mqttClient.setCallback(mqttcallback);

    clientId = "ESP32-" + WiFi.macAddress();
    connectMqttServer(clientId.c_str());

    /* */
}

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

int mqttPublishTopic(const char *topic, const char *msg, bool retained)
{
    int error = mqttClient.publish(topic, msg, retained);
    if (!error)
    {
        loginfo("%s报文发送失败\r\n", MQTT_PUB);
        return -1;
    }
    return 0;
}

int mqttSubscribehTopic(const char *topic, const char *msg, uint8_t qos)
{
    int error = mqttClient.publish(topic, msg, retained);
    if (!error)
    {
        loginfo("%s报文发送失败\r\n", MQTT_PUB);
        return -1;
    }
    return 0;
}