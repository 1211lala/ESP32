#include "ws2812.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 48, NEO_GRB + NEO_KHZ800);

void bsp_ws2812_init(uint8_t setBright)
{
    strip.begin();
    strip.clear();
    strip.setBrightness(setBright);
    strip.setPixelColor(0, strip.Color(random(0, 255), random(0, 255), random(0, 255)));
    strip.show();
}



void bsp_ws2812_shift(void)
{
    uint8_t r, g, b;
    r = random(0, 255);
    g = random(0, 255);
    b = random(0, 255);
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.show();
}
