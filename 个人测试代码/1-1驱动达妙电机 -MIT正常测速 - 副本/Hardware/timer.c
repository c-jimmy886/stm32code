#include "stm32f10x.h"                  // Device header
#include "timer.h"

void TIM3_Config(uint32_t freq)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    

    TIM_TimeBaseStructure.TIM_Period = (72000000 / 72) / freq - 1; // 72MHz/72=1MHz
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72·ÖÆµ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}


