#include<state_machine.h>
#include<stdio.h>
#include<stdarg.h>


/*==============================================================================说明==================================================================================
主要思想：状态机，动态内存分配，内存池

具体实现：
    自定义类型：（1）状态信息STATE_MISS（包括：状态开始函数指针（void (*task_begin)()）；
                                             状态运行函数指针（void (*task_run)()）；
                                             状态结束函数指针（void (*task_end)()）；
                                             下一状态信息在下一状态存储池首地址指针（struct NEXT_STATE_MEMORY*next_state_p）；
                                             下一状态数量（u8 next_state_size）；
                                             状态初始化标志（u8 init_flag））
               （2）下一状态存储池struct NEXT_STATE_MEMORY（包括：切换下一状态事件（uint16_t event）；
                                                                下一状态结构体（STATE_MISS*next_state_p））
               （3）状态机句柄STATE_Handle（包括：状态机信息地址（STATE_MISS* State_Miss）；
                                                 状态切换标志（u8 State_Switch_Flag））
                                                 
    实现过程：
        创建状态：相关函数：u8 StateCreate(STATE_MISS*State_Miss,void (*Task_Begin)(),void (*Task_Run)(),void (*Task_End)(),u8 Next_State_Size, ...)
                 让用户先定义一个STATE_MISS类型（状态信息）的变量然后把地址赋给参数State_Miss，状态创建的相关信息都存放在这里
                 void (*Task_Begin)()：状态开始函数指针，每次进入该状态都运行一次，不用可以赋值NULL
                 void (*Task_Run)()：状态运行函数指针，在该状态下，每状态机周期都运行一次，不用可以赋值NULL
                 void (*Task_End)()：状态结束函数指针，切换下一状态之前运行一次，不用可以赋值NULL
                 Next_State_Size：这是该状态可以切换到下一状态的数量，这里将根据Next_State_Size的值给该状态分配下一状态信息的内存空间
                 ... ：这是一个不定参数，传参格式应为：[下一状态切换事件1],[下一状态地址1],[下一状态切换事件2],[下一状态地址2],...,[下一状态切换事件n],[下一状态地址n]（其中n必须等于Next_State_Size，下一状态切换事件值只能是0~0xFF），
                       也可以通过 NEXT_STATE_MISS_COM(event,state_miss) 宏来传参，使格式看起来顺眼；如果下一状态的值是0xFFFF，则将该事件标记为其他切换事件，如何传入事件不符合前面的所有事件，则切换成该事件对应的状态（此事件只能放在最后），
                       也可以通过 NEXT_STATE_MISS_OTHER_COM(state_miss) 宏来传参，使格式看起来顺眼；如果下一状态地址传入的是NULL，则即使传入事件符合切换事件也不会进行状态切换，跟该功能结合可实现当传入事件不符合某些事件时则切换状态
                       
        状态机运行：相关函数void StateRun(u8*Event,u8 Event_Mask,STATE_Handle*State_Handle)
                   让用户先定义一个STATE_Handle类型（状态机句柄）的变量然后把地址赋给参数State_Miss，状态机运行相关信息都存放在这里
                   u8*Event传入事件：状态机将根据传入的事件是否符合某个下一状态的切换事件，如果符合，则进行状态切换
                   u8 Event_Mask事件掩码：状态机将根据掩码忽略传入事件的某些位（传入事件跟掩码位0的位对应的位将被忽略）
                   STATE_Handle*State_Handle状态机句柄：状态机句柄传入前必须初始化，可通过 State_Handle_Init(State_Handle,Firte_State_Miss) 宏进行，
                                                       本质上是将首个状态地址传给State_Handle.State_Miss，并且将State_Handle.State_Switch_Flag设置成0
                                                       
                                                       
    说明一下该状态机为什么要用到“动态内存”和“内存池”：因为该状态机的状态创建是以状态节点为单位的，而每个状态能切换的下一个状态的数量是不确定的，如果直接给每一个状态给一个
足够大的内存来存储下一个状态的话会浪费大量内存，所以使用了动态内存；又因为单片机的 RAM 一般比较小，如果直接使用malloc来申请内存而且又处理不当的话容易导致系统奔溃，所以使用内
存池思想，事先创建一个足够大的数组作为内存池 Next_State_Memory[NEXT_STATE_MEMORY_MAX] 进行分配使用，就避免了 RAM 的开销和保证了程序运行稳定

====================================================================================================================================================================*/


/*下一状态存储池*/
struct NEXT_STATE_MEMORY
{
    uint16_t event;//切换下一状态事件
    STATE_MISS*next_state_p;//下一状态结构体
}Next_State_Memory[NEXT_STATE_MEMORY_MAX];
u16 Next_State_Memory_P=0;//下一状态存储池指针



