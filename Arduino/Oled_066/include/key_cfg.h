#pragma once

#include "common.h"

class RemoteControl
{
private:
public:
    int get_left_x(void);
    int get_left_y(void);
    int get_right_x(void);
    int get_right_y(void);
    void ledOn(void);
    void ledOff(void);
    void ledToogle(void);
    RemoteControl(void);
    ~RemoteControl();
};

struct Control_Param
{
    short ly;
    short rx;
    short lyu;
    short lyd;
    short rxl;
    short rxr;

};


extern RemoteControl rc;
void correct_def_analog_value(struct Control_Param &cp);
void _rank(int *array, int len);
