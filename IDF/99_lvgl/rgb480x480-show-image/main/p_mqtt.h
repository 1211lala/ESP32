#ifndef _p_MQTT_H_
#define _p_MQTT_H_

void mqtt_init(void);
void mqtt_task(void *arg);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
#endif // !_p_MQTT_H_