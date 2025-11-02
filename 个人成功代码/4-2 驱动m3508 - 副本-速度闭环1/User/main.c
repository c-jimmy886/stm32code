#include "stm32f10x.h"
#include "can.h"
#include "gpio.h"
#include "timer.h"
#include "pid.h"
#include "m3508.h"
#include "usart.h"
#include "Delay.h"
//#include "OLED.h"
#include "Key.h"

Motor_TypeDef motor;
PID_TypeDef speed_pid;

uint16_t  tim=0;
uint16_t  can=0;
    uint8_t keynum=0;
	
int main(void)
{

    SystemInit();
    
    GPIO_Config();
    CAN_Config();
    TIM3_Config(500); // 100Hz控制频率
    Serial_Init();
//	OLED_Init();
    Key_Init();
	//1.0  0.11  0.6
    PID_Init(&speed_pid, 8.1f, 0.1011f,1.0f, 20000, -20000);
    

    M3508_Init(&motor,0x202); // 电机CAN ID，上来先要找到对应地址
    
    motor.target_speed =0; // 100RPM

    TIM_Cmd(TIM3, ENABLE);
    
    while (1)
    {
//		Serial_Printf("aaa\r\n");
//        Serial_Printf("%d,%d,%d\r\n",motor.angle,motor.real_speed,motor.target_speed);
		Serial_Printf("%d,%d\r\n",motor.real_speed,motor.target_speed);

		
		keynum=Key_GetNum();
//		
		if(keynum==4)
		{
			motor.target_speed +=1000;

			keynum=0;
		}
		else if(keynum==3)
		{
			motor.target_speed -=100;

			keynum=0;
		}
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
       M3508_SpeedControl(&motor, &speed_pid);//这个容易卡住
		
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

