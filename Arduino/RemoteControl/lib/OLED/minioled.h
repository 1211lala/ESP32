#pragma once

#ifndef _MINIOLED_H_
#define _MINIOLED_H_

#include "common.h"
#include "Wire.h"

#define OLED_CMD 0x00  
#define OLED_DATA 0x40

#define lie 64
#define hang 48

void OLED_WR_Byte(uint8_t dat, uint8_t mode);


void Oled_Init(uint8_t mode);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Clear(uint8_t mode);
void OLED_Refresh(void);
void OLED_Show_Char(int x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode);
void OLED_Show_String(int x, uint8_t y, uint8_t *str, uint8_t str_size, uint8_t mode);
#endif // !_OLED_H_

