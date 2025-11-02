#ifndef __RM_MOTOR_H
#define __RM_MOTOR_H	 
#include "sys.h"

#define RM_POS_MAX 8191
#include<chassis.h>



typedef struct
{
    uint32_t id;//ID
    int16_t pos;//角度
    int16_t vel;//速度
    int16_t cur;//电流
    int zero_angle;  //上电时的将初始值设为0点
    int16_t last_pos;//上一个角度
    int round;//圈数
    int place;//位置（多圈）
	
}RM_RX_DATA;


void RM_Init(u8 CANx,RM_RX_DATA*RM_Rx_Buf,u8 Buf_Len);//RM电机CAN初始化
//void RM_Tx(uint32_t id,int current1,int current2,int current3,int current4);//发送四路电机电流值
void RM_Tx(uint32_t  id,u8 canx,int current1,int current2,int current3,int current4);
//void RM_Rx(uint32_t id,u8*data);
void RM_Rx(uint32_t id,u8*buf);//接收电机信息
int RM_One_Round(int mb_place,int mq_place);//单圈位置最短距离解算

void Rm_Motor_posManeger(struct PUSH_MOTOR *PushMotor);//四个电机位置控制
void Rm_Motor_velManeger(struct PUSH_MOTOR *PushMotor);//四个电机速度控制

#endif
