#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"

TaskHandle_t myTask1Handler;
TaskHandle_t myTask3Handler;

void myTask1(void *arg)
{		while(1)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			vTaskDelay(500);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			vTaskDelay(500);
		}
}

void myTask3(void *arg)
{		while(1)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			vTaskDelay(300);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			vTaskDelay(800);
		}
}


int main(void)
 {
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//??GPIOA???
	
	/*GPIO???*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//?PA1?PA2??????????
	 
	xTaskCreate(myTask1,"myTask1",256,NULL,2,&myTask1Handler);
	 xTaskCreate(myTask3,"myTask3",256,NULL,2,&myTask3Handler);
	vTaskStartScheduler();

	while (1)
	{
		
	}
}
