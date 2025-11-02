#include "stm32f10x.h"                  // Device header
#include "m3508.h"
#include "pid.h"
#include <math.h>
#define M_PI  3.14


extern Motor_TypeDef Motor[4];

extern CurveObjectType s_curve[4];

void omni(float target_speed,float target_angle,float yaw_target,
	void* pid,void* pid1,void* pid2,void* pid3)
{
	
	PID_TypeDef* speed_pid = (PID_TypeDef*)pid;
	PID_TypeDef* speed_pid1 = (PID_TypeDef*)pid1;
	PID_TypeDef* speed_pid2 = (PID_TypeDef*)pid2;
	PID_TypeDef* speed_pid3 = (PID_TypeDef*)pid3;
	
	static float last_target=0,last_target1=0,last_target2=0,last_target3=0;
	
	 float target_angle_rad = target_angle * M_PI / 180.0; 
	
	
	 if (yaw_target > M_PI) yaw_target -= 2 * M_PI;
     else if (yaw_target < -M_PI) yaw_target += 2 * M_PI;

//      target_speed*=63;
	
	 float vx = target_speed * cos(target_angle_rad);  // x轴方向速度
     float vy = target_speed * sin(target_angle_rad);  // y轴方向速度
	
	 double z_correction =0;

    z_correction=0;
	
	Motor[0].target_speed = vx  + vy - z_correction; // 左前轮;
    Motor[1].target_speed = vx  - vy + z_correction; // 右前轮;
    Motor[2].target_speed = -vx + vy - z_correction; // 左后轮;
    Motor[3].target_speed = -vx - vy + z_correction; // 右后轮;
	
//	void CurveObject_Init(CurveObjectType *curve, float startSpeed, float targetSpeed, 
//float stepSpeed, float speedMin, float speedMax,float flexible)

   if(Motor[0].target_speed!=last_target)
   {
	   s_curve[0].targetSpeed = Motor[0].target_speed;
       ResetCurvePlanning(&s_curve[0]);
   }
   if(Motor[1].target_speed!=last_target1)
   {
	   s_curve[1].targetSpeed = Motor[1].target_speed;
       ResetCurvePlanning(&s_curve[1]);
   }
   if(Motor[2].target_speed!=last_target2)
   {
	   s_curve[2].targetSpeed = Motor[2].target_speed;
       ResetCurvePlanning(&s_curve[2]);
	   
   }
   if(Motor[3].target_speed!=last_target3)
   {
	   s_curve[3].targetSpeed = Motor[3].target_speed;
       ResetCurvePlanning(&s_curve[3]);
   }
   
   CalCurveSPTA(&s_curve[0]);
   CalCurveSPTA(&s_curve[1]);
   CalCurveSPTA(&s_curve[2]);
   CalCurveSPTA(&s_curve[3]);
//	curve->currentSpeed
	
//	float output = PID_Calculate(speed_pid,  Motor[0].target_speed, Motor[0].real_speed);
//	float output1 = PID_Calculate(speed_pid1,Motor[1].target_speed, Motor[1].real_speed);
//	float output2 = PID_Calculate(speed_pid2,Motor[2].target_speed, Motor[2].real_speed);
//	float output3 = PID_Calculate(speed_pid3,Motor[3].target_speed, Motor[3].real_speed);
   
	float output = PID_Calculate(speed_pid,  s_curve[0].currentSpeed, Motor[0].real_speed);
	float output1 = PID_Calculate(speed_pid1,s_curve[1].currentSpeed, Motor[1].real_speed);
	float output2 = PID_Calculate(speed_pid2,s_curve[2].currentSpeed, Motor[2].real_speed);
	float output3 = PID_Calculate(speed_pid3,s_curve[3].currentSpeed, Motor[3].real_speed);
	                 
   
	last_target=Motor[0].target_speed;
	last_target1=Motor[1].target_speed;
    last_target2=Motor[2].target_speed;
    last_target3=Motor[3].target_speed;
	
	
    if(output>5000)
	{
       output=5000;
	}	
	if(output1>5000)
	{
       output1=5000;
	}	
	if(output2>5000)
	{
       output2=5000;
	}	
	if(output3>5000)
	{
       output3=5000;
	}
	
	if(output<-5000)
	{
       output=-5000;
	}
    if(output1<-5000)
	{
       output1=-5000;
	}	
	if(output2<-5000)
	{
       output2=-5000;
	}	
	if(output3<-5000)
	{
       output3=-5000;
	}		
	
	M3508_SetCurrent((int16_t)output,(int16_t)output1,(int16_t)output2,(int16_t)output3);
}



