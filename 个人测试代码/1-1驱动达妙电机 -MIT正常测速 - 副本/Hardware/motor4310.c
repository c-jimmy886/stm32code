#include "motor4310.h"
#include "can.h"

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -65.0f
#define V_MAX 65.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -18.0f
#define T_MAX 18.0f

int16_t position_desired = 0;
int16_t velocity_desired = 200;
uint16_t kp = 3;  // 中间值，约250
uint16_t kd =1;  // 中间值，约2.5
int16_t torque_ff = 0;

int float_to_uint(float x, float x_min, float x_max, int bits) 
{
    float span = x_max - x_min;
    float offset = x_min;
    return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

void MotorControl_SendFrame(uint8_t can_id, float pos, float vel, float kp, float kd, float torq) 
{
    uint8_t data[8];
    
    // 将浮点参数转换为整数值
    uint16_t pos_tmp = float_to_uint(pos, P_MIN, P_MAX, 16);
    uint16_t vel_tmp = float_to_uint(vel, V_MIN, V_MAX, 12);
    uint16_t kp_tmp = float_to_uint(kp, KP_MIN, KP_MAX, 12);
    uint16_t kd_tmp = float_to_uint(kd, KD_MIN, KD_MAX, 12);
    uint16_t tor_tmp = float_to_uint(torq, T_MIN, T_MAX, 12);

    // 构建数据帧
    data[0] = (uint8_t)(pos_tmp >> 8);
    data[1] = (uint8_t)(pos_tmp);
    data[2] = (uint8_t)(vel_tmp >> 4);
    data[3] = ((uint8_t)(vel_tmp & 0x0F) << 4) | ((uint8_t)(kp_tmp >> 8) & 0x0F);
    data[4] = (uint8_t)(kp_tmp);
    data[5] = (uint8_t)(kd_tmp >> 4);
    data[6] = ((uint8_t)(kd_tmp & 0x0F) << 4) | ((uint8_t)(tor_tmp >> 8) & 0x0F);
    data[7] = (uint8_t)(tor_tmp);

    // 发送CAN帧
    CAN_SendMessage(can_id,data, 8);
}



