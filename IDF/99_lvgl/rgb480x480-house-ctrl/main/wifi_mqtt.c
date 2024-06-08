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

void mqtt_connect(bool reconnect);
bool flag = 0;

void mqtt_start_task(void)
{
    mqttdev.isConnected = false;
    xTaskCreate(mqtt_task, "mqtt_task", 1024 * 4, NULL, 6, &mqttdev.taskHandle);
    vTaskSuspend(mqttdev.taskHandle);

    mqttdev.config.broker.address.uri = mqttUri;

    mqttdev.config.session.last_will.topic = mqttLastWill;
    mqttdev.config.session.last_will.msg = "offline";
    mqttdev.config.session.last_will.msg_len = strlen("offline");
    mqttdev.config.session.last_will.retain = true;
    mqttdev.config.session.last_will.qos = 1;
    mqttdev.config.session.keepalive = 20;
    mqttdev.config.credentials.username = "user-1";
    mqttdev.config.credentials.authentication.password = "user-1";
    mqtt_connect(false);
}

void mqtt_connect(bool reconnect)
{
    if (reconnect == true)
    {
        esp_mqtt_client_destroy(mqttdev.client);
    }
    mqttdev.client = esp_mqtt_client_init(&mqttdev.config);
    esp_mqtt_client_register_event(mqttdev.client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqttdev.client);
}

void mqtt_task(void *arg)
{
    while (1)
    {
        if (wp.isConnected == true) /* 未断开 */
        {
            vTaskDelay(pdMS_TO_TICKS(2000));

            if (mqttdev.isConnected == true)
            {
                esp_mqtt_client_enqueue(mqttdev.client, mqttLoop, "loop", 0, 0, 0, true);
            }

            if (mqttdev.isConnected == false)
            {
                vTaskDelay(pdMS_TO_TICKS(2000));
                mqtt_connect(true);
            }
        }
        else /* wifi断开 */
        {
            if (flag == 0)
            {
                flag = 1;
                esp_mqtt_client_destroy(mqttdev.client);
            }
            if (wp.isConnected == true)
            {
                ESP_LOGE("mqtt", "WIFi断开\r\n");
                mqtt_connect(false);
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        mqttdev.isConnected = true;
        flag = 0;
        ESP_LOGI(TAG, "mqtt服务端连接成功\r\n");

        vTaskResume(mqttdev.taskHandle);

        esp_mqtt_client_subscribe(mqttdev.client, mqttRoomLedRead, 0);
        esp_mqtt_client_subscribe(mqttdev.client, mqttFanRead, 0);
        esp_mqtt_client_subscribe(mqttdev.client, mqttCurtainRead, 0);
        esp_mqtt_client_subscribe(mqttdev.client, mqttWcLedRead, 0);
        esp_mqtt_client_subscribe(mqttdev.client, mqttLoop, 1);
        esp_mqtt_client_publish(mqttdev.client, mqttLastWill, "online", 0, 1, 1);
        break;
    case MQTT_EVENT_DISCONNECTED:
        mqttdev.isConnected = false;
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
        mqttdev.isConnected = false;
        ESP_LOGE("mqtt", "mqtt出现错误");
        break;
    }
}