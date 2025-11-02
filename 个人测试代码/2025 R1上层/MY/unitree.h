#ifndef __UNITREE_H
#define __UNITREE_H	 
#include "sys.h"


typedef struct
{
    u8 id;
    u8 usartx;
    u8 dma_enable;
    
    uint8_t mode;
    float t;//N.m
    float vel;//rad/s
    float pos;//rad
    int8_t temp;
    uint8_t error;
    uint16_t force;
    uint16_t crc;
}UNITREE_DATA;

void Unitree_Init(UNITREE_DATA*Unitree_Data,u8 Unitree_Data_Len,u8 USARTx,u8 Dma_Enable,u8 RS485_EN_GPIO,u8 RS485_EN_PIN);
void Unitree_Tx(UNITREE_DATA Unitree_Data,u8 mode,float t,float vel,float pos,float kp,float kd);



#endif
