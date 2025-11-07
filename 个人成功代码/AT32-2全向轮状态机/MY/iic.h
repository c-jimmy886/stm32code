#ifndef __IIC_H
#define __IIC_H	 
#include "sys.h"



void iic_init(char SCL_GPIO,int SCL_PIN,char SDA_GPIO,int SDA_PIN);
void iic_start(void);
void iic_stop(void);
int iic_wait_ack(u8 deal);
void iic_send(char str);
void iic_ack(void);
void iic_nack(void);
int iic_read(u8 ack);

#endif
