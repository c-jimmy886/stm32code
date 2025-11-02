#ifndef __CAN_H
#define __CAN_H



#define CAN_TX_ID    0x200   // ∑¢ÀÕCAN ID
#define CAN_RX_ID    0x201   // Ω” ’CAN ID

typedef struct {
    uint8_t controller_id;
    uint8_t error_code;
    float position;
    float velocity;
    int16_t torque;
    uint8_t mos_temperature;
    uint8_t rotor_temperature;
} MotorFeedback;

extern volatile MotorFeedback motor_feedback;


void CAN_Config(void);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void CAN_SendMessage(uint32_t id, uint8_t* data, uint8_t length);
float getcan_num(void);


#endif

