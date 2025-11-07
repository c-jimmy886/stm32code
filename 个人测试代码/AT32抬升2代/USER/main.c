#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#include<init.h>
#include<pid.h>
#include<rm_motor.h>
#include<dm_motor.h>
#include<dma_usart.h>
#include<delay.h>
#include<chassis.h>
#include<free.h>
#include<state_machine.h>
#include<message_center.h>


//=================================================================================//


char Rx_event[16]={0};//接收事件（上位机）
char com_buf[16];//接收数据，usart发送的数据
uint16_t target_speed=0,target_angle=0;

RM_RX_DATA CAN2_RM_BUF[8]={{0x201,-1},{0x202,-1},{0x203,-1},{0x204,-1},{0x205,-1},{0x206,-1},{0x207,-1},{0x208,-1}};//RM电机接收数据

DM_RX_DATA CAN1_DM_BUF[4]={{0x11,-1},{0x11,-1}};

struct PUSH_MOTOR PushMotora[8]={
	{&CAN2_RM_BUF[0].place,&CAN2_RM_BUF[0].vel,&CAN2_RM_BUF[0].cur,&CAN2_RM_BUF[0].zero_angle,0,10000,20000},//升降机构
	{&CAN2_RM_BUF[1].place,&CAN2_RM_BUF[1].vel,&CAN2_RM_BUF[1].cur,&CAN2_RM_BUF[1].zero_angle,0,10000,20000},
	{&CAN2_RM_BUF[2].place,&CAN2_RM_BUF[2].vel,&CAN2_RM_BUF[2].cur,&CAN2_RM_BUF[2].zero_angle,0,10000,20000},
	
	{&CAN2_RM_BUF[3].place,&CAN2_RM_BUF[3].vel,&CAN2_RM_BUF[3].cur,0,1000,0,10000},
////底盘电机                    //当前位置             //当前速度          //当前电流    //零点位置//目标位置//目标速度 //最大电流
	{&CAN2_RM_BUF[4].place,&CAN2_RM_BUF[4].vel,&CAN2_RM_BUF[4].cur,0,1000,0,10000},
	{&CAN2_RM_BUF[5].place,&CAN2_RM_BUF[5].vel,&CAN2_RM_BUF[5].cur,0,1000,0,10000},
	{&CAN2_RM_BUF[6].place,&CAN2_RM_BUF[6].vel,&CAN2_RM_BUF[6].cur,0,1000,0,10000},
	{&CAN2_RM_BUF[7].place,&CAN2_RM_BUF[7].vel,&CAN2_RM_BUF[7].cur,0,1000,0,10000},
};
//初始化定义pid
struct PID pids1,pidp1,pids2,pidp2,pids3,pidp3,pid4,pid5,pid6,pid7,pid8; //rm电机pid，在rm_motor里面extern

PID_POS_VEL pid1;

/******************************************任务句柄************************************************/
void*Hand_Task_Handler;
void*Test_Task_Handler;
void*Control_Task_Handler;
void*ActionStart_Task_Handler;
void*motor_Task_Handler;

void Hand_Task()
{
	while(1)
	{
		USARTx_Rx(7,com_buf,10);//读取usart的数据
		
//		Uart_Printf(7,"111%c\r\n",Rx_event[0]);
		vTaskDelay(20);
	}
}

void Test_Task()//打印数据
{
	while(1)
	{
		Uart_Printf(7,"%d,%d,%d,%d,%d,%d,%d,%d\r\n",PushMotora[0].Aim_Place,*PushMotora[0].Now_Place,PushMotora[1].Aim_Place,*PushMotora[1].Now_Place,
		PushMotora[2].Aim_Place,*PushMotora[2].Now_Place,PushMotora[3].Aim_Place,*PushMotora[3].Now_Place);
		
		Uart_Printf(7,"111%d\n",Rx_event[0]);
		
	    Uart_Printf(7,"num%d\n",target_speed);
		Uart_Printf(7,"cnum%d\n",target_angle);
		
	    Uart_Printf(7, "%d,%d,%d,%d,%d,%d,%d,%d\r\n",PushMotora[4].Aim_Speed,*PushMotora[4].Now_Speed,PushMotora[5].Aim_Speed,*PushMotora[5].Now_Speed,
		PushMotora[6].Aim_Speed,*PushMotora[6].Now_Speed,PushMotora[7].Aim_Speed,*PushMotora[7].Now_Speed);
		
		vTaskDelay(50);
	}
}

