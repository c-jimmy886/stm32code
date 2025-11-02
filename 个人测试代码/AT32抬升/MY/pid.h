#ifndef __PID_H
#define __PID_H	 
#include "sys.h"

struct PID//pid参数结构体
{
    float Kp;//比例参数
    float Ki;//积分参数
    float Kd;//微分参数
    
    float MB;//目标量
    float MQ;//目前量
    
    float err;//误差
    float last_err;//上一次误差
    
    float P;//比例
    float I;//积分
    float D;//微分
};


typedef struct
{
    struct PID pid;
    float last_place;
    float last_aim_place;
}PID_POS_VEL;

struct sample//平均取样结构体
{
    u8 Sample_init;//取样初始化标志
    u16 Sample_num;//取样总次数
    u16 Sample_add;//当前取样次数
    float I_input;//取样累积
};

void pid_init(struct PID*pid,float Kp,float Ki,float Kd);//初始化pid
double Pid(struct PID*pid,double mb,double mq,double umax,double umin);//进行一次pid运算
uint16_t pid_speed_pwm(struct PID*pid,double mb,double mq,uint16_t pwm_max);//速度环(pwm)
double pid_speed_current(struct PID*pid,double mb,double mq,uint16_t current_max);//速度环(电流)
double pid_place(struct PID*pid,double mb,double mq,double max_input);//位置环
double pid_place_speed(struct PID*pid_place,double aim_place,double now_place,struct PID*pid_speed,double aim_speed,double now_speed,double change_place,double max_output);//位置_速度环
double Pid_PlaceSerialSpeed(struct PID*pid_place,double aim_place,double now_place,struct PID*pid_speed,double max_speed,double now_speed,double max_output,float Ka);
double pid_same(struct PID*pid,double mb,double mq,double average,double max_input,double min_input);//同步环
u8 average(u16 sample_num,float output,float*input,struct sample*SAMPLE);//取平均值
float pid_pos_vel(PID_POS_VEL*BUF,float Kp,float Ki,float Kd,float Ka,float Kj,float speed_max,float now_speed,float aim_place,float now_place,float input_max,u8 fast_on);//位置规划速度的速度位置pid



#endif
