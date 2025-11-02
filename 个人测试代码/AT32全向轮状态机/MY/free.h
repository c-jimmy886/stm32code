#ifndef __FREE_H
#define __FREE_H	 
#include <sys.h>
#include<free.h>
#include<freertos.h>
#include<task.h>
#include<semphr.h>




typedef struct
{
    TaskFunction_t         pxTaskCode;//任务函数
    void*                  Handler;//任务句柄
    
    configSTACK_DEPTH_TYPE usStackDepth;//堆栈
    UBaseType_t            priority;//优先级
    char                   pcName[32];//任务名
    void*                  pvParameters;//参数
}Task_Mess;//任务信息结构体


void FreeRtos_Mode(Task_Mess*Free_Mess_P,u8 Free_num);//使用FreeRTOS系统

#endif

