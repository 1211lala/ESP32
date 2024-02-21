#ifndef _P_I2C_SHT20_H_
#define _P_I2C_SHT20_H_

#include "common.h"

#include "driver/i2c.h"

void sht20_init(uint32_t freq);
uint8_t sht20_get_temp(float *temp);
#endif