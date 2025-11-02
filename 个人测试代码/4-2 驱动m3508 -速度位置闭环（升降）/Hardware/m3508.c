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
	
	//can数据处理函数
	motor->canfirst_read=1;
	motor->canlast_angle=0;
	motor->cantotal_angle=0;
	
	
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
//5-8号电机
void M3508_SetCurrent1(int16_t current1, int16_t current2, int16_t current3, int16_t current4)
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
    
    CAN_SendMessage(CAN_TX_ID1, data, 8);
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

void m3508_upstair(Motor_TypeDef Motor[], void* pid, void* pid1, void* pid2, void* pid3)
{
	PID_TypeDef* speed_pid = (PID_TypeDef*)pid;
	PID_TypeDef* speed_pid1 = (PID_TypeDef*)pid1;
	PID_TypeDef* speed_pid2 = (PID_TypeDef*)pid2;
	PID_TypeDef* speed_pid3 = (PID_TypeDef*)pid3;
	
	//电机1
	float a=pid_position(speed_pid,Motor[0].target_position,Motor[0].total_angle);	
	float output = PID_Calculate(speed_pid,a,Motor[0].real_speed);
	//电机2
	float b=pid_position(speed_pid1,Motor[1].target_position,Motor[1].total_angle);	
	float output1 = PID_Calculate(speed_pid1,b,Motor[1].real_speed);
	//电机3
	float c=pid_position(speed_pid2,Motor[2].target_position,Motor[2].total_angle);	
	float output2 = PID_Calculate(speed_pid2,c,Motor[2].real_speed);
	//电机4
	float d=pid_position(speed_pid3,Motor[3].target_position,Motor[3].total_angle);
	float output3 = PID_Calculate(speed_pid3,d,Motor[3].real_speed);
	
	M3508_SetCurrent((int16_t)output,(int16_t)output1,(int16_t)output2,(int16_t)output3);
}

void m3508_move(Motor_TypeDef Motor[], void* pid, void* pid1, void* pid2, void* pid3)
{
	PID_TypeDef* speed_pid = (PID_TypeDef*)pid;
	PID_TypeDef* speed_pid1 = (PID_TypeDef*)pid1;
	PID_TypeDef* speed_pid2 = (PID_TypeDef*)pid2;
	PID_TypeDef* speed_pid3 = (PID_TypeDef*)pid3;
	
	float output = PID_Calculate(speed_pid,Motor[0].target_speed,Motor[0].real_speed);
	float output1 = PID_Calculate(speed_pid1,Motor[1].target_speed,Motor[1].real_speed);
	float output2 = PID_Calculate(speed_pid2,Motor[2].target_speed,Motor[2].real_speed);
	float output3 = PID_Calculate(speed_pid3,Motor[3].target_speed,Motor[3].real_speed);
	
	M3508_SetCurrent1((int16_t)output,(int16_t)output1,(int16_t)output2,(int16_t)output3);
}



