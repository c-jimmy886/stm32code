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


RM_RX_DATA CAN2_RM_BUF[8]={{0x201,-1},{0x202,-1},{0x203,-1},{0x204,-1},{0x205,-1},{0x206,-1},{0x207,-1},{0x208,-1}};//RM电机接收数据

DM_RX_DATA CAN1_DM_BUF[4]={{0x11,-1},{0x11,-1}};

//推射2006数据结构体
struct PUSH_MOTOR
{
    int       *Now_Place;//当前位置
    int16_t   *Now_Speed;//当前速度
    int16_t   *Now_Current;//当前电流
    int       *Zero_Place;//零点位置
    int       Aim_Place;//目标位置
    int       Aim_Speed;//目标速度
    int       Max_Current;//最大电流
};

//升降机构
struct PUSH_MOTOR PushMotor ={&CAN2_RM_BUF[0].place,&CAN2_RM_BUF[0].vel,&CAN2_RM_BUF[0].cur,&CAN2_RM_BUF[0].zero_angle,0,10000,20000};
struct PUSH_MOTOR PushMotor1={&CAN2_RM_BUF[1].place,&CAN2_RM_BUF[1].vel,&CAN2_RM_BUF[1].cur,&CAN2_RM_BUF[1].zero_angle,0,10000,20000};
struct PUSH_MOTOR PushMotor2={&CAN2_RM_BUF[2].place,&CAN2_RM_BUF[2].vel,&CAN2_RM_BUF[2].cur,&CAN2_RM_BUF[2].zero_angle,0,10000,20000};
struct PUSH_MOTOR PushMotor3={&CAN2_RM_BUF[3].place,&CAN2_RM_BUF[3].vel,&CAN2_RM_BUF[3].cur,&CAN2_RM_BUF[3].zero_angle,0,10000,20000};
//底盘电机                    //当前位置             //当前速度          //当前电流    //零点位置//目标位置//目标速度 //最大电流
struct PUSH_MOTOR PushMotor4={&CAN2_RM_BUF[4].place,&CAN2_RM_BUF[4].vel,&CAN2_RM_BUF[4].cur,0,1000,0,10000};
struct PUSH_MOTOR PushMotor5={&CAN2_RM_BUF[5].place,&CAN2_RM_BUF[5].vel,&CAN2_RM_BUF[5].cur,0,1000,0,10000};
struct PUSH_MOTOR PushMotor6={&CAN2_RM_BUF[6].place,&CAN2_RM_BUF[6].vel,&CAN2_RM_BUF[6].cur,0,1000,0,10000};
struct PUSH_MOTOR PushMotor7={&CAN2_RM_BUF[7].place,&CAN2_RM_BUF[7].vel,&CAN2_RM_BUF[7].cur,0,1000,0,10000};

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

struct PID pids1,pidp1,pids2,pidp2,pids3,pidp3,pids4,pidp4,pid5,pid6,pid7,pid8;

PID_POS_VEL pid1;

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
	Tim_Init(3,2000,120,Up);//用于定时更新速度规划
	RM_Init(2,CAN2_RM_BUF,8);
	
	RM_Init(1,CAN2_RM_BUF,8);
	
	DM_Motor_Init(1, CAN1_DM_BUF,9,0x00);
	
	dma_uart_init(1,115200);
	dma_uart_init(7,115200);//调试串口
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
	
	char buf[16];
	
	int move_time=0;
    while(1)
    {
		nvic_irq_enable(TMR3_GLOBAL_IRQn,1, 0);
		delay_ms(10);
//      dma_printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",*PushMotor.Now_Place,*PushMotor1.Now_Place,*PushMotor2.Now_Place,*PushMotor3.Now_Place,
//		PushMotor.Aim_Place+*PushMotor.Zero_Place,PushMotor1.Aim_Place+*PushMotor1.Zero_Place,PushMotor2.Aim_Place+*PushMotor2.Zero_Place,PushMotor3.Aim_Place+*PushMotor3.Zero_Place,
//		PushMotor4.Aim_Speed,*PushMotor4.Now_Speed,PushMotor6.Aim_Speed,*PushMotor6.Now_Speed);

		dma_printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",*PushMotor4.Now_Speed,PushMotor4.Aim_Speed,*PushMotor5.Now_Speed,PushMotor5.Aim_Speed,
		*PushMotor6.Now_Speed,PushMotor6.Aim_Speed,*PushMotor7.Now_Speed,PushMotor7.Aim_Speed);
		
