#include "mqtt.h"

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

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);


void mqtt_clinet_init(MqttCallback callback)
{
    while (!WiFi.isConnected())
    {
        loginfo("WiFi未连接\r\n");
        vTaskDelay(1000);
    }
    espClient.setCACert(ca_cert);
    mqttClient.setServer(MQTT_URL, MQTT_PORT);
    mqttClient.setKeepAlive(MTQQ_KEEPALIVE);
    mqttClient.setCallback(callback);

    connectMqttServer(MQTT_CLIENTID);

    mqttSubscribehTopic(MQTT_SUB, 0);
}

void connectMqttServer(const char *clientId)
{
    while (!mqttClient.connected())
    {
        if (mqttClient.connect(clientId, MQTT_NAME, MQTT_PWD))
        {
            mqttClient.subscribe(MQTT_SUB);
            mqttClient.publish(MQTT_PUB, "Hi EMQX I'm ESP32 ^^");
            loginfo("mqtt Server: %s 连接成功.   clientId = %s\r\n", MQTT_URL, MQTT_CLIENTID);
        }
        else
        {
            loginfo("mqtt Server: %s 连接失败.    clientId = %s\r\n", MQTT_URL, MQTT_CLIENTID);
            loginfo("错误码: %d\r\n", mqttClient.state());
        }
    }
}

int mqttPublishTopic(const char *topic, const char *msg, bool retained)
{
    int error = mqttClient.publish(topic, msg, retained);
    if (!error)
    {
        loginfo("%s报文发送失败\r\n", topic);
        return -1;
    }
    return 0;
}

int mqttSubscribehTopic(const char *topic, uint8_t qos)
{
    int error = mqttClient.subscribe(topic, qos);
    if (!error)
    {
        loginfo("%s报文订阅失败\r\n", topic);
        return -1;
    }
    return 0;
}