/*创建状态*/
//State_Miss：状态单元（结构体）
//Task_Begin：状态开始任务函数
//Task_Run：状态执行任务函数
//Task_End：状态结束任务函数
//Next_State_Size：连接下一状态数量
//... ：下一状态转换事件以及状态结构体（两者交替放置，数量为Next_State_Size）
u8 StateCreate(STATE_MISS*State_Miss,void (*Task_Begin)(),void (*Task_Run)(),void (*Task_End)(),u8 Next_State_Size, ...)
{
    u8 i;
    va_list ap;
    
    if(Next_State_Size>NEXT_STATE_MEMORY_MAX-Next_State_Memory_P)return 0;//下一状态数量大于下一状态存储池剩余容量，下一状态存储池内存不足，状态创建失败，返回0
    
    State_Miss->task_begin=Task_Begin;//状态开始任务函数
    State_Miss->task_run=Task_Run;//状态执行任务函数
    State_Miss->task_end=Task_End;//状态结束任务函数
    State_Miss->next_state_size=Next_State_Size;//连接下一状态数量
    State_Miss->next_state_p=&Next_State_Memory[Next_State_Memory_P];//下一状态在下一状态存储池的首地址
    State_Miss->init_flag=1;//标记状态已经初始化
    
    va_start(ap,Next_State_Size);
    for(i=0;i<Next_State_Size;i++)
    {
        Next_State_Memory[Next_State_Memory_P].event=va_arg(ap,int);//转换下一状态事件
        Next_State_Memory[Next_State_Memory_P].next_state_p=va_arg(ap,STATE_MISS*);//下一状态地址
        Next_State_Memory_P++;//下一状态存储池指针自增，表明该内存已使用
    }
    va_end(ap);
    
    return 1;//状态创建成功，返回1
}



/*状态机运行*/
//Event：当前事件
//Event_Mask：事件掩码
//State_Handle状态机句柄
void StateRun(u8*Event,u8 Event_Mask,STATE_Handle*State_Handle)
{
    u8 i,j;
    if((*State_Handle).State_Switch_Flag==0)//状态刚发生切换
    {
        if((*State_Handle).State_Miss->task_begin != NULL)(*State_Handle).State_Miss->task_begin();//状态开始任务函数非空，运行状态开始函数
        (*State_Handle).State_Switch_Flag=1;//清除状态切换标志
    }
    if((*State_Handle).State_Miss->task_run != NULL) (*State_Handle).State_Miss->task_run();//状态执行任务函数非空，执行状态执行任务函数
    for(i=0;i<(*State_Handle).State_Miss->next_state_size;i++)//扫描切换下一状态事件
    {
        if((((((*State_Handle).State_Miss->next_state_p+i)->event)&Event_Mask) == ((*Event)&Event_Mask) || ((*State_Handle).State_Miss->next_state_p+i)->event == 0xFFFF) && ((*State_Handle).State_Miss->next_state_p+i)->next_state_p != NULL && ((*State_Handle).State_Miss->next_state_p+i)->next_state_p->init_flag)//存在切换下一状态事件符合传入的事件或者传入事件为其他情况（0xFFFF），且下一状态非空，并且已经初始化
        {
            if(((*State_Handle).State_Miss->next_state_p+i)->event == 0xFFFF)//传入事件为其他情况（0xFFFF）
            {
                for(j=0;j<i;j++)//从下一状态的的一个开始扫描
                {
                    if(((((*State_Handle).State_Miss->next_state_p+j)->event)&Event_Mask) == ((*Event)&Event_Mask))break;//发现存在其中一个下一状态切换事件符合当前事件，说明不符合其他事件
                }
                if(i!=j)return;//不符合其他事件，直接结束扫描
            }
            
            if((*State_Handle).State_Miss->task_end != NULL)(*State_Handle).State_Miss->task_end();//状态结束任务函数非空，运行状态结束函数
            (*State_Handle).State_Miss=((*State_Handle).State_Miss->next_state_p+i)->next_state_p;//状态切换
            (*State_Handle).State_Switch_Flag=0;//标记状态发生切换
            return;
        }
    }
}






/**************************************************示例**************************************************

//定义状态执行任务
void task1()
{
    printf("sta1\n");
}
void task2()
{
    printf("sta2\n");
}
void task3()
{
    printf("sta3\n");
}


STATE_MISS sta1,sta2,sta3;//定义状态节点（结构体）
STATE_Handle sta_handler//定义状态机句柄
u8 event=0;定义事件



int main()
{
    ...
//创建状态1：无状态开始任务，状态执行任务为task1，无状态结束任务，下一状态数量2 ； NEXT_STATE_MISS_COM(2,&sta2)：2为其中一个切换下一状态事件，sta2其中一个下一状态结构体（下面同理）
    StateCreate(&sta[0],NULL,task1,NULL,2,NEXT_STATE_MISS_COM(2,&sta2),NEXT_STATE_MISS_COM(3,&sta3));//创建状态1：无状态开始任务，状态执行任务为task1，无状态结束任务，下一状态数量2 ；
	StateCreate(&sta[1],NULL,task2,NULL,2,NEXT_STATE_MISS_COM(1,&sta1),NEXT_STATE_MISS_COM(3,&sta3));NEXT_STATE_MISS_COM(2,&sta2)：2为其中一个切换下一状态事件，sta2其中一个下一状态结构体（下面同理）
    StateCreate(&sta[2],NULL,task3,NULL,2,NEXT_STATE_MISS_COM(1,&sta1),NEXT_STATE_MISS_COM(2,&sta2));
    
    State_Handle_Init(sta_handler,sta1);初始化状态机句柄，设置首个状态为是sta1
    
    while(1)
    {
        StateRun(&event,0xFF,&sta_handler);//状态机运行：event传入事件，0xFF事件掩码，sta_handler状态机句柄
        delay_ms(500);
    }
}

*******************************************************************************************************/


