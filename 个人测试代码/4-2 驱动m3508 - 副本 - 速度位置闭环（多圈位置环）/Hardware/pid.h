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
} PID_TypeDef;

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float ka_p,float ka_i,float ka_d, float max, float min);
float PID_Calculate(PID_TypeDef* pid, float target, float current);
float pid_position(PID_TypeDef* pid,float target,float current);


#endif

