#include <stdio.h>
#include <unistd.h>

#define DEALY (1000 * 50)

// PID 控制器的参数
#define KP 1
#define KI 0.05
#define KD 0.2

// PID 控制器的状态
double error_prior = 0;
double integral = 0;

// 更新 PID 控制器状态

/*
    error 是当前时刻的误差值。
    derivative 是误差的导数，通过当前误差和上一时刻的误差值 error_prior 的差值计算得到。
    integral 是误差的积分，它是历史误差值的累加。
    output 是PID控制器的输出，通过误差、积分和导数以及对应的系数 KP、KI、KD 来计算。
    error_prior 保存了上一时刻的误差值，用于计算导数。
*/
double updatePID(double error)
{
    double derivative = error - error_prior;
    integral += error;
    double output = KP * error + KI * integral + KD * derivative;
    error_prior = error;
    return output;
}

int main()
{
    double setpoint = 10000.893;     // 目标值
    double process_variable = -199; // 当前值

    while (process_variable != setpoint)
    {
        double error = setpoint - process_variable;
        double control_variable = updatePID(error);
        process_variable += control_variable; // 根据 PID 控制输出更新当前值
        printf("当前值: %lf\n", process_variable);
        usleep(DEALY);
    }

    printf("Reached setpoint: %lf\n", process_variable);

    return 0;
}
