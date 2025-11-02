#ifndef __PID_H
#define __PID_H

typedef struct {
    float kp;
    float ki;
    float kd;
	float ka_p;
	float ka_i;
	float ka_d;
    float integral;
	float integral_a;
	
	float output_s;
	float output_a;
	
    float prev_error;
    float output_max;
    float output_min;
	
	//位置PID计算函数中的数值
	float perror;
	float plast_error;
	
	float pp_output;
	float pi_output;
	float pd_output;
	
	float pfun_output;
	
	//速度PID计算函数中的数值
	float s_error;	
	float s_derivative;
	
	float s_fun_output;
	
} PID_TypeDef;




void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float ka_p,float ka_i,float ka_d, float max, float min);
float PID_Calculate(PID_TypeDef* pid, float target, float current);
float pid_position(PID_TypeDef* pid,float target,float current);


#endif

