#include "stm32f10x.h"                  // Device header
#include "m3508.h"
#include "can.h"
#include "pid.h"

void M3508_Init(Motor_TypeDef* motor, uint16_t can_id)
{
    motor->angle = 0;
    motor->real_speed = 0;
    motor->current = 0;
    motor->temperature = 0;
    motor->total_angle = 0;
    motor->target_speed = 0;
	motor->target_position=0;
    motor->can_id = can_id;
}
//现在只使用一个电机，0x201
void M3508_SetCurrent(int16_t current1, int16_t current2, int16_t current3, int16_t current4)
{
    uint8_t data[8];
    
    data[0] = current1 >> 8;
    data[1] = current1 & 0xFF;
    data[2] = current2 >> 8;
    data[3] = current2 & 0xFF;
    data[4] = current3 >> 8;
    data[5] = current3 & 0xFF;
    data[6] = current4 >> 8;
    data[7] = current4 & 0xFF;
    
    CAN_SendMessage(CAN_TX_ID, data, 8);
}

void M3508_SpeedControl(Motor_TypeDef* motor, void* pid)
{
    PID_TypeDef* speed_pid = (PID_TypeDef*)pid;
    
//    // 速度环pid
//    float output = PID_Calculate(speed_pid, motor->target_speed, motor->real_speed);
	
//	//位置环pid
//	float output=pid_position(speed_pid, motor->target_position,motor->total_angle);
    
	//先计算一下位置环pid，然后将位置环得到的参数传到速度环
	float a=pid_position(speed_pid, motor->target_position,motor->total_angle);
	
	    //速度环PID输出
    float output = PID_Calculate(speed_pid,a, motor->real_speed);
	
    // 发送电流值到电机
    M3508_SetCurrent((int16_t)output,(int16_t)output,(int16_t)output,(int16_t)output);
	
}