//	    dma_printf("%d,%d,%d,%d\r\n",*PushMotor.Now_Current,*PushMotor1.Now_Current,*PushMotor2.Now_Current,*PushMotor3.Now_Current);

		dma_rx(7,buf,13);
		
		
		if(buf[0]=='1'&&move_time<=150)//
		{
			PushMotor.Aim_Place+=2000;    PushMotor2.Aim_Place+=2000;
		    PushMotor1.Aim_Place+=2000;   PushMotor3.Aim_Place+=2000;
			move_time++;
			if(move_time==150)
			{
				buf[0]=0;
				move_time=0;
			}
		}
		if(buf[0]=='2'&&move_time<=150)//脚抬
		{
			PushMotor.Aim_Place -=2000;   PushMotor2.Aim_Place-=2000;
		    PushMotor1.Aim_Place-=2000;   PushMotor3.Aim_Place-=2000;
			move_time++;
			if(move_time==150)
			{
				buf[0]=0;
				move_time=0;
			}
		}
		if(buf[0]=='3'&&move_time<=80)
		{
			PushMotor.Aim_Place +=500;   PushMotor1.Aim_Place+=500;
			move_time++;
			if(move_time==80)
			{
				buf[0]=0;
				move_time=0;
			}
		}
		if(buf[0]=='4'&&move_time<=150)
		{
			PushMotor.Aim_Place -=2000;   PushMotor1.Aim_Place-=2000;
			move_time++;
			if(move_time==150)
			{
				buf[0]=0;
				move_time=0;
			}
		}
		if(buf[0]=='5')
		{
		    PushMotor4.Aim_Speed+=500;    PushMotor5.Aim_Speed+=500;
			PushMotor6.Aim_Speed+=500;    PushMotor7.Aim_Speed+=500;
			
			buf[0]=0;
		}
		if(buf[0]=='6')
		{
		    PushMotor4.Aim_Speed=0;   PushMotor5.Aim_Speed=0;
			PushMotor6.Aim_Speed=0;   PushMotor7.Aim_Speed=0;
            buf[0]=0;
		}

		
    }
    
}

void TMR3_GLOBAL_IRQHandler(void)
{
	int U1=0,U2=0,U3=0,U4=0;
	int U5=0,U6=0,U7=0,U8=0;
    if(tmr_flag_get(TMR3, TMR_OVF_FLAG) != RESET)
    {
		U1=Pid_PlaceSerialSpeed(&pidp1, PushMotor.Aim_Place+*PushMotor.Zero_Place,
		*PushMotor.Now_Place,&pids1,PushMotor.Aim_Speed,*PushMotor.Now_Speed,PushMotor.Max_Current,10000);
		
		U2=Pid_PlaceSerialSpeed(&pidp2, PushMotor1.Aim_Place+*PushMotor1.Zero_Place,
		*PushMotor1.Now_Place,&pids2,PushMotor1.Aim_Speed,*PushMotor1.Now_Speed,PushMotor1.Max_Current,10000);
		
		U3=Pid_PlaceSerialSpeed(&pidp3, PushMotor2.Aim_Place+*PushMotor2.Zero_Place,
		*PushMotor2.Now_Place,&pids3,PushMotor2.Aim_Speed,*PushMotor2.Now_Speed,PushMotor2.Max_Current,10000);
				
        U4=Pid_PlaceSerialSpeed(&pidp4, PushMotor3.Aim_Place+*PushMotor3.Zero_Place,
		*PushMotor3.Now_Place,&pids4,PushMotor3.Aim_Speed,*PushMotor3.Now_Speed,PushMotor3.Max_Current,10000);
//		
////	U5=pid_pos_vel(&pid1,8,1.1,2,50,20,8000,*PushMotor4.Now_Speed,PushMotor4.Aim_Place,*PushMotor4.Now_Current,17000,0);
//	
		U5=pid_speed_current(&pid5,PushMotor4.Aim_Speed,*PushMotor4.Now_Speed,1000);
        U6=pid_speed_current(&pid6,PushMotor5.Aim_Speed,*PushMotor5.Now_Speed,1000);
		U7=pid_speed_current(&pid7,PushMotor6.Aim_Speed,*PushMotor6.Now_Speed,1000);
		U8=pid_speed_current(&pid8,PushMotor7.Aim_Speed,*PushMotor7.Now_Speed,1000);
//		
		RM_Tx(0x200,2,U1,U2,U3,U4);
		
		RM_Tx(0x1FF,1,U5,U6,U7,U8);
		
//		//达妙电机
//		DM_Motor_Tx(0x01,20,200,1,1,1);
//		
//		DM_Motor_Cmd(0x01,DM_ENABLE);
		
        tmr_flag_clear(TMR3, TMR_OVF_FLAG);//清除标志位
    }
}



