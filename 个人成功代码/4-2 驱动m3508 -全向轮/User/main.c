#include "stm32f10x.h"
#include "can.h"
#include "gpio.h"
#include "timer.h"
#include "pid.h"
#include "m3508.h"
#include "usart.h"
#include "Delay.h"
#include "Key.h"
#include "omni.h"

//#include "OLED.h"
PID_TypeDef speed_pid;
PID_TypeDef speed_pid1;
PID_TypeDef speed_pid2;
PID_TypeDef speed_pid3;
//电机1
Motor_TypeDef motor;
SCurvePlanner velocity_planner;
//电机2
Motor_TypeDef motor1;
SCurvePlanner velocity_planner1;
//电机3
Motor_TypeDef motor2;
SCurvePlanner velocity_planner2;
//电机4
Motor_TypeDef motor3;
SCurvePlanner velocity_planner3;

//全向轮电机数组
Motor_TypeDef  Motor[4];

//全向轮目标速度和目标角度
float target_speed=0;
float target_angle=0;


//速度规划用于计时
uint16_t  tim=0;
uint16_t  can=0;

volatile uint32_t system_time_ms = 0;



int main(void)
{

    SystemInit();
    
    GPIO_Config();
    CAN_Config();
    TIM3_Config(500); // 100Hz控制频率
	TIM2_Init();
	
	Key_Init();
	
    Serial_Init();
//	OLED_Init();
	//pid  1.0  0.11  0.6  10000  -10000
	//scurve  40  45   1000

	//pid初始化
    PID_Init(&speed_pid, 1.0f, 0.12f, 0.6f, 10000, -10000);
	PID_Init(&speed_pid1, 1.0f, 0.12f, 0.6f, 10000, -10000);
	PID_Init(&speed_pid2, 1.0f, 0.12f, 0.6f, 10000, -10000);
	PID_Init(&speed_pid3, 1.0f, 0.12f, 0.6f, 10000, -10000);
	
	//速度规划结构体初始化
    SCurve_Init(&velocity_planner, 40.0f, 45.0f, 1000.0f);
    SCurve_Init(&velocity_planner1, 40.0f, 45.0f, 1000.0f);
	SCurve_Init(&velocity_planner2, 40.0f, 45.0f, 1000.0f);
	SCurve_Init(&velocity_planner3, 40.0f, 45.0f, 1000.0f);
	
	//电机初始化，四个电机分别初始化
    M3508_Init(&motor,0x201);  // 电机CAN ID，上来先要找到对应地址
    M3508_Init(&motor1,0x202); // 电机CAN ID，上来先要找到对应地址
	M3508_Init(&motor2,0x203); // 电机CAN ID，上来先要找到对应地址   dddd
	M3508_Init(&motor3,0x204); // 电机CAN ID，上来先要找到对应地址
	
	//全向轮电机初始化
	M3508_Init(&Motor[0],0x201);
	M3508_Init(&Motor[1],0x202);
    M3508_Init(&Motor[2],0x203);
	M3508_Init(&Motor[3],0x204);
	
	
    motor.target_speed =0; // 100RPM

    TIM_Cmd(TIM3, ENABLE);
	
	uint8_t keynum=0;
    
    while (1)
    {
//		Serial_Printf("aaa\r\n");
//        Serial_Printf("%d,%d,%d\r\n",motor.angle,motor.real_speed,motor.target_speed);
//		Serial_Printf("%d,%d\r\n",motor.real_speed,motor.target_speed);
//		Serial_Printf("%d,%d\r\n",motor1.real_speed,motor1.target_speed);
//		Serial_Printf("%d,%d\r\n",motor2.real_speed,motor2.target_speed);
//		Serial_Printf("%d,%d\r\n",motor3.real_speed,motor3.target_speed);
		
		
//		Serial_Printf("%d,%d,%lf\r\n",Motor[0].real_speed,Motor[0].target_speed,target_speed);
		Serial_Printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",Motor[0].real_speed,Motor[0].target_speed,Motor[1].real_speed,Motor[1].target_speed,
		Motor[2].real_speed,Motor[2].target_speed,Motor[3].real_speed,Motor[3].target_speed);
//		Serial_Printf("%d,%d\r\n",Motor[1].real_speed,Motor[1].target_speed);
//		Serial_Printf("%d,%d\r\n",Motor[2].real_speed,Motor[2].target_speed);
//		Serial_Printf("%d,%d\r\n",Motor[3].real_speed,Motor[3].target_speed);
//		
		
		keynum=Key_GetNum();
		if(keynum==3)
		{
			motor.target_speed +=300;
			motor1.target_speed +=300;
			motor2.target_speed +=300;
			motor3.target_speed +=300;
			
			Motor[0].target_speed+=300;
		}
		else if(keynum==4)
		{
			//四个电机驱动测试
			motor.target_speed  +=300;
			motor1.target_speed +=300;
			motor2.target_speed +=300;
			motor3.target_speed +=300;
			
			target_speed+=300;
			
			target_angle+=15;
		
		}
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
		
//        M3508_SpeedControl(&motor,&motor1,&motor2,&motor3,
//		&speed_pid,&speed_pid1,&speed_pid2,&speed_pid3,
//		&velocity_planner,&velocity_planner1,&velocity_planner2,&velocity_planner3);

			//全向轮底盘
		omni(target_speed,target_angle,0,
		&speed_pid,&speed_pid1,&speed_pid2,&speed_pid3);
		
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        system_time_ms++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}


