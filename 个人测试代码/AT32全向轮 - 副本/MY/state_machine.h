#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H	 
#include "sys.h"


#define NEXT_STATE_MEMORY_MAX 256//下一状态储存池容量（可修改）



#define NEXT_STATE_MISS_COM(event,state_miss) event,state_miss//下一状态信息组合（event：下一状态切换事件；state_miss：下一状态地址）
#define NEXT_STATE_MISS_OTHER_COM(state_miss) 0xFFFF,state_miss//下一状态信息组合（其他事件切换）（state_miss：下一状态地址）（此信息必须放在下一状态信息的最后面）
#define State_Handle_Init(State_Handle,Firte_State_Miss) State_Handle.State_Switch_Flag=0; (State_Handle.State_Miss)=&Firte_State_Miss//状态机句柄初始化（即将状态句柄当前状态信息指向首状态）（State_Handle：状态机句柄；Firte_State_Miss首状态）


//状态信息结构体
typedef struct
{
    void (*task_begin)();//状态开始函数指针
    void (*task_run)();//状态运行函数指针
    void (*task_end)();//状态结束函数指针
    struct NEXT_STATE_MEMORY*next_state_p;//下一状态信息在下一状态存储池首地址指针
    u8 next_state_size;//下一状态数量
    u8 init_flag;//状态初始化标志
}STATE_MISS;


//状态机句柄结构体
typedef struct 
{
    STATE_MISS* State_Miss;//状态机信息地址
    u8 State_Switch_Flag;//状态切换标志
}STATE_Handle;


u8 StateCreate(STATE_MISS*State_Miss,void (*Task_Begin)(),void (*Task_Run)(),void (*Task_End)(),u8 Next_State_Size, ...);//创建状态
void StateRun(u8*Event,u8 Event_Mask,STATE_Handle*State_Handle);//状态机运行



#endif
