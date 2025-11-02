#include "stm32f10x.h"                  // Device header
#include "pid.h"

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float ka_p,float ka_i,float ka_d, float max, float min)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
	pid->ka_p=ka_p;
	pid->ka_i=ka_i;
    pid->ka_d=ka_d;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output_max = max;
    pid->output_min = min;
}
//target给目标角度，current给total角
float pid_position(PID_TypeDef* pid,float target,float current)
{
	float error=target-current;
	static float last_error=0;
	
	pid->integral_a += error;
	
	float p_output=pid->ka_p*error;
	float i_output=pid->ka_i*pid->integral_a;
	float d_output=pid->ka_d*(error-last_error);
	
	if(pid->integral_a>10000)
	{
		pid->integral_a=10000;
	}
	else if(pid->integral_a<-10000){
		pid->integral_a=-10000;
	}
	
	float output=p_output+i_output+d_output;
	
	if(output>15000)
	{
		output=15000;
	}
	else if(output<-15000)
	{
		output=-15000;
	}
	last_error=error;
	
	pid->output_a=output;
	return output;
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
//    if (output > pid->output_max)
//        output = pid->output_max;
//    else if (output < pid->output_min)
//        output = pid->output_min;
	
	    if (output > 5000)
        output =5000;
    else if (output <-5000)
        output = -5000;
    
    pid->prev_error = error;
    pid->output_s=output;
	
    return output;
}

