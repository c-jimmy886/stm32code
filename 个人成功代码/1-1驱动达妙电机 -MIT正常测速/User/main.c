#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "timer.h"
#include "can.h"
#include "motor4310.h"
#include "Key.h"
#include "LED.h"
#include "usart.h"

extern volatile MotorFeedback motor_feedback;
uint8_t target_position=0;
uint8_t target_ves=0;


uint8_t open_flag=0;


int main(void)
{
//	OLED_Init();
    TIM3_Config(1000);//1m/x  //为什么修改这个频率可以读到can接收数据？？

	LED_Init();
	CAN_Config();
	Key_Init();
	Serial_Init();
	TIM_Cmd(TIM3, ENABLE);
	
	uint8_t keynum=0;
	uint16_t i=0;
	while (1)
	{
		uint16_t a=0;
		
		a=motor_feedback.position;
		
		Serial_Printf("%d,%lf\r\n",target_ves,motor_feedback.velocity);

		
		keynum=Key_GetNum();
		
		if(keynum==3)  //发送电机使能信号
		{
			open_flag=1;
			uint8_t d[8];
			d[0]=(uint8_t)0xFF;
			d[1]=(uint8_t)0xFF;
			d[2]=(uint8_t)0xFF;
			d[3]=(uint8_t)0xFF;
			d[4]=(uint8_t)0xFF;
			d[5]=(uint8_t)0xFF;
			d[6]=(uint8_t)0xFF;
			d[7]=(uint8_t)0xFC;
			
			CAN_SendMessage(0x01,d,8);   
			LED5_Turn();
			Delay_ms(100);
			
		}
	     if(keynum==4)  //发送电机失能信号
		{
			uint8_t c[8];
			c[0]=0xFF;
			c[1]=0xFF;
			c[2]=0xFF;
			c[3]=0xFF;
			c[4]=0xFF;
			c[5]=0xFF;
			c[6]=0xFF;
			c[7]=0xFD;
			
			CAN_SendMessage(0x01,c,8);
		}
		if(keynum==6&&open_flag==0)//零点设定，使能之前先把当前设置位零点
		{
			uint8_t d[8];
			d[0]=(uint8_t)0xFF;
			d[1]=(uint8_t)0xFF;
			d[2]=(uint8_t)0xFF;
			d[3]=(uint8_t)0xFF;
			d[4]=(uint8_t)0xFF;
			d[5]=(uint8_t)0xFF;
			d[6]=(uint8_t)0xFF;
			d[7]=(uint8_t)0xFE;
			
			CAN_SendMessage(0x01,d,8);   
			
			Delay_ms(100);
			LED5_Turn();
		}
		if(keynum==6&&open_flag!=0)//位置修改
		{
			target_position+=2;
			
			target_ves+=5;
		}
	}
}

// 定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
		 MotorControl_SendFrame(0x01,target_position,0.1,0.9,0.6,0.1);//参数控制  //位置环
//		 MotorControl_SendFrame(0x01,target_position,0.1,2.2,0.6,0.29);//参数控制  //位置环
//		 MotorControl_SendFrame(0x01,0,target_ves,0.0,1.35,0.2);//参数控制  //速度环
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

