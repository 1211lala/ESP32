/**
 * 实验目的: 创建任务时携带参数，并在任务创建成功后获取参数
 * 实验现象: 在上电后,会通过串口打印参数的值
*/

#include <Arduino.h>
#include "pincfg.h"


TaskHandle_t os_handle_1;
TaskHandle_t os_handle_2;
void os_task_fun_1(void *arg);
void os_task_fun_2(void *arg);


/*参数结构体*/
struct Arg
{
    char taskName[20];
    uint8_t taskPriority;
}param;

void setup()
{
    /* 为参数赋值 */
    strcpy(param.taskName, "os_task_fun_1");
    param.taskPriority = 2;

    button_init();
    xTaskCreatePinnedToCore(os_task_fun_1, "os_task_fun_1", 1024*4,(void*)&param, 2, &os_handle_1, 1);
    xTaskCreatePinnedToCore(os_task_fun_2, "os_task_fun_2", 1024*4,NULL, 3, &os_handle_2, 1);
    vTaskDelete(NULL);
}
/********************************************************************************************
 * 任务功能: 在任务初始化时,获取携带的参数结构体
*********************************************************************************************/
void os_task_fun_1(void *args)
{
    struct Arg *arg = (struct Arg*)args;
    logoinfo("arg->taskName: %s\r\n", arg->taskName);
    logoinfo("arg->taskPriority: %d\r\n", arg->taskPriority);
    while (1)
    {
       vTaskDelay(100);
    }
}

/********************************************************************************************
 * 任务功能: 为OneButton提供tick时钟
*********************************************************************************************/
void os_task_fun_2(void *args)
{
    while(1)
    {
        button.tick();
        vTaskDelay(20);
    }
}
void loop()
{
}
