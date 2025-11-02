#ifndef __VESC_DRIVER_H
#define __VESC_DRIVER_H

#include "stm32f10x.h"
#include <stdint.h>

// VESC CANÃüÁîÃ¶¾Ù
typedef enum {
    CAN_PACKET_SET_DUTY = 0,
    CAN_PACKET_SET_CURRENT = 1,
    CAN_PACKET_SET_CURRENT_BRAKE = 2,
    CAN_PACKET_SET_RPM = 3,
    CAN_PACKET_SET_POS = 4,
    CAN_PACKET_SET_CURRENT_REL = 10,
    CAN_PACKET_SET_CURRENT_BRAKE_REL = 11,
    CAN_PACKET_SET_CURRENT_HANDBRAKE = 12,
    CAN_PACKET_SET_CURRENT_HANDBRAKE_REL = 13,
} CAN_PACKET_ID;

// º¯ÊýÉùÃ÷
void VESC_Init(void);
void VESC_SetRPM(uint8_t vesc_id, int32_t rpm);
void VESC_SetCurrent(uint8_t vesc_id, float current);
void VESC_SetDutyCycle(uint8_t vesc_id, float duty);

#endif

