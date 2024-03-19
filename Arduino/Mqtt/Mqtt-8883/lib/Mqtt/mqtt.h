#ifndef _MQTT_H_
#define _MQTT_H_

#include "common.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>


#define MQTT_PORT 8883
#define MQTT_URL "v392e1e3.ala.cn-hangzhou.emqxsl.cn"
#define MQTT_NAME "liuao"
#define MQTT_PWD "121314"
#define MTQQ_KEEPALIVE 60
#define MQTT_PUB "liuao/ledStatus"
#define MQTT_SUB "liuao/ledSet"
#define MQTT_CLIENTID   "ESP32-Client-1"


extern PubSubClient mqttClient;

typedef void (*MqttCallback)(char *topic, uint8_t *payload, unsigned int len);
void mqtt_clinet_init(MqttCallback callback);
void connectMqttServer(const char *clientId);
int mqttSubscribehTopic(const char *topic, uint8_t qos);
int mqttPublishTopic(const char *topic, const char *msg, bool retained);
#endif