#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "can.h"
#include "m3508.h"
//#include "OLED.h"

extern Motor_TypeDef motor;

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
    

    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = CAN_RX_ID << 5;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x7FF;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
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


//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//    CanRxMsg RxMessage;
//    
//    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
//    {
//        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

//        // 解析电机反馈数据
//        if (RxMessage.StdId == motor.can_id)
//        {
//            motor.angle = (RxMessage.Data[0] << 8) | RxMessage.Data[1];
//            motor.real_speed = (RxMessage.Data[2] << 8) | RxMessage.Data[3];
//            motor.current = (RxMessage.Data[4] << 8) | RxMessage.Data[5];
//            motor.temperature = RxMessage.Data[6];
//            
//			
//            // 计算总角度（处理溢出）
//            static uint16_t last_angle = 0;
//            static int32_t total_angle = 0;
//            int16_t delta_angle = motor.angle - last_angle;
//            
//            if (delta_angle > 4096) {
//                delta_angle -= 8192;
//            } else if (delta_angle < -4096) {
//                delta_angle += 8192;
//            }
//            
//            total_angle += delta_angle;
//            last_angle = motor.angle;
//            motor.total_angle = total_angle;
//			
//			
//        }
//        
//        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
//    }
//}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        
        // 解析电机反馈数据
        if (RxMessage.StdId == motor.can_id)
        {
			static uint16_t initial_angle = 0;
			static int32_t round_count=0;
			static uint16_t last_angle=0;
			static uint8_t first_read=1;
			static int32_t total_angle = 0;
			
            motor.angle = (RxMessage.Data[0] << 8) | RxMessage.Data[1];        //角度
            motor.real_speed = (RxMessage.Data[2] << 8) | RxMessage.Data[3];   //速度
            motor.current = (RxMessage.Data[4] << 8) | RxMessage.Data[5];      //电流
            motor.temperature = RxMessage.Data[6];

            // 计算总角度（处理溢出）
			
			if(first_read)
			{
				initial_angle = motor.angle;
				round_count=0;
				last_angle=motor.angle;
				motor.total_angle=0;
				total_angle=0;
				first_read=0;
			}
			else{
			   int16_t delta_angle = motor.angle - last_angle; //实际角度减去之前角度
				
			   if (delta_angle > 4096) {
                round_count--;
               } 
			   else if (delta_angle < -4096)
			   {
                round_count++;
			    }
			   
			    motor.total_angle = round_count * 8192 + (motor.angle - initial_angle);
			}

            last_angle = motor.angle;
            motor.total_angle = total_angle;
	
        }
        
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}




