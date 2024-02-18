#ifndef _LEDPIN_H_
#define _LEDPIN_H_


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define ledPin 2

void led_init(void);

#endif