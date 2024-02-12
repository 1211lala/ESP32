#ifndef _KEY_H_
#define _KEY_H_

#include <Arduino.h>

#define BOOT_KEY 0

#define DOWN_STATUS 0
#define UP_STATUS 1

#define down 0 /* 按键按下 */
#define up 1   /* 按键松开 */

void bsp_bootkey_init(void);
uint8_t get_key_value(uint16_t checktime);

#endif //  _KEY_H_