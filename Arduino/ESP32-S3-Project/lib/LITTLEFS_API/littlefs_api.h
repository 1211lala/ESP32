/*
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-12-12 13:39:47
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-12-14 09:27:28
 * @FilePath: \arduino-FreeRTOS\lib\LITTLEFS\littlefs_api.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _LITTLEFS_API_H_
#define _LITTLEFS_API_H_

#include "Arduino.h"
#include "LittleFS.h"
#include "FS.h"

#define FS_OK 0
#define FS_OPEN_ERROE -1
#define FS_READ_ERROR -2
#define FS_WRITE_ERROE -3

void littlefs_start_get_info(fs::LittleFSFS &littlefs);

int littlefs_writeFile(fs::LittleFSFS &littlefs, const char *path, const char *message);
int littlefs_readFile(fs::LittleFSFS &littlefs, const char *path, char *rxbuf);
String littlefs_readfile(fs::LittleFSFS &littlefs, const char *path);
int littlefs_appendFile(fs::LittleFSFS &littlefs, const char *path, const char *message);
void littlefs_scanFile(fs::LittleFSFS &littlefs, const char *path);
void littlefs_fileTest(void);

#endif // ! _LITTLEFS_API_H_