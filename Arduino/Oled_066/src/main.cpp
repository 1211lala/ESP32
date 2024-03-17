#include "main.h"

TaskHandle_t wifi_Handle = NULL;
TaskHandle_t key_Handle = NULL;
TaskHandle_t oled_Handle = NULL;

void setup()
{
   Serial.begin(115200);
   fs_mount();
   read_fs_config(_wc);
   // xTaskCreatePinnedToCore(start_wifi_webserver, "start_wifi_webserver", 1024 * 4, NULL, 4, &wifi_Handle, 1);
   xTaskCreatePinnedToCore(oled_show, "oled_show", 1024 * 5, NULL, 3, &oled_Handle, 1);
   xTaskCreatePinnedToCore(get_key_value, "get_key_value", 1024 * 4, NULL, 5, &key_Handle, 1);
   vTaskDelete(NULL);
}

/********************************************************************************************************************
 * 函数功能 : 读取配置文件中的数据、连接WiFi、开启web服务器,当 MIDDLE_BED_BUTTON 为低电平时开启AP模式,否则开启STA模式
 ********************************************************************************************************************/
void start_wifi_webserver(void *arg)
{
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
void task1_fun(void)
{
   logoinfo("task1_fun\r\n");
}
void task2_fun(void)
{
   logoinfo("task2_fun\r\n");
}
void task3_fun(void)
{
   logoinfo("task3_fun\r\n");
}
void task4_fun(void)
{
   logoinfo("task4_fun\r\n");
}
void task5_fun(void)
{
   logoinfo("task5_fun\r\n");
}
void task6_fun(void)
{
   logoinfo("task6_fun\r\n");
}
void task7_fun(void)
{
   logoinfo("task7_fun\r\n");
}

/* 正在被选中的下标的数组下标*/
int Catch_i = 0;
/* 正在显示的最小下标 */
int Show_i = 0;
/* 当前光标位置 在屏幕中的行数 */
int Cursor_i = 0;
void oled_show(void *arg)
{

   Oled_Init(0x00);
   struct _task_list tasklist[10];
   tasklist[0].name = "<<<";
   tasklist[1].name = "task1";
   tasklist[2].name = "task2";
   tasklist[3].name = "task3";
   tasklist[4].name = "task4";
   tasklist[5].name = "task5";
   tasklist[6].name = "task6";
   tasklist[7].name = "task7";
   tasklist[8].name = "...";
   tasklist[1].taskFun = task1_fun;
   tasklist[2].taskFun = task2_fun;
   tasklist[3].taskFun = task3_fun;
   tasklist[4].taskFun = task4_fun;
   tasklist[5].taskFun = task5_fun;
   tasklist[6].taskFun = task6_fun;
   tasklist[7].taskFun = task7_fun;

   int task_num = 0;

   while (tasklist[task_num].name != "...")
   {
      logoinfo("[%d] = %s\r\n", task_num, tasklist[task_num].name.c_str());
      task_num += 1;
   }
   logoinfo("共有%d个任务\r\n", task_num + 1);

   task_num = task_num + 1;

   for (int i = 0; i < task_num; i++)
   {
      OLED_Show_String(0, i * 10, (uint8_t *)tasklist[i].name.c_str(), 8, 1);
   }
   OLED_Refresh();

   /* 一页显示5行*/
   int show_lines = 5;
   while (1)
   {
      if (Catch_i < 0)
         Cursor_i = 0;
      if (Cursor_i > task_num)
         Cursor_i = task_num;

      if (Cursor_i < 0)
         Cursor_i = 0;
      if (Cursor_i > 5)
         Cursor_i = 5;
      if(Cursor_i > task_num) Cursor_i = task_num;
      
      Show_i = Catch_i - Cursor_i;

      OLED_Clear(0x00);
      for (int i = 0; i < 5; i++)
      {
         OLED_Show_String(0, i * 10, (uint8_t *)tasklist[Show_i + i].name.c_str(), 8, 1);
      }

      OLED_Show_String(0, Cursor_i * 10, (uint8_t *)"#######", 8, 1);

      logoinfo("Show_i = %d\r\n", Show_i);
      OLED_Refresh();
      vTaskDelay(50);
   }
}

/********************************************************************************************************************
 * 函数功能 : 绑定按键
 ********************************************************************************************************************/
void lmb_button_attachClick(void *oneButton)
{
   Catch_i += 1;
   Cursor_i += 1;
}
void lmb_button_attachDoubleClick(void *oneButton)
{
   Catch_i -= 1;
   Cursor_i -= 1;
}
void get_key_value(void *arg)
{
   pinMode(LEFT_MUTE_BUTTON, INPUT | PULLUP);
   OneButton lmb_button(LEFT_MUTE_BUTTON, true);

   lmb_button.attachClick(lmb_button_attachClick, &lmb_button);
   lmb_button.attachDoubleClick(lmb_button_attachDoubleClick, &lmb_button);
   lmb_button.setClickMs(400);
   while (1)
   {
      lmb_button.tick();
      vTaskDelay(20);
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

void loop()
{
}
