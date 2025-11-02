#ifndef __IIC_H
#define __IIC_H	 
#include "sys.h"

#define Stop 0

#define Front 1
#define Back 2
#define Left 3
#define Right 4

#define Front_Left 13
#define Front_Right 14
#define Back_Left 23
#define Back_Right 24

#define Clockwise 0xf0
#define Anticlockwise 0x0f

#define Front_Clockwise 0xf1
#define Front_Anticlockwise 0x1f

#define Back_Clockwise 0xf2
#define Back_Anticlockwise 0x2f

#define Left_Clockwise 0xf3
#define Left_Anticlockwise 0x3f

#define Right_Clockwise 0xf4
#define Right_Anticlockwise 0x4f

//初始化对应IO口
void mellen_init(u8 GPIO_A1_out1,u8 PIN_A1_out1 , u8 GPIO_A1_out2,u8 PIN_A1_out2,
                 u8 GPIO_B1_out1,u8 PIN_B1_out1 , u8 GPIO_B1_out2,u8 PIN_B1_out2,
                 u8 GPIO_B2_out1,u8 PIN_B2_out1 , u8 GPIO_B2_out2,u8 PIN_B2_out2,
                 u8 GPIO_A2_out1,u8 PIN_A2_out1 , u8 GPIO_A2_out2,u8 PIN_A2_out2);

void mellen_status(u8 status);//麦轮状态
void  mellen_move(int x,int y,int*A,int*B);//麦轮全方位移动（测试）
void mellen_brake(u32 ms);//制动

#endif
