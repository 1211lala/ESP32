/**
 * 免费的Mqtt服务器地址 "mqtt://mqtt.eclipseprojects.io"
 *
 * mqtx软件下载连接 https://www.emqx.io/zh/downloads?os=Windows
 *
 *
 *
 */

#include "wifi_mqtt.h"
#include "lvgl.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include <sys/param.h>
#include "mqtt_client.h"
struct mqtt_dev mqttdev;

static const char *TAG = "mqtt";

void mqtt_start_task(void)
{
    xTaskCreate(mqtt_task, "mqtt_task", 1024 * 4, NULL, 5, &mqttdev.taskHandle);
    vTaskSuspend(mqttdev.taskHandle);

    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = mqttUri;

    mqtt_cfg.session.last_will.topic = mqttLastWill;
    mqtt_cfg.session.last_will.msg = "offline";
    mqtt_cfg.session.last_will.msg_len = strlen("offline");
    mqtt_cfg.session.last_will.retain = true;
    mqtt_cfg.session.last_will.qos = 1;
    mqtt_cfg.session.keepalive = 20;

    mqttdev.client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqttdev.client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqttdev.client);
}




void mqtt_reconnect(void)
{

    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = mqttUri;

    mqtt_cfg.session.last_will.topic = mqttLastWill;
    mqtt_cfg.session.last_will.msg = "offline";
    mqtt_cfg.session.last_will.msg_len = strlen("offline");
    mqtt_cfg.session.last_will.retain = true;
    mqtt_cfg.session.last_will.qos = 1;
    mqtt_cfg.session.keepalive = 20;

    mqttdev.client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqttdev.client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqttdev.client);

    vTaskDelay(pdMS_TO_TICKS(500));
    esp_mqtt_client_subscribe(mqttdev.client, mqttRoomLedRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttFanRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttCurtainRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttWcLedRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttLoop, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_publish(mqttdev.client, mqttLastWill, "online", 0, 1, 1);
}

void mqtt_task(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(500));
    esp_mqtt_client_subscribe(mqttdev.client, mqttRoomLedRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttFanRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttCurtainRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttWcLedRead, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_subscribe(mqttdev.client, mqttLoop, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_mqtt_client_publish(mqttdev.client, mqttLastWill, "online", 0, 1, 1);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_mqtt_client_enqueue(mqttdev.client, mqttLoop, "loop", 0, 0, 0, true);
    }
}
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "mqtt服务端连接成功\r\n");
        vTaskResume(mqttdev.taskHandle);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "mqtt断开连接成功\r\n");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "mqtt订阅成功\r\n");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "mqtt取消订阅成功\r\n");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "mqtt发布成功\r\n");
        break;
    case MQTT_EVENT_DATA:

        char topic[100];
        char data[100];
        memcpy(topic, event->topic, event->topic_len);
        memcpy(data, event->data, event->data_len);
        topic[event->topic_len] = '\0';
        data[event->data_len] = '\0';
        printf("topic: %s  data: %s\r\n", topic, data);
        if (use_lvgl == true)
        {
            if (0 == strcmp(topic, mqttRoomLedRead))
            {
                if (strcmp(data, "on") == 0)
                {
                    lv_label_set_text_fmt(label00, "ON");
                    lv_obj_add_state(switch0, LV_STATE_CHECKED);
                }
                if (strcmp(data, "off") == 0)
                {
                    lv_label_set_text_fmt(label00, "OFF");
                    lv_obj_clear_state(switch0, LV_STATE_CHECKED);
                }
            }
            else if (0 == strcmp(topic, mqttFanRead))
            {
                if (strcmp(data, "on") == 0)
                {
                    lv_label_set_text_fmt(label11, "ON");
                    lv_obj_add_state(switch1, LV_STATE_CHECKED);
                }
                if (strcmp(data, "off") == 0)
                {
                    lv_label_set_text_fmt(label11, "OFF");
                    lv_obj_clear_state(switch1, LV_STATE_CHECKED);
                }
            }
            else if (0 == strcmp(topic, mqttCurtainRead))
            {
                if (strcmp(data, "on") == 0)
                {
                    lv_label_set_text_fmt(label22, "ON");
                    lv_obj_add_state(switch2, LV_STATE_CHECKED);
                }
                if (strcmp(data, "off") == 0)
                {
                    lv_label_set_text_fmt(label22, "OFF");
                    lv_obj_clear_state(switch2, LV_STATE_CHECKED);
                }
            }
            else if (0 == strcmp(topic, mqttWcLedRead))
            {
                if (strcmp(data, "on") == 0)
                {
                    lv_label_set_text_fmt(label33, "ON");
                    lv_obj_add_state(switch3, LV_STATE_CHECKED);
                }
                if (strcmp(data, "off") == 0)
                {
                    lv_label_set_text_fmt(label33, "OFF");
                    lv_obj_clear_state(switch3, LV_STATE_CHECKED);
                }
            }
        }

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE("mqtt", "mqtt出现错误");
        break;
    }
}