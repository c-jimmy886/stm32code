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

void M3508_Init(Motor_TypeDef* motor, uint16_t can_id);
void M3508_SetCurrent(int16_t current1, int16_t current2, int16_t current3, int16_t current4);
void M3508_SpeedControl(Motor_TypeDef* motor, void* pid);

#endif

