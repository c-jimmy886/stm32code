#ifndef __MY_MALLOC_H
#define __MY_MALLOC_H	 
#include "sys.h"

void my_malloc_init(void);//内存管理初始化
void*my_malloc(u32 byte);//申请byte字节内存
void*my_free(void*P);//释放内存
void memory_use(float*per,u32*byte);//内存占用

#endif
