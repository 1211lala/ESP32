#include "key.h"

void bsp_bootkey_init(void)
{
    /*
        SWD --- GPIO0
    */
    pinMode(BOOT_KEY, INPUT_PULLUP);
}



uint8_t get_key_value(uint16_t checktime)
{
    static uint64_t tick = 0;
    static uint8_t flag = 0;
    uint8_t key_status = 0;

    key_status = digitalRead(BOOT_KEY);
    if (flag == 0 && (key_status == DOWN_STATUS))
    {
        flag = 1;
        tick = millis(); /* 保存当前值 单位ms uint64_t */
    }
    if(flag == 1 &&  (millis() - tick ) > checktime)
	{
		if(flag == 1  && key_status == DOWN_STATUS )
		{
			flag = 2;
            return down;
		}
	}
    if(key_status == UP_STATUS)
	{
		flag = 0;
	}
    return up;
}