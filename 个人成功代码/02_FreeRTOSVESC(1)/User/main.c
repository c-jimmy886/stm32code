#include "main.h"



int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); /* 4号分组方案 4 位抢占优先级， 0 位响应优先级 */
    SysTick_Init(72);       /* 滴答定时器初始化，带中断 */
    USART1_Init(115200);    /* 串口1初始化 115200 */
    Key_Init();
    LED_Init();
	
	
    freeRTOS_demo();        /* freeRTOS实验 */
	
}


