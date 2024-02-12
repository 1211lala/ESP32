#ifndef _WS2812_H_
#define _WS2812_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

extern Adafruit_NeoPixel strip;

void bsp_ws2812_init(uint8_t setBright);
void bsp_ws2812_shift(void);

#endif // !_WS2812_H_