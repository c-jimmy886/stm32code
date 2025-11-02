#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<init.h>
#include<pid.h>
#include<rm_motor.h>
#include<dm_motor.h>
#include<dma_usart.h>
#include<delay.h>
#include<chassis.h>
#include<free.h>
#include<state_machine.h>


//=================================================================================//
#define  init          'a'
#define  liftup_begin  'b'
#define  down_begin    'c'
#define  finish        'z'
#define  key_press     'k'

u8 Rx_event=0;//接收事件（上位机）

RM_RX_DATA CAN2_RM_BUF[8]={{0x201,-1},{0x202,-1},{0x203,-1},{0x204,-1},{0x205,-1},{0x206,-1},{0x207,-1},{0x208,-1}};//RM电机接收数据

DM_RX_DATA CAN1_DM_BUF[4]={{0x11,-1},{0x11,-1}};

struct PUSH_MOTOR PushMotora[8]={
	{&CAN2_RM_BUF[0].place,&CAN2_RM_BUF[0].vel,&CAN2_RM_BUF[0].cur,&CAN2_RM_BUF[0].zero_angle,0,10000,20000},//升降机构
	{&CAN2_RM_BUF[1].place,&CAN2_RM_BUF[1].vel,&CAN2_RM_BUF[1].cur,&CAN2_RM_BUF[1].zero_angle,0,10000,20000},
	{&CAN2_RM_BUF[2].place,&CAN2_RM_BUF[2].vel,&CAN2_RM_BUF[2].cur,&CAN2_RM_BUF[2].zero_angle,0,10000,20000},
	{&CAN2_RM_BUF[3].place,&CAN2_RM_BUF[3].vel,&CAN2_RM_BUF[3].cur,&CAN2_RM_BUF[3].zero_angle,0,10000,20000},
////底盘电机                    //当前位置             //当前速度          //当前电流    //零点位置//目标位置//目标速度 //最大电流
	{&CAN2_RM_BUF[4].place,&CAN2_RM_BUF[4].vel,&CAN2_RM_BUF[4].cur,0,1000,0,10000},
	{&CAN2_RM_BUF[5].place,&CAN2_RM_BUF[5].vel,&CAN2_RM_BUF[5].cur,0,1000,0,10000},
	{&CAN2_RM_BUF[6].place,&CAN2_RM_BUF[6].vel,&CAN2_RM_BUF[6].cur,0,1000,0,10000},
	{&CAN2_RM_BUF[7].place,&CAN2_RM_BUF[7].vel,&CAN2_RM_BUF[7].cur,0,1000,0,10000},
};
//初始化定义pid
struct PID pids1,pidp1,pids2,pidp2,pids3,pidp3,pids4,pidp4,pid5,pid6,pid7,pid8; //rm电机pid，在rm_motor里面extern

PID_POS_VEL pid1;

/******************************************任务句柄************************************************/
void*Hand_Task_Handler;
void*Test_Task_Handler;
void*Control_Task_Handler;
void*ActionStart_Task_Handler;


void Hand_Task()
{
	char com_buf[16];
	while(1)
	{
		USARTx_Rx(7,com_buf,5);
		if(com_buf[0]=='['&&com_buf[3]==']')
		{
			Rx_event=com_buf[1];
		}
		
        Rx_event=com_buf[1];//接收串口发送的信息
//		Uart_Printf(7, "hand_task\r\n");
		
		vTaskDelay(10);
	}

}

void Test_Task()
{
	while(1)
	{
		Uart_Printf(7,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",PushMotora[0].Aim_Place,*PushMotora[0].Now_Place,PushMotora[1].Aim_Place,*PushMotora[1].Now_Place,
		PushMotora[2].Aim_Place,*PushMotora[2].Now_Place,PushMotora[3].Aim_Place,*PushMotora[3].Now_Place);
		
	    Uart_Printf(7, "%d,%d,%d,%d,%d,%d,%d,%d\r\n",PushMotora[4].Aim_Speed,*PushMotora[4].Now_Speed,PushMotora[5].Aim_Speed,*PushMotora[5].Now_Speed,
		PushMotora[6].Aim_Speed,*PushMotora[6].Now_Speed,PushMotora[7].Aim_Speed,*PushMotora[7].Now_Speed);
		
		vTaskDelay(50);
	}
}

