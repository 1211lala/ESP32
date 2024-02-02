#include "key_cfg.h"

RemoteControl rc;

void _rank(int *array, int len)
{
    /* 总轮数=元素个数-1 */
    for (int i = 1; i < len; i++)
    {
        /* j代表每轮排序次数，次数=个数-轮数-1，但j初值为0 */
        for (int j = 0; j < len - i; j++)
        {
            /* 如果前一项比后一项大，则两项的值互换 */
            if (array[j] > array[j + 1])
            {
                /* temp为数值交换时使用的临时变量 */
                int temp;
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

RemoteControl::RemoteControl(void)
{
    /* 按键引脚初始化 */
    
    pinMode(LED, OUTPUT);
    pinMode(LEFT_ANALOG_BUTTON, INPUT | PULLUP);
    pinMode(RIGHT_ANALOG_BUTTON, INPUT | PULLUP);
    pinMode(LEFT_BED_BUTTON, INPUT | PULLUP);
    pinMode(MIDDLE_BED_BUTTON, INPUT | PULLUP);
    pinMode(RIGHT_BED_BUTTON, INPUT | PULLUP);
    /* 模拟量引脚初始化 */
    pinMode(LEFT_X_ANALOG_PIN, INPUT);
    pinMode(LEFT_Y_ANALOG_PIN, INPUT);
    pinMode(RIGHT_X_ANALOG_PIN, INPUT);
    pinMode(RIGHT_Y_ANALOG_PIN, INPUT);
}

RemoteControl::~RemoteControl()
{
}

int RemoteControl::get_left_x(void)
{
    int temp[10];
    for (int8_t i = 0; i < 10; i++)
    {
        temp[i] = analogRead(LEFT_X_ANALOG_PIN);
    }
    _rank(temp, 10);

    return (int)((float)(temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7]) / 6);
}
int RemoteControl::get_left_y(void)
{
    int temp[10];
    for (int8_t i = 0; i < 10; i++)
    {
        temp[i] = analogRead(LEFT_Y_ANALOG_PIN);
    }
    _rank(temp, 10);

    return (int)((float)(temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7]) / 6);
}

int RemoteControl::get_right_x(void)
{
    int temp[10];
    for (int8_t i = 0; i < 10; i++)
    {
        temp[i] = analogRead(RIGHT_X_ANALOG_PIN);
    }
    _rank(temp, 10);

    return (int)((float)(temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7]) / 6);
}

int RemoteControl::get_right_y(void)
{
    int temp[10];
    for (int8_t i = 0; i < 10; i++)
    {
        temp[i] = analogRead(RIGHT_Y_ANALOG_PIN);
    }
    _rank(temp, 10);

    return (int)((float)(temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7]) / 6);
}

void RemoteControl::ledOn(void)
{
    digitalWrite(LED, HIGH);
}
void RemoteControl::ledOff(void)
{
    digitalWrite(LED, LOW);
}
void RemoteControl::ledToogle(void)
{
    digitalWrite(LED, !digitalRead(LED));
}
void correct_def_analog_value(struct Control_Param &cp)
{
    cp.ly = rc.get_left_y();
    cp.rx = rc.get_right_x();

    cp.lyu = (4095 - cp.ly) / 100.0;
    cp.lyd = (cp.ly - 0) / 100.0;

    cp.rxl = (4095 - cp.rx) / 100.0;
    cp.rxr = (cp.rx - 0) / 100.0;

#if 1
    printf("ly = %d\r\n", cp.ly);
    printf("rx = %d\r\n", cp.rx);
    printf("lyu = %d\r\n", cp.lyu);
    printf("lyd = %d\r\n", cp.lyd);
    printf("rxl = %d\r\n", cp.rxl);
    printf("rxr = %d\r\n", cp.rxr);
#endif
}