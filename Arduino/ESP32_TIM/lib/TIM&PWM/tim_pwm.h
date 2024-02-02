#ifndef __TIM_PWM_H__
#define __TIM_PWM_H__

#include <Arduino.h>


void PWM_Init(void);
void PWM_SetDuty( uint16_t channel, uint16_t duty);

extern const uint8_t  PWM_Channel_A;

#endif