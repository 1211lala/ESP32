#ifndef _COMMON_H_
#define _COMMON_H_

#include <Arduino.h>
#include <vector>
using namespace std;

/* 遥控器下方的静音按键 */
#define LEFT_MUTE_BUTTON 13
#define LED 2 /* 和核心板上的LED同一引脚 */
/* 摇杆上的按键 */
#define LEFT_ANALOG_BUTTON 5
#define RIGHT_ANALOG_BUTTON 4
#define LEFT_X_ANALOG_PIN 34
#define LEFT_Y_ANALOG_PIN 35
#define RIGHT_X_ANALOG_PIN 36
#define RIGHT_Y_ANALOG_PIN 32
/* 遥控器前方的弯脚拨动开关 */
#define LEFT_BED_BUTTON 19
#define MIDDLE_BED_BUTTON 23
#define RIGHT_BED_BUTTON 25

#define PIN 17
#define CoUNT 20

#define FS_OK 0
#define FS_OPEN_ERROE -1
#define FS_READ_ERROR -2
#define FS_WRITE_ERROE -3

#define WIFI_OPEN 1
#define WIFI_CLOSE 0

#define ap_ssid "ESP32-WROOME-Mini"
#define ap_password "123456789"

#define ap_ip IPAddress(192, 168, 1, 2)
#define ap_gateway IPAddress(192, 168, 1, 1)
#define ap_subnet IPAddress(255, 255, 255, 0)
#define ap_dns IPAddress(8, 8, 8, 8)

#define WIFI_CONFIG_PATH "/wifi_config/config.json"

struct _wifi_config
{
    String ssid;
    String password;
    String port;
    bool use_static_ip;
    String ip;
    String subnet;
    String gateway;
    String dns;
    String remote_ip;
    String remote_port;
};
extern _wifi_config _wc;

struct _wifi_params
{
    String ssid;
    int8_t rssi;
    int8_t channel;
    String encryption;
};


struct _task_list
{
    String name;
    void (*taskFun)(void);
    void (*showFun)(void);
};

void logoinfo(const char *format, ...);

#endif // !_COMMON_H_
