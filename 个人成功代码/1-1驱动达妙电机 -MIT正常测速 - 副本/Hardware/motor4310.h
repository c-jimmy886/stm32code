#ifndef __MOTOR4310_H
#define __MOTOR4310_H

#include <stdint.h>

// 声明控制参数为外部变量，可在主函数中直接修改
extern int16_t position_desired;
extern int16_t velocity_desired;
extern uint16_t kp;
extern uint16_t kd;
extern int16_t torque_ff;

int float_to_uint(float x, float x_min, float x_max, int bits);
void MotorControl_SendFrame(uint8_t can_id, float pos, float vel, float kp, float kd, float torq);

#endif 