void Control_Task()
{
    char buf[16];
	
	int move_time=0;
	while(1)
	{
        vTaskDelay(50);      
	}
}

/*******************************************************************条件延时*******************************************************************/

int delay_5ms=0;//延时计数
//condition：条件
//MaxDelay：最大延时
//每5ms进行一次判断，符合条件并且不超过最大延时，则继续延时，否则结束返回0
//当MaxDelay=0xFFFF时，不存在最大延时，即最大延时无限
#define CON_DELAY_5MS(condition,MaxDelay)                      \
{                                                              \
    delay_5ms=0;                                               \
    while(condition&&(delay_5ms<MaxDelay || MaxDelay==0xFFFF)) \
    {                                                          \
        delay_5ms++;                                           \
        vTaskDelay(5);                                         \
    }                                                          \
}

/************************************************************************************************/


int lift_move_times=0;
void lift_state_task_begin()//抬升开始任务
{
   	PushMotora[0].Aim_Place+=2000;    PushMotora[1].Aim_Place+=2000; 
    PushMotora[2].Aim_Place+=2000;    PushMotora[3].Aim_Place+=2000; 
	Rx_event='a';//
	
	CON_DELAY_5MS(Rx_event!=init,0xFFFF);
}

void lift_state_task_run()
{
    Rm_Motor_posManeger(PushMotora);
		
	Rm_Motor_velManeger(PushMotora);
	Rx_event='z';
	CON_DELAY_5MS(Rx_event!=finish,0xFFFF);
}

void lift_state_task_end()
{
	CON_DELAY_5MS(Rx_event!=key_press,0xFFFF);
}

void down_state_task_begin()//抬升开始任务
{
   	PushMotora[0].Aim_Place-=2000;    PushMotora[1].Aim_Place-=2000; 
    PushMotora[2].Aim_Place-=2000;    PushMotora[3].Aim_Place-=2000; 
	Rx_event='a';
	
	CON_DELAY_5MS(Rx_event!=init,0xFFFF);
}

void down_state_task_run()
{
    Rm_Motor_posManeger(PushMotora);
		
	Rm_Motor_velManeger(PushMotora);
	Rx_event='z';
	CON_DELAY_5MS(Rx_event==finish,0xFFFF);
}

void down_state_task_end()
{
	 CON_DELAY_5MS(Rx_event==key_press,0xFFFF);
}

/*复位*/
void  recover_state_task_run()
{
	static int initialized=0;
	
	if(!initialized)
	{
		PushMotora[0].Aim_Place=0;    PushMotora[1].Aim_Place=0; 
		PushMotora[2].Aim_Place=0;    PushMotora[3].Aim_Place=0; 
		initialized=1;
	}
//	Rx_event='z';
//	CON_DELAY_5MS(Rx_event!=finish,0xFFFF);
}

STATE_MISS  lift_state, down_state, recover_state;
STATE_Handle  Sta_Handler;  //状态机句柄

void ActionStart_Task()
{
	TickType_t lasttick;
    lasttick = xTaskGetTickCount();
	
	StateCreate(&lift_state,lift_state_task_begin,lift_state_task_run,lift_state_task_end,2,
	            NEXT_STATE_MISS_COM(down_begin,&down_state),
	            NEXT_STATE_MISS_COM(liftup_begin,&lift_state));
	
	StateCreate(&down_state,down_state_task_begin,down_state_task_run,down_state_task_end,2,
	            NEXT_STATE_MISS_COM(liftup_begin,&lift_state),
	            NEXT_STATE_MISS_COM(down_begin,&down_state));
	
	StateCreate(&recover_state,NULL,recover_state_task_run,NULL,2,
	            NEXT_STATE_MISS_COM(liftup_begin,&lift_state),
	            NEXT_STATE_MISS_COM(down_begin,&down_state));
	
	
	State_Handle_Init(Sta_Handler,recover_state);
	while(1)
	{
		 StateRun(&Rx_event,0xFF,&Sta_Handler);//状态机运行：当前状态事件Rx_event，状态事件掩码0xFF，状态机句柄Sta_Handler
		//状态机的事件是通过全局变量Rx_event来传递的。在状态机运行函数StateRun中，我们传入Rx_event的地址
		//通过串口给Rx_event赋值
        vTaskDelayUntil(&lasttick, 10);//每10ms运行一次
	}
}

