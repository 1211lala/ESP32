#include "soc/rtc_wdt.h"    //设置看门狗用

rtc_wdt_protect_off();      //看门狗写保护关闭 关闭后可以喂狗
rtc_wdt_protect_on();       //看门狗写保护打开 打开后不能喂狗
rtc_wdt_disable();          //禁用看门狗
rtc_wdt_enable();           //启用看门狗
rtc_wdt_set_time(RTC_WDT_STAGE0, 8000); // 设置看门狗超时 8000ms.则reset重启
rtc_wdt_feed();             //喂狗函数



修改修改分区表参考
https://blog.csdn.net/wojueburenshu/article/details/117407643

分区表参考
https://blog.csdn.net/yake827/article/details/100081008

LittleFS参考
https://randomnerdtutorials.com/esp32-write-data-littlefs-arduino/
https://lingshunlab.com/book/esp32/esp32-filesystem-littlefs-fatfs-spiffs-operation-file-read-write-delete-and-list#LITTLEFSbegin


http参考
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-server/pin-control/
视频
https://www.bilibili.com/video/BV1L7411c7jw/?p=6&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=582815cfc933bf06a04f3f29f43d7977