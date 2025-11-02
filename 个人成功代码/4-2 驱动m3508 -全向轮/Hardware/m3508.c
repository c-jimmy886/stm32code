#include "stm32f10x.h"                  // Device header
#include "m3508.h"
#include "can.h"
#include "pid.h"
#include <math.h>
#include <stdint.h>



extern volatile uint32_t system_time_ms;

void SCurve_Init(SCurvePlanner* planner, float accel, float decel, float max_vel)
{
    planner->acceleration = accel;
    planner->deceleration = decel;
    planner->max_velocity = max_vel;
    planner->target_velocity = 0;
    planner->current_velocity = 0;
    planner->last_time = system_time_ms;
}

float SCurve_Calculate(SCurvePlanner* planner, float target_vel)
{
    // 获取当前时间和时间差
    uint32_t current_time = system_time_ms;
    float dt = (current_time - planner->last_time) / 1000.0f; // 转换为秒
    
    // 如果时间差为0，直接返回当前速度
    if (dt <= 0) {
        return planner->current_velocity;
    }
    
    planner->last_time = current_time;
    
    // 更新目标速度
    planner->target_velocity = target_vel;
    
    // 计算速度误差
    float velocity_error = planner->target_velocity - planner->current_velocity;
    
    // 判断是需要加速还是减速
    if (fabsf(velocity_error) > 0.001f) {
        if (velocity_error > 0) {
            // 加速阶段
            planner->current_velocity += planner->acceleration * dt;
            
            // 限制速度不超过目标值和最大值
            if (planner->current_velocity > planner->target_velocity) {
                planner->current_velocity = planner->target_velocity;
            }
            if (planner->current_velocity > planner->max_velocity) {
                planner->current_velocity = planner->max_velocity;
            }
        } else {
            // 减速阶段
            planner->current_velocity -= planner->deceleration * dt;
            
            // 限制速度不低于目标值
            if (planner->current_velocity < planner->target_velocity) {
                planner->current_velocity = planner->target_velocity;
            }
        }
    }
    
    return planner->current_velocity;
}

void SCurve_Reset(SCurvePlanner* planner, float initial_velocity)
{
    planner->current_velocity = initial_velocity;
    planner->last_time = system_time_ms;
}


void M3508_Init(Motor_TypeDef* motor, uint16_t can_id)
{
    motor->angle = 0;
    motor->real_speed = 0;
    motor->current = 0;
    motor->temperature = 0;
    motor->total_angle = 0;
    motor->target_speed = 0;
    motor->can_id = can_id;
}
//现在只使用一个电机，0x201
void M3508_SetCurrent(int16_t current1, int16_t current2, int16_t current3, int16_t current4)
{
    uint8_t data[8];
    
    data[0] = current1 >> 8;
    data[1] = current1 & 0xFF;
    data[2] = current2 >> 8;
    data[3] = current2 & 0xFF;
    data[4] = current3 >> 8;
    data[5] = current3 & 0xFF;
    data[6] = current4 >> 8;
    data[7] = current4 & 0xFF;
    
    CAN_SendMessage(CAN_TX_ID, data, 8);
}

void M3508_SpeedControl(Motor_TypeDef* motor,Motor_TypeDef* motor1,Motor_TypeDef* motor2,Motor_TypeDef* motor3,
	void* pid,void* pid1,void* pid2,void* pid3, 
	SCurvePlanner* planner,SCurvePlanner* planner1,SCurvePlanner* planner2,SCurvePlanner* planner3)
{
    PID_TypeDef* speed_pid = (PID_TypeDef*)pid;
	PID_TypeDef* speed_pid1 = (PID_TypeDef*)pid1;
	PID_TypeDef* speed_pid2 = (PID_TypeDef*)pid2;
	PID_TypeDef* speed_pid3 = (PID_TypeDef*)pid3;
	
	
	float plan_speed=SCurve_Calculate(planner, motor->target_speed);
    // 计算PID输出
    float output = PID_Calculate(speed_pid,plan_speed, motor->real_speed);
	
	float plan_speed1=SCurve_Calculate(planner1, motor1->target_speed);
    // 计算PID输出
    float output1 = PID_Calculate(speed_pid1,plan_speed1, motor1->real_speed);
	
/*3*/float plan_speed2=SCurve_Calculate(planner2, motor2->target_speed);
        // 计算PID输出
     float output2 = PID_Calculate(speed_pid2,plan_speed2,motor2->real_speed);
	
	float plan_speed3=SCurve_Calculate(planner3, motor3->target_speed);
    // 计算PID输出
    float output3 = PID_Calculate(speed_pid3,plan_speed3, motor3->real_speed);
    
    // 发送电流值到电机
    M3508_SetCurrent((int16_t)output,(int16_t)output1,(int16_t)output2,(int16_t)output3);
}

