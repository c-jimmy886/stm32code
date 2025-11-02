#ifndef __VESC_H
#define __VESC_H
#include "can.h"
//#include "pid.h"

/*VESC命令*/
typedef enum {
  CAN_PACKET_SET_DUTY = 0,
  CAN_PACKET_SET_CURRENT,
  CAN_PACKET_SET_CURRENT_BRAKE,
  CAN_PACKET_SET_RPM,
  CAN_PACKET_SET_POS,
  CAN_PACKET_FILL_RX_BUFFER,
  CAN_PACKET_FILL_RX_BUFFER_LONG,
  CAN_PACKET_PROCESS_RX_BUFFER,
  CAN_PACKET_PROCESS_SHORT_BUFFER,
  CAN_PACKET_STATUS,
  CAN_PACKET_SET_CURRENT_REL,
  CAN_PACKET_SET_CURRENT_BRAKE_REL,
  CAN_PACKET_SET_CURRENT_HANDBRAKE,
  CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
  CAN_PACKET_STATUS_2,
  CAN_PACKET_STATUS_3,
  CAN_PACKET_STATUS_4,
  CAN_PACKET_PING,
  CAN_PACKET_PONG,
  CAN_PACKET_DETECT_APPLY_ALL_FOC,
  CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
  CAN_PACKET_CONF_CURRENT_LIMITS,
  CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
  CAN_PACKET_CONF_CURRENT_LIMITS_IN,
  CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
  CAN_PACKET_CONF_FOC_ERPMS,
  CAN_PACKET_CONF_STORE_FOC_ERPMS,
  CAN_PACKET_STATUS_5,
  CAN_PACKET_POLL_TS5700N8501_STATUS
} VESC_CAN_COMMAND_ID;



typedef struct 
{
	  float all_round;
		float offset_round;
		float target_speed;
		float leida_angle;
		float leida_x;
		float leida_y;
		float leida_target_angle;
    int32_t RPM;
		int32_t angle_error;
		int32_t way_error;
	  int16_t offset_angle;
	  int16_t NowAngle;
	  int16_t LastAngle;
      int16_t all_angle;
		int16_t round_cnt;
//		S_Curve S_Curve;
//		T_Curve T_Curve;
//		pid_control pid_control;
//		coordinate coordinate;
}Chassis_MOTO;


extern Chassis_MOTO MOTO[4];
extern int first_state[4];//一级状态（解锁或锁定）


void VESC_RX(CanRxMsg *msg);
void VESC_TX(uint8_t controller_id, VESC_CAN_COMMAND_ID mode,int32_t Temp_PWM1);
void vesc_pos(int id ,int speed,float pos,Chassis_MOTO MOTO,float cnt);//vesc位置环
void vesc_lock(int id,Chassis_MOTO*MOTO);

#endif 



