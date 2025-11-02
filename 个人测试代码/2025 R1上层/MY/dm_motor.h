#ifndef __DM_MOTOR_H
#define __DM_MOTOR_H	 
#include "sys.h"



typedef struct
{
    uint32_t id;//电机id;
    float pos;//位置
    float vel;//速度
    float t;//转矩
    u8 t_mos;//MOS温度
    u8 t_rotor;//电机温度
    int round;
    float last_pos;//上一个单圈位置
    double place;//多圈位置
}DM_RX_DATA;//达妙电机数据接收结构体




#define DM_P_MIN   -12.5  //位置最小值
#define DM_P_MAX   12.5   //位置最大值
#define DM_V_MIN   -45    //速度最小值
#define DM_V_MAX   45     //速度最大值
#define DM_KP_MIN  0      //Kp最小值
#define DM_KP_MAX  500    //Kp最大值
#define DM_KD_MIN  0      //Kd最小值
#define DM_KD_MAX  5      //Kd最大值
#define DM_T_MIN   -18    //转矩最大值
#define DM_T_MAX   18     //转矩最小值
#define DM_AP_MIN  -3.125  //单圈位置最小值
#define DM_AP_MAX  3.125  //单圈位置最大值


//电机基础指令
#define DM_ENABLE 0xFC  //使能电机
#define DM_DISABLE 0xFD //失能电机
#define DM_POS_ZERO 0xFE//保存位置零点
#define DM_ERROR 0xFB	 //清除错误


//电机模式
#define CtrlMotor_Mode_MIT 0x00//MIT模式
#define CtrlMotor_Mode_POS_VEL 0x100//位置速度模式
#define CtrlMotor_Mode_VEL 0x200//速度模式


int float_to_uint(float x, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void DM_Motor_Init(u8 CANx,DM_RX_DATA*DM_Rx_Buf,u8 Buf_Len,uint16_t mode);
void DM_Motor_Tx(u8 id,float _vel,float _pos,float _KP,float _KD,float _torq);
void DM_Motor_Cmd(u8 id,u8 cmd);
void DM_Motor_Rx(uint32_t id,u8*data);

#endif





