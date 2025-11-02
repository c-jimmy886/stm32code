#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "timer.h"
#include "can.h"
#include "motor4310.h"
#include "Key.h"
#include "LED.h"
#include "usart.h"
#include "VESC.h"

//达妙
extern volatile MotorFeedback motor_feedback;
uint8_t target_position=0;
uint8_t target_ves=0;

//VESC
uint8_t  id=0;
uint8_t  i=0;

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
		if(keynum==3)  //发送解锁信号
		{
			i++;
			first_state[0]=0;  //解锁，解锁才能动
			LED5_Turn();
		}

		if(keynum==6)
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
		
//		 MotorControl_SendFrame(0x01,target_position,0.1,2.2,0.6,0.29);//参数控制  //位置环
//		 MotorControl_SendFrame(0x01,0,target_ves,0.0,1.35,0.2);//参数控制  //速度环
		
		//VESC
		
		 vesc_pos(id,2000,2.5*i,MOTO[id],0.5);
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

