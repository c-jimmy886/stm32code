#ifndef __INIT_SIMPLIFY_H
#define __INIT_SIMPLIFY_H
#include "sys.h"


#define Pin(GPIO,PIN)  BIT_ADDR(APB2PERIPH_BASE + (GPIO-'A')*0x400 + 0x800 + 8,PIN)//GPIO输入
#define Pout(GPIO,PIN) BIT_ADDR(APB2PERIPH_BASE + (GPIO-'A')*0x400 + 0x800 + 12,PIN)//GPIO输出


#define OUT_PP      1 //推挽输出
#define OUT_OD      2 //开漏输出
#define AF_PP       3 //复用推挽输出
#define AF_OD       4 //复用开漏输出
#define IPU         5 //上拉输入
#define IPD         6 //下拉输入
#define IN_FLOATING 7 //浮空输入
#define AIN         8 //模拟输入






void GPIO_init(u8 GPIO,u8 PIN,u8 GPIO_Mode);//简化GPIO初始化
void CAN_init(u8 CANx,u8 CAN_Mode,u8 CAN_SJW,u8 CAN_BS1,u8 CAN_BS2,u8 CAN_Prescaler);//简化CAN初始化

#endif
