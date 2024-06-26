#include "common.h"

void loginfo(const char *format, ...)
{
    char buff[1024] = {};
    va_list list;
    va_start(list, format);
    vsnprintf(buff, 1024, format, list);
    printf("[%.3f] : %s", (float)millis() / 1000.0, buff);
}