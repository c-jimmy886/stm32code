#include "stm32f10x.h"
#include "can.h"
#include "gpio.h"
#include "timer.h"
#include "pid.h"
#include "m3508.h"
#include "usart.h"
#include "Delay.h"
#include "Key.h"

#include <stdlib.h>


//usart接收到的数据
double  usart_data_p=0;
double  usart_data_i=0;

Motor_TypeDef motor;
PID_TypeDef speed_pid;

//升降系统四个电机
Motor_TypeDef Motor[4];
PID_TypeDef Speed_pid[4];

uint16_t  tim=0;
uint16_t  can=0;

uint8_t  move_flag=0;
uint8_t  move_time=0;

void process(char *string,double * data);

int main(void)
{

    SystemInit();
    
    GPIO_Config();
    CAN_Config();
    TIM3_Config(500); // 100Hz控制频率
    Serial_Init();
	Key_Init();
	//pid串级  1.5  0.01  0.1  1.5  0.004  0.5  20000  -20000
	//串级    7.0  0.11  1.0  1.6   0.007   2.4  20000  -20000    完整版 不会抖  力气大
//    PID_Init(&speed_pid,7.0f,0.11f,1.0f,1.6f,0.007f,2.4f,20000,-20000);//位置环调好了，速度环还不行
	
//   M3508_Init(&motor,0x203); // 电机CAN ID，上来先要找到对应地址
	//速度环  4.0  0.1  0.8
	//ad:当在接近的时候抖动厉害就单独增大d，但是增大d好像会影响i靠近目标值  2.4
	
	
	//升降系统初始化
	PID_Init(&Speed_pid[0],8.0f,0.1098f,1.0f,1.6f,0.007f,2.4f,20000,-20000);
	PID_Init(&Speed_pid[1],8.0f,0.1098f,1.0f,1.6f,0.007f,2.4f,20000,-20000);
	PID_Init(&Speed_pid[2],8.0f,0.1098f,1.0f,1.6f,0.007f,2.4f,20000,-20000);
	PID_Init(&Speed_pid[3],8.0f,0.1098f,1.0f,1.6f,0.007f,2.4f,20000,-20000);
	
	M3508_Init(&Motor[0],0x201); 
	M3508_Init(&Motor[1],0x202); 
	M3508_Init(&Motor[2],0x203); 
	M3508_Init(&Motor[3],0x204); 
	
    
    motor.target_speed =0; // 100RPM
	
	motor.target_position=0;

    TIM_Cmd(TIM3, ENABLE);
    uint8_t keynum=0;
	
    while (1)
    {
		
//      Serial_Printf("%d,%d,%d\r\n",motor.angle,motor.real_speed,motor.target_speed);
//		Serial_Printf("%d,%d,%f\r\n",motor.total_angle,motor.target_position,motor.current);
//		Serial_Printf("%.2lf,%.2lf\r\n",speed_pid.integral_a,speed_pid.integral);
//		Serial_Printf("%.2lf,%.2lf\r\n",speed_pid.output_a,speed_pid.output_s);
		
		//升降四电机
		Serial_Printf("A%d,%d\r\n",Motor[0].total_angle,Motor[0].target_position);
		Serial_Printf("B%d,%d\r\n",Motor[1].total_angle,Motor[1].target_position);
		Serial_Printf("C%d,%d\r\n",Motor[2].total_angle,Motor[2].target_position);
		Serial_Printf("D%d,%d\r\n",Motor[3].total_angle,Motor[3].target_position);
		
		
//		Serial_Printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",Motor[0].total_angle,Motor[0].target_position,
//		Motor[1].total_angle,Motor[1].target_position,Motor[2].total_angle,Motor[2].target_position,Motor[3].total_angle,Motor[3].target_position);
		
		
		if(Serial_RxFlag==1)
		{
			process(Serial_RxPacket,&usart_data_p);
			
			Serial_Printf("111%lf\r\n",usart_data_p);
			PID_Init(&speed_pid,0,0,0.0f,usart_data_p,usart_data_i,0.5,10000,-10000);
			
			Serial_RxFlag=0;
		}
		if(Serial_RxFlag==2)
		{
			process(Serial_RxPacket,&usart_data_i);
			
			Serial_Printf("222%lf\r\n",usart_data_i);
			PID_Init(&speed_pid,0,0,0.0f,usart_data_p,usart_data_i,0.5,10000,-10000);
			
			Serial_RxFlag=0;
		}
		
		
		keynum=Key_GetNum();
//		
		if(keynum==4)
		{
//          Motor[0].target_position+=500;
//			Motor[1].target_position+=500;
//			Motor[2].target_position+=500;
//			Motor[3].target_position+=500;
			
//			motor.target_position+=500;
			keynum=0;
			move_flag=1;
			move_time=0;
		}
		if(move_flag==1&&move_time<=25)
		{
			
		    Motor[0].target_position+=500;
			Motor[1].target_position+=500;
			Motor[2].target_position+=500;
			Motor[3].target_position+=500;
			
			move_time++;
			
		}
		else if(keynum==3)
		{

			motor.target_position-=500;
			keynum=0;
		}
//		
		Delay_ms(10);
    }
}

// 定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
		tim++;
       
//		OLED_ShowNum(2,1,motor.angle,3);
//        // 速度环控制
//       M3508_SpeedControl(&motor, &speed_pid);
		
		m3508_upstair(Motor,&Speed_pid[0],&Speed_pid[1],&Speed_pid[2],&Speed_pid[3]);
		
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

void process(char *string,double * data)
{
	*data=atof(string);
	
	Serial_Printf("%lf\r\n",*data);
}


