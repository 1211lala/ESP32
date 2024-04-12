#ifndef _FATFS_SD_H_
#define _FATFS_SD_H_

#include "common.h"
#include "FS.h"
#include "ff.h"
#include "SD.h"
#include "SPI.h"

#define SD_SCK 48
#define SD_MISO 41
#define SD_MOSI 47
#define SD_CS 42

int sd_mount_fatfs(void);

int isexist(const char *path);
int fatfs_write(const char *path, char *buffer, uint32_t size);
int fatfs_read(const char *path, char **buffer, uint32_t max_size);

void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
#endif // !_FATFS_SD_H_