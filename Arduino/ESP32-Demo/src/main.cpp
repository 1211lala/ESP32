#include "main.h"

TaskHandle_t get_control_value_Handle = NULL;
TaskHandle_t wifi_Handle = NULL;
TaskHandle_t get_esp_info_Handle = NULL;
TaskHandle_t ws2812_Handle = NULL;
TaskHandle_t key_Handle = NULL;
TaskHandle_t oled_Handle = NULL;

struct Control_Param def_cp;
struct Ws2812Param
{
   int count = 0;
   uint8_t r;
   uint8_t g;
   uint8_t b;
   int Color;
};

Ws2812Param wp;

void setup()
{
   Serial.begin(115200);

   xTaskCreatePinnedToCore(start_wifi_webserver, "start_wifi_webserver", 1024 * 4, NULL, 4, &wifi_Handle, 1);
   xTaskCreatePinnedToCore(oled_show, "oled_show", 1024 * 5, NULL, 3, &oled_Handle, 1);
   // xTaskCreatePinnedToCore(get_rocker_value, "get_rocker_value", 1024 * 4, NULL, 3, &get_control_value_Handle, 1);
   // xTaskCreatePinnedToCore(get_esp_info, "get_esp_info", 1024 * 4, NULL, 3, &get_esp_info_Handle, 1);
   xTaskCreatePinnedToCore(get_key_value, "get_key_value", 1024 * 4, NULL, 5, &key_Handle, 1);
   // xTaskCreatePinnedToCore(ws2812_show, "ws2812_show", 1024 * 4, NULL, 3, &ws2812_Handle, 1);
   vTaskDelete(NULL);
}

/********************************************************************************************************************
 * 函数功能 : 读取配置文件中的数据、连接WiFi、开启web服务器,当 MIDDLE_BED_BUTTON 为低电平时开启AP模式,否则开启STA模式
 ********************************************************************************************************************/
void start_wifi_webserver(void *arg)
{
   fs_mount();
   read_fs_config(_wc);
   if (digitalRead(MIDDLE_BED_BUTTON) == LOW)
   {
      bsp_wifi_ap_init(ap_ssid, ap_password, ap_ip, ap_gateway, ap_subnet);
      rc.ledOn();
      web_server_init();
      vTaskSuspend(NULL);
   }
   else
   {
      bsp_wifi_sta_init(_wc);
   }
   web_server_init();
   while (1)
   {
      if (WiFi.isConnected())
      {
         rc.ledOn();
      }
      else
      {
         rc.ledOff();
      }
      vTaskDelay(100);
   }
}

/********************************************************************************************************************
 * 函数功能 : oled显示
 ********************************************************************************************************************/

uint8_t get_center_x(const char *buf, uint8_t font_size)
{
   uint8_t size = 0;
   if (font_size == 8)
      size = 6;
   else
      size = font_size / 2;

   if ((strlen(buf) * font_size) > lie)
      return 0;
   else
      return (lie - (strlen(buf) * font_size)) / 2;
}

float d0 = 0, d1 = 0;
void oled_show(void *arg)
{
   Oled_Init(0x00);
   while (1)
   {
      d0 += (d1 - d0) / 8;

      OLED_Clear(0x00);
      OLED_Show_String(get_center_x(_wc.ssid.c_str(), 8), 0, (uint8_t *)_wc.ssid.c_str(), 8, 1);
      OLED_Show_String(d0++, 10, (uint8_t *)_wc.ip.c_str(), 8, 1);
      OLED_Show_String(get_center_x(_wc.port.c_str(), 8), 20, (uint8_t *)_wc.port.c_str(), 8, 1);
      OLED_Refresh();
      vTaskDelay(20);
      logoinfo("%f\r\n", d0);
   }
}

/********************************************************************************************************************
 * 函数功能 : 得到电位器的值,并转换成百分比
 ********************************************************************************************************************/
void get_rocker_value(void *arg)
{
   uint8_t waittime = 20;
   correct_def_analog_value(def_cp);

   while (1)
   {
      if (rc.get_left_y() > (def_cp.ly + def_cp.lyu * 10))
      {
         wp.count = ((rc.get_left_y() - def_cp.ly - def_cp.lyu * 10) / def_cp.lyu) / 5;
         logoinfo("前:%d\r\n", (rc.get_left_y() - def_cp.ly - def_cp.lyu * 10) / def_cp.lyu);
      }
      else
      {
         wp.count = 0;
      }
      if (rc.get_left_y() < (def_cp.ly - def_cp.lyd * 10))
      {
         logoinfo("后:%d\r\n", (rc.get_left_y() - def_cp.ly + def_cp.lyd * 10) / def_cp.lyd);
      }
      if (rc.get_right_x() > (def_cp.rx + def_cp.rxl * 10))
      {
         wp.Color = rc.get_right_x() * 4096;
         logoinfo("左:%d\r\n", (rc.get_right_x() - def_cp.rx - def_cp.rxl * 10) / def_cp.rxl);
      }
      if (rc.get_right_x() < (def_cp.rx - def_cp.rxl * 10))
      {
         logoinfo("右:%d\r\n", (def_cp.rx - rc.get_right_x() - def_cp.rxl * 10) / def_cp.rxr);
      }
      vTaskDelay(waittime);
   }
}

