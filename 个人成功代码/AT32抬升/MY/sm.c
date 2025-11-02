#include<sm.h>
#include<usart.h>


//状态机运行
//state_machine：状态机信息
void Sta_Run(StateMachine*state_machine)
{
    u8 i;
    for(i=0;i<state_machine->state_list_len;i++)
    {
        if(state_machine->now_strate == (state_machine->state_list+i)->now_state)//检测当前状态
        {
            if((state_machine->state_list+i)->state_task!=NULL)((state_machine->state_list+i)->state_task)();//执行任务
            if((state_machine->event_check)()==(state_machine->state_list+i)->event)//检测事件是否触发
            {
                if((state_machine->state_list+i)->act_evernt!=NULL)((state_machine->state_list+i)->act_evernt)();//执行动作
                state_machine->now_strate=(state_machine->state_list+i)->next_state;//状态转移
            }
        }
    }
}
