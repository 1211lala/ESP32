/*
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-12-14 10:02:39
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-12-14 17:47:43
 * @FilePath: \K06-FireDoor-RTOS\lib\JSON\json.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _JSON_H_
#define _JSON_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "littlefs_api.h"
#include "sta.h"
#include "ap.h"


struct _wifi_config
{
    String ssd;
    String password;
    IPAddress ip;
    IPAddress getwey;
    IPAddress subnet;
};

extern _wifi_config wifi_config;

/* sta模式需要连接的ssd 或者ap模式需要开启的ssd*/
extern String ssd;
/* sta模式下的密码 或者ap模式下的密码 */
extern String password;
/* sta模式下的ip 或者ap模式下的ip */
extern IPAddress ip;
/* sta模式下的getway 或者ap模式下的getway */
extern IPAddress getway;
/* sta模式下的subnet 或者ap模式下的subnet */
extern IPAddress subnet;
/* 设置的tcp端口 */
extern uint32_t tcp_port;
/* 防火门状态 */
extern uint8_t door_status;
/* 防火门ID */
extern uint16_t door_id;
/* 防火门张开角度 */
extern uint16_t door_angle;

/* 参数获取命令 */
#define get_para_cmd 0x00
/* 参数配置命令 */
#define set_para_cmd 0x01
/* 获取状态命令 */
#define get_status_cmd 0x02
/* 设置状态命令 */
#define set_control_cmd 0x03

/* 防火门状态 */
#define open 1
#define close 0

void json_set_wifi_configs(uint8_t esp32_mode);
IPAddress ipaddress_ascll_to_int(const char *ipbuf);
void write_wifi_config(_wifi_config config);
#endif