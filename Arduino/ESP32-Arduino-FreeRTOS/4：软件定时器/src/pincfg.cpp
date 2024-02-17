#include "pincfg.h"

OneButton button(KEY, true);

void button_attachClick(void)
{
}


void button_init(void)
{
    pinMode(KEY, INPUT|PULLUP);
    button.attachClick(button_attachClick);
}

void led_init(void)
{
    pinMode(LED, OUTPUT|PULLUP);
    digitalWrite(LED, HIGH);
}




void logoinfo(const char *format, ...)
{
    char buff[1024] = {};
    va_list list;
    va_start(list, format);
    vsnprintf(buff, 1024, format, list);
    printf("[INFO] [%.3f] : %s", (float)millis() / 1000.0, buff);
}

