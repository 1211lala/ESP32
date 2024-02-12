#include <Arduino.h>
#include "ws2812.h"
#include "key.h"
#include "littlefs_api.h"
#include "sta.h"
#include "ap.h"
#include "ota.h"
#include "json.h"
#include "main.h"
#include <WebServer.h>

TaskHandle_t ws2812handle = NULL;
TaskHandle_t keyhandle = NULL;
TaskHandle_t serverhandle = NULL;


void setup()
{
  Serial.begin(115200);
  bsp_bootkey_init();
  bsp_ws2812_init(5);
  littlefs_start_get_info(LittleFS);

  wifi_config.ssd = "Xiaomi_4C";
  wifi_config.password = "121314liuAO#";
  wifi_config.ip = IPAddress(192, 168, 31, 22);
  wifi_config.getwey = IPAddress(192, 168, 31, 1);
  wifi_config.subnet = IPAddress(255, 255, 255, 0);
  write_wifi_config(wifi_config);

  // bsp_wifi_sta_init("Xiaomi_4C", "121314liuAO#", ip, getway, subnet);
  // async_elegant_ota_start();

  // /******************************************TRTOS*******************************************/
  // xTaskCreate(task_ws2812, "task_ws2812", 1024 * 2, NULL, 2, &ws2812handle);
  // xTaskCreate(task_key, "task_key", 1024 * 2, NULL, 2, &keyhandle);
  // xTaskCreate(task_server, "task_server", 1024 * 2, NULL, 2, &serverhandle);

  vTaskDelete(NULL);
}

void loop()
{
}

void task_server(void *pt)
{
  while (1)
  {
    // Server.handleClient();
    vTaskDelay(50);
  }
}

void task_ws2812(void *pt)
{
  while (1)
  {
    bsp_ws2812_shift();
    vTaskDelay(400);
  }
}

void task_key(void *pt)
{
  uint8_t flag = 0;
  while (1)
  {
    if (get_key_value(40) == down)
    {
      if (flag == 1)
      {
        flag = 0;
        vTaskResume(ws2812handle);
      }
      else if (flag == 0)
      {
        flag = 1;
        vTaskSuspend(ws2812handle);
      }
    }
    vTaskDelay(5);
  }
}
