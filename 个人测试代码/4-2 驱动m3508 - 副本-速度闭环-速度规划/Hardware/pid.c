#include "stm32f10x.h"                  // Device header
#include "pid.h"

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float max, float min)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output_max = max;
    pid->output_min = min;
}

float PID_Calculate(PID_TypeDef* pid, float target, float current)
{
    float error = target - current;
    float derivative = error - pid->prev_error;
    
    // 积分项
    pid->integral += error;
    
    // 积分限幅
    if (pid->integral > pid->output_max) 
        pid->integral = pid->output_max;
    else if (pid->integral < pid->output_min) 
        pid->integral = pid->output_min;
    
    // PID计算
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    
    // 输出限幅
    if (output > pid->output_max)
        output = pid->output_max;
    else if (output < pid->output_min)
        output = pid->output_min;
    
    pid->prev_error = error;
    
    return output;
}
