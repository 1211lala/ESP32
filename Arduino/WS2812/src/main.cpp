// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 3

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500

#define Rand() (rand() % (255 - 10) + 10)

void setup()
{

    pixels.begin();
    pixels.setBrightness(10);
}

void loop()
{
    pixels.clear();
    pixels.show();
    delay(DELAYVAL);

    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(Rand(), Rand(), Rand()));

        pixels.show();

        delay(DELAYVAL);
    }
}
