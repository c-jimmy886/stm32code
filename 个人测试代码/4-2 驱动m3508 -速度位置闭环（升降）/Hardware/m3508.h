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
	int32_t target_position;
    uint16_t can_id;
	
	//candata结构体处理
	uint16_t canlast_angle;  //初始值0
	uint8_t canfirst_read;  //初始值为1
	int32_t cantotal_angle;   //初始值0
	
	
} Motor_TypeDef;

void M3508_Init(Motor_TypeDef* motor, uint16_t can_id);
void M3508_SetCurrent(int16_t current1, int16_t current2, int16_t current3, int16_t current4);
void M3508_SetCurrent1(int16_t current1, int16_t current2, int16_t current3, int16_t current4);
void M3508_SpeedControl(Motor_TypeDef* motor, void* pid);
void m3508_upstair(Motor_TypeDef Motor[], void* pid, void* pid1, void* pid2, void* pid3);
void m3508_move(Motor_TypeDef Motor[], void* pid, void* pid1, void* pid2, void* pid3);



#endif


