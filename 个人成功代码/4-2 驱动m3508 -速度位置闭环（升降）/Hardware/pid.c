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
	
	//位置pid值初始化
    pid->perror=0;
    pid->plast_error=0;
    
    pid->pp_output=0;
    pid->pi_output=0;
    pid->pd_output=0;
   
    pid->pfun_output=0;
	//速度pid值初始化
	
	pid->s_error=0;	
	pid->s_derivative=0;
	
	pid->s_fun_output=0;

}
//target给目标角度，current给total角
float pid_position(PID_TypeDef* pid,float target,float current)
{

	pid->perror=target-current;
//	static float last_error=0;
	
	pid->integral_a +=pid->perror;
	
	pid->pp_output=pid->ka_p*pid->perror;
	pid->pi_output=pid->ka_i*pid->integral_a;
	pid->pd_output=pid->ka_d*(pid->perror-pid->plast_error);
	
	if(pid->integral_a>10000)
	{
		pid->integral_a=10000;
	}
	else if(pid->integral_a<-10000){
		pid->integral_a=-10000;
	}
	
	pid->pfun_output=pid->pp_output+pid->pi_output+pid->pd_output;
	
	if(pid->pfun_output>15000)
	{
		pid->pfun_output=15000;
	}
	else if(pid->pfun_output<-15000)
	{
		pid->pfun_output=-15000;
	}
	pid->plast_error=pid->perror;
	
	pid->output_a=pid->pfun_output;
	return  pid->pfun_output;
}

float PID_Calculate(PID_TypeDef* pid, float target, float current)
{
//    float error = target - current;
	pid->s_error= target - current;
//    float derivative = pid->speed_error - pid->prev_error;
	pid->s_derivative =pid->s_error - pid->prev_error;
    // 积分项
    pid->integral +=pid->s_error;
    
    // 积分限幅
    if (pid->integral > pid->output_max) 
        pid->integral = pid->output_max;
    else if (pid->integral < pid->output_min) 
        pid->integral = pid->output_min;

	// PID计算
//    float output = pid->kp *pid->speed_error+ pid->ki * pid->integral + pid->kd * pid->derivative;
	pid->s_fun_output= pid->kp *pid->s_error+ pid->ki * pid->integral + pid->kd * pid->s_derivative;
	
    // 输出限幅
//    if (pid->s_fun_output> pid->output_max)
//        pid->s_fun_output = pid->output_max;
//    else if (pid->s_fun_output < pid->output_min)
//        pid->s_fun_output = pid->output_min;
	
	if (pid->s_fun_output>10000)
       pid->s_fun_output =10000;
    else if (pid->s_fun_output < -10000)
       pid->s_fun_output = -10000;
	
    
    pid->prev_error =pid->s_error;
    pid->output_s=pid->s_fun_output;
    return pid->s_fun_output;
}

