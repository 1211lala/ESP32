#ifndef _PINCFG_H_
#define _PINCFG_H_

#include "Arduino.h"

#define LED 2
#define KEY 13


void led_init(void);
void button_init(void);
void logoinfo(const char *format, ...);


#endif