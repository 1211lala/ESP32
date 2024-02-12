<!--
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-12-09 20:09:28
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-12-09 20:10:21
 * @FilePath: \arduino-FreeRTOS\readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
#include "soc/rtc_wdt.h"    //设置看门狗用

rtc_wdt_protect_off();      //看门狗写保护关闭 关闭后可以喂狗
rtc_wdt_protect_on();       //看门狗写保护打开 打开后不能喂狗
rtc_wdt_disable();          //禁用看门狗
rtc_wdt_enable();           //启用看门狗
rtc_wdt_set_time(RTC_WDT_STAGE0, 8000); // 设置看门狗超时 8000ms.则reset重启
rtc_wdt_feed();             //喂狗函数



