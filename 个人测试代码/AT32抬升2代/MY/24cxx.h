#ifndef __24CXX_H
#define __24CXX_H	 
#include "sys.h"


u8 AT24Cxx_Init(u8 SCL_GPIO,u8 SCL_PIN,u8 SDA_GPIO,u8 SDA_PIN,uint16_t check);//AT24Cxx初始化
void AT24Cxx_Send(uint16_t add,u8 data);//发送一个数据
u8 AT24Cxx_Read(uint16_t add);//读一个数据
void AT24Cxx_Many_Send(uint16_t first_add,char*data,uint16_t len);//发送多个数据
void AT24CXx_Many_Read(uint16_t first_add,char*buf,uint16_t len);//读多个数据
#endif
