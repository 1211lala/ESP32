#ifndef _COMMON_H_
#define _COMMON_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
using namespace std;


#define LED 2 /* 和核心板上的LED同一引脚 */
#define KEY 0 /* 和核心板上的LED同一引脚 */

void ledOn(void);
void ledOff(void);
void ledToogle(void);
void loginfo(const char *format, ...);

#endif // !_COMMON_H_
