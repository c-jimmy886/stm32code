#ifndef __SM_H
#define __SM_H
#include "sys.h"


typedef struct
{
    u8 now_state;//当前状态
    void(*state_task)(void);//状态任务函数指针
    u8 event;//触发事件
    void(*act_evernt)(void);//动作函数指针
    u8 next_state;//下一状态
}StateList;//状态列表

typedef struct
{
    StateList*state_list;//状态列表指针
    u8 state_list_len;//状态列表长度
    u8(*event_check)(void);//事件检测指针
    u8 now_strate;//当前状态
}StateMachine;//状态机信息



void Sta_Run(StateMachine*state_machine);


#endif
