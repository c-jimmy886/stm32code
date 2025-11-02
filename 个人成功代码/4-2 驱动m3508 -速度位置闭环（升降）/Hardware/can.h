#ifndef __CAN_H
#define __CAN_H

#include "m3508.h"

#define CAN_TX_ID    0x200   // ∑¢ÀÕCAN ID
#define CAN_RX_ID    0x201   // Ω” ’CAN ID

void CAN_Config(void);
void CAN_SendMessage(uint32_t id, uint8_t* data, uint8_t length);
void candata_process(CanRxMsg *RxMessage,Motor_TypeDef *motor);


#endif