/********************************************************************************************************************
 * 函数功能 : 通过摇杆的值控制灯珠
 ********************************************************************************************************************/
Adafruit_NeoPixel strip = Adafruit_NeoPixel(CoUNT, PIN, NEO_GRB + NEO_KHZ800);
void ws2812_show(void *arg)
{
   int waittime = 20;

   strip.begin();
   strip.setBrightness(50);
   strip.show();
   while (1)
   {
      if (wp.count > 20)
         wp.count = 20;

      for (int i = 0; i < wp.count; i++)
      {
         strip.setPixelColor(i, strip.Color(100, 0, 0));
      }
      for (int i = wp.count; i < CoUNT; i++)
      {
         strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
      vTaskDelay(10);
   }
}

/********************************************************************************************************************
 * 函数功能 :
 ********************************************************************************************************************/
void lmb_button_attachClick(void *oneButton)
{
   static int flag = 0;
   if (flag == 0)
   {
      d0 = 0;
      d1 = 40;
      flag = 1;
   }
   else if (flag == 1)
   {
      d0 = d1;
      d1 = -20;
      flag = 0;
   }
   // vTaskResume(get_esp_info_Handle);
}

void get_key_value(void *arg)
{
   pinMode(LEFT_MUTE_BUTTON, INPUT | PULLUP);
   OneButton lmb_button(LEFT_MUTE_BUTTON, true);

   lmb_button.attachClick(lmb_button_attachClick, &lmb_button);
   lmb_button.setClickMs(400);
   while (1)
   {
      lmb_button.tick();
      vTaskDelay(20);
   }
}
/********************************************************************************************************************
 * 函数功能 : 串口打印esp的信息
 ********************************************************************************************************************/
void get_esp_info(void *arg)
{

   uint64_t chipid;
   while (1)
   {

      chipid = ESP.getEfuseMac();

      logoinfo("ESP32 Chip ID = %02x%02x%02x%02x%02x%02x\r\n", chipid >> 56, chipid >> 48, chipid >> 40, chipid >> 32, chipid >> 24, chipid >> 16, chipid >> 8, chipid);
      logoinfo("SDK版本 : %s\r\n", ESP.getSdkVersion());
      logoinfo("芯片版本 : %d\r\n", ESP.getChipRevision());
      logoinfo("MD5 : %s\r\n", ESP.getSketchMD5().c_str());
      logoinfo("芯片频率 : %dMhz\r\n", ESP.getCpuFreqMHz());
      switch (ESP.getFlashChipMode())
      {
         //    DOUT:地址为 1 线模式输入，数据为 2 线模式输出
         //    DIO：地址为 2 线模式输入，数据为 2 线模式输出
         //    QOUT：地址为 1 线模式输入，数据为 4 线模式输出
         //    QIO：地址为 4 线模式输入，数据为 4 线模式输出
         //    用户如果需要使用 QIO 模式，则需要在选择 Flash 时确认该 Flash 是否支持 QIO 模式。
      case (0):
         logoinfo("Flash模式 : FM_QIO\r\n");
         break;
      case (1):
         logoinfo("Flash模式 : FM_QOUT\r\n");
         break;
      case (2):
         logoinfo("Flash模式 : FM_DIO\r\n");
         break;
      case (3):
         logoinfo("Flash模式 : FM_DOUT\r\n");
         break;
      case (4):
         logoinfo("Flash模式 : FM_FAST_READ\r\n");
         break;
      case (5):
         logoinfo("Flash模式 : FM_SLOW_READ\r\n");
         break;
      case (0xff):
         logoinfo("Flash模式 : FM_UNKNOWN\r\n");
         break;
      }
      logoinfo("Flash速度 : %dMhz\r\n", ESP.getFlashChipSpeed() / 1000000);
      logoinfo("Flash容量 : %dM\r\n", ESP.getFlashChipSize() / 1024 / 1024);
      logoinfo("OTA容量 : %dK\r\n", ESP.getFreeSketchSpace() / 1024);
      logoinfo("已用Flash : %dK\r\n", ESP.getSketchSize() / 1024);

      logoinfo("Sram容量 : %dK\r\n", ESP.getHeapSize() / 1024);
      logoinfo("可用Sram : %dK\r\n", ESP.getFreeHeap() / 1024);
      logoinfo("最大堆分配 : %dK\r\n", ESP.getMaxAllocHeap() / 1024);
      logoinfo("最小空闲堆 : %dK\r\n", ESP.getMinFreeHeap() / 1024);

      logoinfo("Psram容量 : %dK\r\n", ESP.getPsramSize() / 1024);
      logoinfo("可用Psram : %dK\r\n", ESP.getFreePsram() / 1024);
      logoinfo("最大Psram分配 : %dK\r\n", ESP.getMaxAllocPsram() / 1024);
      logoinfo("最小空闲Psram : %dK\r\n\r\n", ESP.getMinFreePsram() / 1024);

      vTaskSuspend(NULL);
   }
}

void loop()
{
}
