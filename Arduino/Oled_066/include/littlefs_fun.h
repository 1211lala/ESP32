
#ifndef _LITTLEFS_FUN_H_
#define _LITTLEFS_FUN_H_

#include "Arduino.h"
#include <ArduinoJson.h>
#include "LittleFS.h"
#include "FS.h"

#include "common.h"


void fs_mount(void);
int fs_write( const char *path, const char *message);
int fs_append( const char *path, const char *message);
String fs_read( const char *path);
void fs_scanfile( const char *path);
void fs_test(void);

int write_fs_config(_wifi_config &config);
int read_fs_config(_wifi_config &config);

#endif // ! _LITTLEFS_API_H_