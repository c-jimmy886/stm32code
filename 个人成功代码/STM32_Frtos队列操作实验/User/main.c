#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "key.h"

TaskHandle_t myTask1Handler;
TaskHandle_t myTask3Handler;

//创建队列
QueueHandle_t  key_queue;      //小数据
QueueHandle_t  big_data_queue; //大数据

char buff[100]={"1234567"};
	

void myTask1(void *arg)
{		
	uint8_t key=0;
	char *buf;
	BaseType_t  error=0;
	buf=buff;    //buf=&buff[0]
	while(1)
	{
		key=Key_GetNum();
		if(key==1)
		{
			//         待写入队列  写入的值   等待时长
			error=xQueueSend(key_queue,&key,portMAX_DELAY);
			if(error!=pdTRUE)
			{
				
			}
		}
		else if(key==2)
		{
			xQueueSend(big_data_queue,&buf,portMAX_DELAY);
		}
		vTaskDelay(10);
	}
}

void myTask3(void *arg)
{		
	uint8_t key=0;
	BaseType_t  error=0;
	while(1)
	{
		xQueueReceive(key_queue,&key,portMAX_DELAY);
		if(error!=pdTRUE)
		{
			
			
		}
		
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
	 
	key_queue=xQueueCreate(2,sizeof(uint8_t));  //创建成功等于句柄，创建失败等于null
	 
	big_data_queue=xQueueCreate(1,sizeof(char *));  //因为是char的字符串
	 
	 
	vTaskStartScheduler();

	while (1)
	{
		
	}
}
