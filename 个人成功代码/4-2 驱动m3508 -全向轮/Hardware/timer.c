#include "stm32f10x.h"                  // Device header
#include "timer.h"

void TIM3_Config(uint32_t freq)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    

    TIM_TimeBaseStructure.TIM_Period = (72000000 / 72) / freq - 1; // 72MHz/72=1MHz
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}

void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 启用TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器基础设置
    // 假设系统时钟72MHz，预分频72-1，计数器频率为1MHz
    // 每1ms中断一次 (1000Hz)
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 启用TIM2更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 启用TIM2
    TIM_Cmd(TIM2, ENABLE);
}

