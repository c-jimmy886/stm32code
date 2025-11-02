#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "usart.h"
#include "dma.h"
#include "sys.h"

int main(void)
{
	
	LED_Init();
	Key_Init();
	Serial_Init();
	
	USART_TXRX_DMA_Init();
//	OLED_Init();
	
//	OLED_ShowChar(1, 1, 'A');
//	OLED_ShowString(1, 3, "HelloWorld!");
//	OLED_ShowNum(2, 1, 12345, 5);
//	OLED_ShowSignedNum(2, 7, -66, 2);
//	OLED_ShowHexNum(3, 1, 0xAA55, 4);
//	OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	while (1)
	{
//		Serial_Printf("222%s\r\n",usart_data);
//		Serial_Printf("111\r\n");
//		
//		DMA_Printf(USART1,"222%s\r\n",usart_data); 
		
//		Delay_ms(500);
		uint8_t keynum=0;
		keynum=Key_GetNum();
		
		if(keynum==3)
		{
			LED5_Turn();
		}
//		if(keynum==4)
//		{
//			LED5_Turn();
//		}
//		if(keynum==6)
//		{
//			LED5_Turn();
//		}
		
		Delay_ms(100);
	}
	
}


