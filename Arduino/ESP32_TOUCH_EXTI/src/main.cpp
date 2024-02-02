/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-01 08:51:29
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-17 10:37:43
 * @FilePath: \ESP32_TOUCH_EXTI\ESP32_TOUCH_EXTI\src\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <Arduino.h>

#define   LED           2
#define   TOUCH_PIN     27
#define   KEY_PIN       0

void Touch_Evevt_Callback(void);
void Key_Exti_Callback(void);

void setup() 
{
  Serial.begin(115200);
  Serial.printf("Touch_PAD Mapping to GPIO4\r\n");
  Serial.printf("Exti Mapping to GPIO0\r\n");

  pinMode(LED, OUTPUT);

  /* 绑定事件引脚 */
  touchAttachInterrupt(TOUCH_PIN, Touch_Evevt_Callback, 20);

  /* 绑定中断引脚 */
  pinMode(KEY_PIN, INPUT_PULLUP);
  attachInterrupt(KEY_PIN, Key_Exti_Callback, FALLING);
}

void Touch_Evevt_Callback(void)
{
  Serial.printf("touch_num %3d\r\n", touchRead(TOUCH_PIN));
} 


void Key_Exti_Callback(void)
{
  Serial.printf("key_exti\r\n");
}


void loop() 
{
  digitalWrite(LED, !digitalRead(LED));
  delay(1000);
}
