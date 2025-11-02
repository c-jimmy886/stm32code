#ifndef __WALK_MOTOR_H
#define __WALK_MOTOR_H
#include "sys.h"

void walk_motor_init(u8 GPIO1,u8 PIN1,u8 GPIO2,u8 PIN2,u8 GPIO3,u8 PIN3,u8 GPIO4,u8 PIN4);//初始化步进电机
void walk_motor_go_4(u8 FX);//发出一个脉冲信号，FX>0电机顺时针转动，FX<=0电机逆时针转动（单四拍）
void walk_motor_go_2_4(u8 FX);//发出一个脉冲信号，FX>0电机顺时针转动，FX<=0电机逆时针转动（双四拍）
void walk_motor_go_8(u8 FX);//发出一个脉冲信号，FX>0电机顺时针转动，FX<=0电机逆时针转动（八拍）
void walk_motor_angle(float angle,int T_ms);//控制电机转过特定角度

#endif