#define task_num  4
Task_Mess R1_Task[task_num]={
{(TaskFunction_t)Test_Task,&Test_Task_Handler,256,4,"Test_Task",NULL},//测试任务
{(TaskFunction_t)Hand_Task,&Hand_Task_Handler,256,4,"Hand_Task",NULL},
{(TaskFunction_t)Control_Task,&Control_Task_Handler,256,4,"Control_Task",NULL},
{(TaskFunction_t)ActionStart_Task,&ActionStart_Task_Handler,256,3,"ActionStart_Task",NULL},//运动状态任务
};


void CAN2_IRQ(uint32_t id,u8*data)//CAN2中断接收回调函数
{
    RM_Rx(id,data);//大疆电机数据接收，
	//分两个can控制rm电机，因为接收到的id不同，所以数据不会串
//    sys_timeout=0;
}


void CAN1_IRQ(uint32_t id,u8*data)//CAN1中断接收回调函数
{
     RM_Rx(id,data);
}



uint8_t onmi_speed=0;
uint8_t onmi_angle=0;

int main()
{
   //初始化定时器
	
	delay_init();
	 //初始化GPIO口
    GPIO_init('D',13,OUT_PP);PDout(13)=0;
    GPIO_init('D',14,OUT_PP);PDout(14)=1;
    GPIO_init('D',15,OUT_PP);PDout(15)=0;
    GPIO_init('C',3,IPU);
    GPIO_init('E',3,IPD);
	
	//120
//	Tim_Init(3,2000,120,Up);//用于定时更新速度规划
	RM_Init(2,CAN2_RM_BUF,8);
	
	RM_Init(1,CAN2_RM_BUF,8);
	
	DM_Motor_Init(1, CAN1_DM_BUF,9,0x00);
	
//	dma_uart_init(1,115200);
//	dma_uart_init(7,115200);//调试串口
	
	Uartx_Init(7, 115200);  // USART3
	
	CAN2_RX1_IRQ=CAN2_IRQ;//装载CAN2回调函数  m3508
	CAN1_RX0_IRQ=CAN1_IRQ;//装载CAN1回调函数  dm
	
	//升降电机pid初始化
	pid_init(&pids1,10.0,0.1,4);
    pid_init(&pidp1,0.8,0,0.1);
    pid_init(&pids2,10.0,0.1,4);
    pid_init(&pidp2,0.8,0,0.1);
	pid_init(&pids3,10.0,0.1,4);
    pid_init(&pidp3,0.8,0,0.1);
    pid_init(&pids4,10.0,0.1,4);
    pid_init(&pidp4,0.8,0,0.1);
	//底盘电机pid初始化
     pid_init(&pid5,10,0.002,20);
	 pid_init(&pid6,10,0.002,20);
	 pid_init(&pid7,10,0.002,20);
	 pid_init(&pid8,10,0.002,20);
	


		
    while(1)
    {

//		dma_printf("%d,%d,%d,%d,%d\r\n",onmi_speed,PushMotora[5].Aim_Speed,*PushMotora[5].Now_Speed,PushMotora[6].Aim_Speed,*PushMotora[6].Now_Speed);
//		
////	    dma_printf("%d,%d,%d,%d\r\n",*PushMotor.Now_Current,*PushMotor1.Now_Current,*PushMotor2.Now_Current,*PushMotor3.Now_Current);

//		dma_rx(7,buf,13);
		

        Uart_Printf(7, "String: %s, Hex: 0x%X\r\n", "Test", 255);
		
		FreeRtos_Mode(R1_Task,task_num);
    }
}

//void TMR3_GLOBAL_IRQHandler(void)
//{
//    if(tmr_flag_get(TMR3, TMR_OVF_FLAG) != RESET)
//    {
//	
//////	U5=pid_pos_vel(&pid1,8,1.1,2,50,20,8000,*PushMotor4.Now_Speed,PushMotor4.Aim_Place,*PushMotor4.Now_Current,17000,0);

////		omni(PushMotora,onmi_speed,onmi_angle,2);
//		
//		Rm_Motor_posManeger(PushMotora);
//		
//		Rm_Motor_velManeger(PushMotora);
//		
////		//达妙电机
////		DM_Motor_Tx(0x01,20,200,1,1,1);
////		
////		DM_Motor_Cmd(0x01,DM_ENABLE);
//		
//        tmr_flag_clear(TMR3, TMR_OVF_FLAG);//清除标志位
//    }
//}



