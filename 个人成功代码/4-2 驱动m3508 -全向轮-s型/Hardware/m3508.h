#ifndef __M3508_H
#define __M3508_H

#include <stdint.h>

typedef struct {
    uint16_t angle;
    int16_t real_speed;
    int16_t current;
    uint8_t temperature;
    int32_t total_angle;
    int32_t target_speed;
    uint16_t can_id;
} Motor_TypeDef;

typedef struct {
	
    float target_velocity;    // 目标速度
    float current_velocity;   // 当前规划速度
    float acceleration;       // 加速度
    float deceleration;       // 减速度
    float max_velocity;       // 最大允许速度
    uint32_t last_time;       // 上次更新时间
	
} SCurvePlanner;


void SCurve_Init(SCurvePlanner* planner, float accel, float decel, float max_vel);
float SCurve_Calculate(SCurvePlanner* planner, float target_vel);
void SCurve_Reset(SCurvePlanner* planner, float initial_velocity);

void M3508_Init(Motor_TypeDef* motor, uint16_t can_id);
void M3508_SetCurrent(int16_t current1, int16_t current2, int16_t current3, int16_t current4);
void M3508_SpeedControl(Motor_TypeDef* motor,Motor_TypeDef* motor1,Motor_TypeDef* motor2,Motor_TypeDef* motor3,
	void* pid,void* pid1,void* pid2,void* pid3,
	SCurvePlanner* planner,SCurvePlanner* planner1,SCurvePlanner* planner2,SCurvePlanner* planner3);

#endif