void Control_Task()
{
//    char buf[16];//
	char temp_speed[5]={0};
	char temp_angle[5]={0};

	uint16_t move_times=0;
	while(1)
	{
		strncpy(temp_speed,&com_buf[2],4);//temp得到com_buf[2]-com_buf[5]
		temp_speed[4]='\0';
		target_speed =atoi(temp_speed);//target_speed
		
		strncpy(temp_angle,&com_buf[7],3);//temp得到com_buf[7]-com_buf[9]
		temp_speed[3]='\0';
		target_angle=atoi(temp_angle);//target_angle
		
		if(com_buf[0]=='1')//全向轮控制
		{
			omni(PushMotora,target_speed,target_angle,0);
		}
		else if(com_buf[0]=='2'&&move_times<=50)//抬升机构
		{
			PushMotora[0].Aim_Place+=2000,PushMotora[1].Aim_Place+=2000;
	
			move_times++;
			if(move_times==50)
			{
				com_buf[0]=0;
				move_times=0;
			}
		}
		else if(com_buf[0]=='3'&&move_times<=50)//抬升机构
		{
			PushMotora[0].Aim_Place-=2000,PushMotora[1].Aim_Place-=2000;

			move_times++;
			if(move_times==50)
			{
				com_buf[0]=0;
				move_times=0;
			}
		}
		else if(com_buf[0]=='4')//后轮移动
		{
			PushMotora[3].Aim_Speed+=500;
			com_buf[0]=0;
		}
		else if(com_buf[0]=='5')//刹车
		{
			PushMotora[3].Aim_Speed+=500;
			com_buf[0]=0;
		}
		
        vTaskDelay(100);      
	}
}

void motor_Task()
{
	int U1=0,U2=0,U3=0,U4=0;
	int U5=0,U6=0,U7=0,U8=0;
	
	TickType_t lasttick;
    lasttick = xTaskGetTickCount();
	
	while(1){
	//两个串级	
		U1=Pid_PlaceSerialSpeed(&pidp1, PushMotora[0].Aim_Place+*PushMotora[0].Zero_Place,
		*PushMotora[0].Now_Place,&pids1,PushMotora[0].Aim_Speed,*PushMotora[0].Now_Speed,PushMotora[0].Max_Current,10000);//
		
		U2=Pid_PlaceSerialSpeed(&pidp2, PushMotora[1].Aim_Place+*PushMotora[1].Zero_Place,
		*PushMotora[1].Now_Place,&pids2,PushMotora[1].Aim_Speed,*PushMotora[1].Now_Speed,PushMotora[1].Max_Current,10000);//
		
		U3=Pid_PlaceSerialSpeed(&pidp3, PushMotora[2].Aim_Place+*PushMotora[2].Zero_Place,
		*PushMotora[2].Now_Place,&pids3,PushMotora[2].Aim_Speed,*PushMotora[2].Now_Speed,PushMotora[2].Max_Current,10000);
				
		//底部轮速度
		U4=pid_speed_current(&pid4,PushMotora[3].Aim_Speed,*PushMotora[3].Now_Speed,1000);
		//全向轮
		U5=pid_speed_current(&pid5,PushMotora[4].Aim_Speed,*PushMotora[4].Now_Speed,1000);
		U6=pid_speed_current(&pid6,PushMotora[5].Aim_Speed,*PushMotora[5].Now_Speed,1000);
		U7=pid_speed_current(&pid7,PushMotora[6].Aim_Speed,*PushMotora[6].Now_Speed,1000);
		U8=pid_speed_current(&pid8,PushMotora[7].Aim_Speed,*PushMotora[7].Now_Speed,1000);
		
		RM_Tx(0x200,2,U1,U2,U3,U4);
		RM_Tx(0x1FF,1,U5,U6,U7,U8);
		
	    vTaskDelayUntil(&lasttick,4);
	}
}

/*******************************************************************条件延时*******************************************************************/


STATE_MISS  lift_state, down_state, recover_state;
STATE_Handle  Sta_Handler;  //状态机句柄

void ActionStart_Task()
{
	State_Handle_Init(Sta_Handler,recover_state);
	while(1)
	{
//		 StateRun(&Rx_event,0xFF,&Sta_Handler);//状态机运行：当前状态事件Rx_event，状态事件掩码0xFF，状态机句柄Sta_Handler
		//状态机的事件是通过全局变量Rx_event来传递的。在状态机运行函数StateRun中，我们传入Rx_event的地址
		//通过串口给Rx_event赋值
	}
}

#define task_num  4
Task_Mess R1_Task[task_num]={
{(TaskFunction_t)Test_Task,&Test_Task_Handler,256,4,"Test_Task",NULL},//测试任务
{(TaskFunction_t)Hand_Task,&Hand_Task_Handler,256,4,"Hand_Task",NULL},
{(TaskFunction_t)Control_Task,&Control_Task_Handler,256,4,"Control_Task",NULL},
{(TaskFunction_t)motor_Task,&motor_Task_Handler,256,3,"motor_Task",NULL},
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
	//底盘电机pid初始化
	 pid_init(&pid4,10,0.002,20);
	//全向轮pid初始化
     pid_init(&pid5,10,0.002,20);
	 pid_init(&pid6,10,0.002,20);
	 pid_init(&pid7,10,0.002,20);
	 pid_init(&pid8,10,0.002,20);
		
    while(1)
    {
		FreeRtos_Mode(R1_Task,task_num);
    }
}



