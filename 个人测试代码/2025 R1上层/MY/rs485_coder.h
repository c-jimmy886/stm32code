#ifndef __RS485_CODER_H
#define __RS485_CODER_H	 
#include "sys.h"


void RS485_coder_init(u8 USARTx,u8 RS485_DE_GPIO,u8 RS485_DE_PIN);//RS485±àÂëÆ÷³õÊ¼»¯
float RS485_coder_angle(u8 USARTx);//¶ÁÈ¡RS485±àÂëÆ÷

#endif
