#include "stm32f10x.h"                  // Device header

#include "can.h"

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -65.0f
#define V_MAX 65.0f
#define T_MIN -18.0f
#define T_MAX 18.0f

volatile MotorFeedback motor_feedback;

void CAN_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // CAN_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; // CAN_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 4; // 36MHz/(1+5+3)/4 =1mbps
    CAN_Init(CAN1, &CAN_InitStructure);  //计算波特率
    
    //全通
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow =0x000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN_SendMessage(uint32_t id, uint8_t* data, uint8_t length)
{
    CanTxMsg TxMessage;
    
    TxMessage.StdId = id;
    TxMessage.ExtId = 0;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = length;
    
    for (uint8_t i = 0; i < length; i++)
    {
        TxMessage.Data[i] = data[i];
    }
    
    CAN_Transmit(CAN1, &TxMessage);
}

float uint_to_float(int x_int, float x_min, float x_max, int bits) {
  
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

 //修改后的CAN接收中断处理函数
void USB_LP_CAN1_RX0_IRQHandler(void) {
    CanRxMsg RxMessage;
    
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        
        if (RxMessage.StdId ==0x11) //配置对应的id，对应master id
		{
    
		uint16_t p_int = (RxMessage.Data[1] << 8) | RxMessage.Data[2];
		uint16_t v_int = (RxMessage.Data[3] << 4) | (RxMessage.Data[4] >> 4);
		uint16_t t_int = ((RxMessage.Data[4] & 0x0F) << 8) | RxMessage.Data[5];
		
		// 转换为实际物理值
		motor_feedback.position = uint_to_float(p_int, P_MIN, P_MAX, 16);
		motor_feedback.velocity = uint_to_float(v_int, V_MIN, V_MAX, 12);
		motor_feedback.torque   = uint_to_float(t_int, T_MIN, T_MAX, 12);
		
  
		motor_feedback.controller_id = RxMessage.Data[0] & 0x0F;
		motor_feedback.error_code = (RxMessage.Data[0] & 0xF0) >> 4; 
		motor_feedback.mos_temperature = RxMessage.Data[6]; 
        }
        
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}

float getcan_num(void)
{
	return motor_feedback.position;
}







