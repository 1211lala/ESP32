#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "sdkconfig.h"

#include <esp_heap_caps.h>
#include "gt911.h"
#include "st7701s.h"


void app_main(void)
{
    gt911_init(100000);
    while (1)
    {
        if(gt911_scan(0))
        {
            ESP_LOGI("tag", "x: %d  y: %d", tp.x[0], tp.y[0]);
        }
        
        vTaskDelay(1);
    }
}

// ESP_LOGW("main", "内部RAM    : %dKb", heap_caps_get_total_size(MALLOC_CAP_INTERNAL) / 1024);
// ESP_LOGW("main", "内部可用RAM : %dKb", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024);
// ESP_LOGW("main", "外部RAM    : %dKb", heap_caps_get_total_size(MALLOC_CAP_SPIRAM) / 1024);
// ESP_LOGW("main", "外部可用RAM : %dKb\r\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);