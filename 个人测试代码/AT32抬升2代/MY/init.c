#include<init.h>
#include<sys.h>
#include<string.h>
#include<delay.h>
#include<usart.h>
#include<dma_usart.h>
#include<stdio.h>
#include<stdarg.h>


////================================================================GPIO_PIN================================================================\\








void Gpio_Init(char GPIO,int PIN,int GPIO_MODE,int GPIO_OUTPUT,int GPIO_PULL,int GPIO_DRIVE)//IO口初始化
{
 
    gpio_init_type gpio_init_struct;
    
    if(GPIO=='A')  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK,TRUE);
    if(GPIO=='B')  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK,TRUE);
    if(GPIO=='C')  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK,TRUE);
    if(GPIO=='D')  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK,TRUE);
    if(GPIO=='E')  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK,TRUE);

    
    
    if(PIN==0)   gpio_init_struct.gpio_pins = GPIO_PINS_0;
    if(PIN==1)   gpio_init_struct.gpio_pins = GPIO_PINS_1;
    if(PIN==2)   gpio_init_struct.gpio_pins = GPIO_PINS_2;
    if(PIN==3)   gpio_init_struct.gpio_pins = GPIO_PINS_3;
    if(PIN==4)   gpio_init_struct.gpio_pins = GPIO_PINS_4;
    if(PIN==5)   gpio_init_struct.gpio_pins = GPIO_PINS_5;
    if(PIN==6)   gpio_init_struct.gpio_pins = GPIO_PINS_6;
    if(PIN==7)   gpio_init_struct.gpio_pins = GPIO_PINS_7;
    if(PIN==8)   gpio_init_struct.gpio_pins = GPIO_PINS_8;
    if(PIN==9)   gpio_init_struct.gpio_pins = GPIO_PINS_9;
    if(PIN==10)  gpio_init_struct.gpio_pins = GPIO_PINS_10;
    if(PIN==11)  gpio_init_struct.gpio_pins = GPIO_PINS_11;
    if(PIN==12)  gpio_init_struct.gpio_pins = GPIO_PINS_12;
    if(PIN==13)  gpio_init_struct.gpio_pins = GPIO_PINS_13;
    if(PIN==14)  gpio_init_struct.gpio_pins = GPIO_PINS_14;
    if(PIN==15)  gpio_init_struct.gpio_pins = GPIO_PINS_15;
    
    gpio_init_struct.gpio_mode=(gpio_mode_type)GPIO_MODE;//GPIO模式
    gpio_init_struct.gpio_out_type=(gpio_output_type)GPIO_OUTPUT;//输出类型
    gpio_init_struct.gpio_pull=(gpio_pull_type)GPIO_PULL;//电平控制
    gpio_init_struct.gpio_drive_strength=(gpio_drive_type)GPIO_DRIVE_STRENGTH_STRONGER;//输出强度


    if(GPIO=='A') gpio_init(GPIOA, &gpio_init_struct);
    if(GPIO=='B') gpio_init(GPIOB, &gpio_init_struct);
    if(GPIO=='C') gpio_init(GPIOC, &gpio_init_struct);
    if(GPIO=='D') gpio_init(GPIOD, &gpio_init_struct);
    if(GPIO=='E') gpio_init(GPIOE, &gpio_init_struct);
}
/*必须先开启IO口时钟，再对其进行复用*/


//////================================================================外部中断================================================================\\

//GPIO_PIN：选择GPIO口（"P<大写字母><数字>"）
//EXINT_LINE：外部中断触发模式
//EXINT_TRIGGER：触发边沿
void Exti_Init(char*GPIO_PIN,u8 EXINT_LINE,u8 EXINT_TRIGGER)//外部中断初始化
{
    u8 GPIO,PIN;
    u8 EXTI_PortSourceGPIOX,EXTI_PinSourceX;
    uint32_t EXTI_LineX;
    exint_init_type exint_init_struct;
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);//开启复用mux时钟
    if(*GPIO_PIN=='P'&&(strlen(GPIO_PIN)==3||strlen(GPIO_PIN)==4))
    {
        GPIO=*(GPIO_PIN+1);
        if(strlen(GPIO_PIN)==3)PIN=*(GPIO_PIN+2)-48;
        if(strlen(GPIO_PIN)==4)PIN=(*(GPIO_PIN+2)-48)*10+*(GPIO_PIN+3)-48;
    }
    else return;
    
    switch(EXINT_TRIGGER)//初始化相应IO口
    {
        case EXINT_TRIGGER_FALLING_EDGE:Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);break;
        case EXINT_TRIGGER_RISING_EDGE:Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_DOWN,GPIO_DRIVE_STRENGTH_STRONGER);break;
        case EXINT_TRIGGER_BOTH_EDGE:Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);break;
    }
    
    switch(GPIO)
    {
        case 'A':EXTI_PortSourceGPIOX=GPIO_PORT_SOURCE_GPIOA;break;
        case 'B':EXTI_PortSourceGPIOX=GPIO_PORT_SOURCE_GPIOB;break;
        case 'C':EXTI_PortSourceGPIOX=GPIO_PORT_SOURCE_GPIOC;break;
        case 'D':EXTI_PortSourceGPIOX=GPIO_PORT_SOURCE_GPIOD;break;
        case 'E':EXTI_PortSourceGPIOX=GPIO_PORT_SOURCE_GPIOE;break;
    }
    switch(PIN)
    {
        case 0: EXTI_PinSourceX=GPIO_PINS_SOURCE0; EXTI_LineX=EXINT_LINE_0; break;
        case 1: EXTI_PinSourceX=GPIO_PINS_SOURCE1; EXTI_LineX=EXINT_LINE_1; break;
        case 2: EXTI_PinSourceX=GPIO_PINS_SOURCE2; EXTI_LineX=EXINT_LINE_2; break;
        case 3: EXTI_PinSourceX=GPIO_PINS_SOURCE3; EXTI_LineX=EXINT_LINE_3; break;
        case 4: EXTI_PinSourceX=GPIO_PINS_SOURCE4; EXTI_LineX=EXINT_LINE_4; break;
        case 5: EXTI_PinSourceX=GPIO_PINS_SOURCE5; EXTI_LineX=EXINT_LINE_5; break;
        case 6: EXTI_PinSourceX=GPIO_PINS_SOURCE6; EXTI_LineX=EXINT_LINE_6; break;
        case 7: EXTI_PinSourceX=GPIO_PINS_SOURCE7; EXTI_LineX=EXINT_LINE_7; break;
        case 8: EXTI_PinSourceX=GPIO_PINS_SOURCE8; EXTI_LineX=EXINT_LINE_8; break;
        case 9: EXTI_PinSourceX=GPIO_PINS_SOURCE9; EXTI_LineX=EXINT_LINE_9; break;
        case 10:EXTI_PinSourceX=GPIO_PINS_SOURCE10;EXTI_LineX=EXINT_LINE_10;break;
        case 11:EXTI_PinSourceX=GPIO_PINS_SOURCE11;EXTI_LineX=EXINT_LINE_11;break;
        case 12:EXTI_PinSourceX=GPIO_PINS_SOURCE12;EXTI_LineX=EXINT_LINE_12;break;
        case 13:EXTI_PinSourceX=GPIO_PINS_SOURCE13;EXTI_LineX=EXINT_LINE_13;break;
        case 14:EXTI_PinSourceX=GPIO_PINS_SOURCE14;EXTI_LineX=EXINT_LINE_14;break;
        case 15:EXTI_PinSourceX=GPIO_PINS_SOURCE15;EXTI_LineX=EXINT_LINE_15;break;
    }
    gpio_exint_line_config((gpio_port_source_type)EXTI_PortSourceGPIOX, (gpio_pins_source_type)EXTI_PinSourceX);//设置PA为外部中新源
    
    exint_init_struct.line_select = EXTI_LineX;//中断线
    exint_init_struct.line_mode = (exint_line_mode_type)EXINT_LINE;//外部中断触发模式
    exint_init_struct.line_polarity = (exint_polarity_config_type)EXINT_TRIGGER;//触发边沿
    exint_init_struct.line_enable = TRUE;//使能中断
    exint_init(&exint_init_struct);
}
/*
void EXINTx_IRQHandler()
{
    ...
    
    exint_flag_clear(EXINT_LINE_x);
}
*/



////================================================================中断设置================================================================\\


//设置中断
void Nvic_Init(int IRQn,int x,int y)
{
    nvic_irq_enable((IRQn_Type)IRQn, x, y);
}


////================================================================独立看门狗================================================================\\


void IWDG_Init(int reload_value,int WDT_CLK_DIV)//看门狗初始化
{
    wdt_register_write_enable(TRUE);
    wdt_divider_set((wdt_division_type)WDT_CLK_DIV);//设置预分频值
    wdt_reload_value_set(reload_value);//设置重装载值
    wdt_counter_reload();//重装载WDT寄存器
    wdt_enable();//使能看门狗
}

void IWDG_Feed()//喂狗
{
    wdt_counter_reload();
}


////================================================================定时器================================================================\\


//定时器初始化
void Tim_Init(int tim,uint32_t arr,uint32_t psc,int mode)
{
    tmr_type * TMRx;
    
    //使能TIM时钟，获取定时器号
    if(tim==1)  {crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK , TRUE);TMRx=TMR1 ;}
    if(tim==2)  {crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK , TRUE);TMRx=TMR2 ;}
    if(tim==3)  {crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK , TRUE);TMRx=TMR3 ;}
    if(tim==4)  {crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK , TRUE);TMRx=TMR4 ;}
    if(tim==5)  {crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK , TRUE);TMRx=TMR5 ;}
    if(tim==6)  {crm_periph_clock_enable(CRM_TMR6_PERIPH_CLOCK , TRUE);TMRx=TMR6 ;}
    if(tim==7)  {crm_periph_clock_enable(CRM_TMR7_PERIPH_CLOCK , TRUE);TMRx=TMR7 ;}
    if(tim==8)  {crm_periph_clock_enable(CRM_TMR8_PERIPH_CLOCK , TRUE);TMRx=TMR8 ;}
    if(tim==9)  {crm_periph_clock_enable(CRM_TMR9_PERIPH_CLOCK , TRUE);TMRx=TMR9 ;}
    if(tim==10) {crm_periph_clock_enable(CRM_TMR10_PERIPH_CLOCK, TRUE);TMRx=TMR10;}
    if(tim==11) {crm_periph_clock_enable(CRM_TMR11_PERIPH_CLOCK, TRUE);TMRx=TMR11;}
    if(tim==12) {crm_periph_clock_enable(CRM_TMR12_PERIPH_CLOCK, TRUE);TMRx=TMR12;}
    if(tim==13) {crm_periph_clock_enable(CRM_TMR13_PERIPH_CLOCK, TRUE);TMRx=TMR13;}
    if(tim==14) {crm_periph_clock_enable(CRM_TMR14_PERIPH_CLOCK, TRUE);TMRx=TMR14;}
    
    tmr_base_init(TMRx,arr,psc);//设置 重装载值 和 预分频值
    
    if(mode==Up)   tmr_cnt_dir_set(TMRx, TMR_COUNT_UP);//TIM向上计数模式
    if(mode==Down) tmr_cnt_dir_set(TMRx, TMR_COUNT_DOWN);//TIM向下计数模式
    if(mode==CA1)  tmr_cnt_dir_set(TMRx, TMR_COUNT_TWO_WAY_1);//TIM中央对齐计数模式1
    if(mode==CA2)  tmr_cnt_dir_set(TMRx, TMR_COUNT_TWO_WAY_2);//TIM中央对齐计数模式2
    if(mode==CA3)  tmr_cnt_dir_set(TMRx, TMR_COUNT_TWO_WAY_3);//TIM中央对齐计数模式3
    
    tmr_interrupt_enable(TMRx, TMR_OVF_INT, TRUE);//使能中断,允许更新中断
    
    tmr_counter_enable(TMRx, TRUE);//使能TIM外设
    
}
//定时器中断例子

//void TMR3_GLOBAL_IRQHandler(void)
//{
//    if(tmr_flag_get(TMR3, TMR_OVF_FLAG) != RESET)
//    {
//    
//        tmr_flag_clear(TMR3, TMR_OVF_FLAG);//清除标志位
//    }
//}

/*
void TMRx_GLOBAL_IRQHandler(void)
{
    if(tmr_flag_get(TMRx, TMR_OVF_FLAG) != RESET)
    {
        ...
        
        tmr_flag_clear(TMRx, TMR_OVF_FLAG);
    }
}
*/
/*?不知什么原因在该中断服务函数里USARTx->sts无法清0?*/

///************************************************************************
//TIM复用引脚

//TIM1_CH1        PA8     PE9
//TIM1_CH2        PA9     PE11
//TIM1_CH3        PA10    PE13
//TIM1_CH4        PA11    PE14
//TIM1_CH1N       PA7     PB13    PE8
//TIM1_CH2N       PB0     PB14    PE10
//TIM1_CH3N       PB1     PB15    PE12

//TIM2_CH1_ETR    PA5     PA15
//TIM2_CH2        PA1     PB3
//TIM2_CH3        PA2     PB10
//TIM2_CH4        PA3     PB11

//TIM3_CH1        PA6     PB4     PC6
//TIM3_CH2        PA7     PB5     PC7
//TIM3_CH3        PB0     PC8
//TIM3_CH4        PB1     PC9

//TIM4_CH1        PB6     PD12
//TIM4_CH2        PB7     PD13
//TIM4_CH3        PB8     PD14
//TIM4_CH4        PB9     PD15

//TIM5_CH1        PA0     PH10
//TIM5_CH2        PA1     PH11
//TIM5_CH3        PA2     PH12
//TIM5_CH4        PA3     PH13

//TIM8_CH1        PA5     PC6     PI5
//TIM8_CH2        PC7     PI6
//TIM8_CH3        PC8     PI7
//TIM8_CH4        PC9     PI2
//TIM8_CH1N       PA7     PH13
//TIM8_CH2N       PB0     PB14    PH14
//TIM8_CH3N       PB1     PB15    PH15

//TIM9_CH1        PA2     PE5
//TIM9_CH2        PA3     PE6

//TIM10_CH1       PB8     PF6

//TIM11_CH1       PB9     PF7

//TIM12_CH1       PB14    PH6
//TIM12_CH2       PB15    PH9

//TIM13_CH1       PA6     PF8

//TIM14_CH1       PA7     PF9


//*************************************************************************/



////================================================================PWM================================================================\\


//设置PWM模式
void Pwm_Init(u8 GPIO,u8 PIN,int mode,int pol,tmr_type* TIMx,int oc)
{
    tmr_output_config_type tmr_oc_init_structure;
    
    Gpio_Init(GPIO,PIN,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);//初始化对应IO口
    
    
    if(mode==1) tmr_oc_init_structure.oc_mode= TMR_OUTPUT_CONTROL_PWM_MODE_A;//PWM模式A
    if(mode==2) tmr_oc_init_structure.oc_mode= TMR_OUTPUT_CONTROL_PWM_MODE_B;//PWM模式B
    
    tmr_oc_init_structure.oc_idle_state = FALSE;
    
    if(pol==High) tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;//TIM输出比较极性高
    if(pol==Low)  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;//TIM输出比较极性低
    
    tmr_oc_init_structure.oc_output_state = TRUE;//使能输出
    
    if(oc==1) tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);//设置TMRx的通道1
    if(oc==2) tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_2, &tmr_oc_init_structure);//设置TMRx的通道2
    if(oc==3) tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_3, &tmr_oc_init_structure);//设置TMRx的通道3
    if(oc==4) tmr_output_channel_config(TIMx, TMR_SELECT_CHANNEL_4, &tmr_oc_init_structure);//设置TMRx的通道4
    
    if(oc==1) tmr_output_channel_buffer_enable(TIMx, TMR_SELECT_CHANNEL_1, TRUE);//设置TMRx的通道1的数据寄存器的值，溢出事件时更新
    if(oc==2) tmr_output_channel_buffer_enable(TIMx, TMR_SELECT_CHANNEL_2, TRUE);//设置TMRx的通道2的数据寄存器的值，溢出事件时更新
    if(oc==3) tmr_output_channel_buffer_enable(TIMx, TMR_SELECT_CHANNEL_3, TRUE);//设置TMRx的通道3的数据寄存器的值，溢出事件时更新
    if(oc==4) tmr_output_channel_buffer_enable(TIMx, TMR_SELECT_CHANNEL_4, TRUE);//设置TMRx的通道4的数据寄存器的值，溢出事件时更新
    
    tmr_period_buffer_enable(TIMx, TRUE);//TMRx 的重载值，溢出事件时更新
    
    tmr_output_enable(TIMx, TRUE);//使能输出，高级定时器特有。
}

/*tmr_channel_value_set(TMRx,TMR_SELECT_CHANNEL_x,pwm)*/

////================================================================DMA================================================================\\


//设置DMA（DMA数据流，通道，外设地址，外设传输宽度，内存地址，内存传输宽度，数据长度，传输方向，DMA模式，优先级）
void Dma_Init(dma_type *DMAx,dma_channel_type * DMAx_CHANNELx,u32 cpar,u32 P_size,u32 cmar,u32 M_size,u32 len,u32 DIR,u32 loop_mode,u32 priority,u8 DMA_FLEXIBLE)
{
    dma_init_type dma_init_struct;
    uint8_t FLEX_CHANNELx;
    
    if(DMAx_CHANNELx==DMA1_CHANNEL1 || DMAx_CHANNELx==DMA2_CHANNEL1)FLEX_CHANNELx=FLEX_CHANNEL1;
    if(DMAx_CHANNELx==DMA1_CHANNEL2 || DMAx_CHANNELx==DMA2_CHANNEL2)FLEX_CHANNELx=FLEX_CHANNEL2;
    if(DMAx_CHANNELx==DMA1_CHANNEL3 || DMAx_CHANNELx==DMA2_CHANNEL3)FLEX_CHANNELx=FLEX_CHANNEL3;
    if(DMAx_CHANNELx==DMA1_CHANNEL4 || DMAx_CHANNELx==DMA2_CHANNEL4)FLEX_CHANNELx=FLEX_CHANNEL4;
    if(DMAx_CHANNELx==DMA1_CHANNEL5 || DMAx_CHANNELx==DMA2_CHANNEL5)FLEX_CHANNELx=FLEX_CHANNEL5;
    if(DMAx_CHANNELx==DMA1_CHANNEL6 || DMAx_CHANNELx==DMA2_CHANNEL6)FLEX_CHANNELx=FLEX_CHANNEL6;
    if(DMAx_CHANNELx==DMA1_CHANNEL7 || DMAx_CHANNELx==DMA2_CHANNEL7)FLEX_CHANNELx=FLEX_CHANNEL7;
    
    if((u32)DMAx>=(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
    {
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);//DMA2时钟使能 
    }
    else 
    {
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);//DMA1时钟使能 
    }
    
    dma_reset(DMAx_CHANNELx);
    
    dma_init_struct.memory_base_addr=cmar;//设置内存地址
    dma_init_struct.memory_data_width=(dma_memory_data_size_type)M_size;//设置内存传输宽度
    
    dma_init_struct.peripheral_base_addr=cpar;//设置外设地址
    dma_init_struct.peripheral_data_width=(dma_peripheral_data_size_type)P_size;//设置外设传输宽度
    
    dma_init_struct.buffer_size=len;//设置数据长度
    
    dma_init_struct.direction=(dma_dir_type)DIR;//设置传输方向
    
    dma_init_struct.peripheral_inc_enable = FALSE;//外设地址不变
    dma_init_struct.memory_inc_enable = TRUE;//内存地址增加
    
    dma_init_struct.loop_mode_enable=(confirm_state)loop_mode;//是否开启循环模式
    dma_init_struct.priority=(dma_priority_level_type)priority;//设置优先级
    
    dma_init(DMAx_CHANNELx, &dma_init_struct);
    
    dma_flexible_config(DMAx,FLEX_CHANNELx,(dma_flexible_request_type)DMA_FLEXIBLE);
    
    dma_channel_enable(DMAx_CHANNELx, TRUE);
}

void DMA_NO(dma_channel_type * DMAx_CHANNELx,u32 Len)//开启DMA
{ 
    dma_channel_enable(DMAx_CHANNELx, FALSE);  //关闭DMA所指示的通道
    dma_flag_clear((uint32_t)DMAx_CHANNELx);
    dma_data_number_set(DMAx_CHANNELx, Len);//DMA通道的DMA缓存的大小
    dma_channel_enable(DMAx_CHANNELx, TRUE);//使能DMA所指示的通道 
}



//////================================================================定时器捕获================================================================\\


//定时器捕获设置
void Cap_Init(u8 GPIO,u8 PIN,tmr_type*TMRx,u8 Channel,u8 TMR_INPUT,u8 ICFilter)
{
    tmr_input_config_type  tmr_input_config_struct;
    
    Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);//初始化对应IO口
    
    //设置映射通道
    if(Channel==1) tmr_input_config_struct.input_channel_select = TMR_SELECT_CHANNEL_1;
    if(Channel==2) tmr_input_config_struct.input_channel_select = TMR_SELECT_CHANNEL_2;
    if(Channel==3) tmr_input_config_struct.input_channel_select = TMR_SELECT_CHANNEL_3;
    if(Channel==4) tmr_input_config_struct.input_channel_select = TMR_SELECT_CHANNEL_4;
    
    tmr_input_config_struct.input_polarity_select = (tmr_input_polarity_type)TMR_INPUT;//设置捕获电平
    tmr_input_config_struct.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;//通道被配置为输入，直接映射
    tmr_input_config_struct.input_filter_value=ICFilter;//设置滤波
    tmr_input_channel_init(TMRx, &tmr_input_config_struct, TMR_CHANNEL_INPUT_DIV_1);
    
    //开启中断
    if(Channel==1) tmr_interrupt_enable(TMRx, TMR_C1_INT, TRUE);
    if(Channel==2) tmr_interrupt_enable(TMRx, TMR_C2_INT, TRUE);
    if(Channel==3) tmr_interrupt_enable(TMRx, TMR_C3_INT, TRUE);
    if(Channel==4) tmr_interrupt_enable(TMRx, TMR_C4_INT, TRUE);
}

/*
void TMRx_GLOBAL_IRQHandler(void)
{
    if(tmr_flag_get(TMRx, TMR_Cx_FLAG) != RESET)
    {
        ...
        
        tmr_flag_clear(TMRx, TMR_Cx_FLAG);
    }
}
*/
//注意：不开启的通道默认是触发的


//////================================================================SPI================================================================\\


void Spi_Init(spi_type*SPIx,u16 direction,u16 mode,u16 datasize,u16 cpol,u16 cpha,u16 baudrateprescaler,u16 firstbit)//初始化SPI
{
    spi_init_type spi_init_struct;
    
    if(SPIx==SPI1)
    {
        crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK , TRUE);//SPI1时钟使能
        
        crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);//开启复用时钟
        gpio_pin_remap_config(SWJTAG_MUX_010,TRUE);//关闭掉jtag,保留swd
        gpio_pin_remap_config(SPI1_MUX_01,TRUE);//spi io复用到pa15,pb3，pb4,pb5
        
        GPIO_init('B',3,AF_PP);//SCL
        GPIO_init('B',4,AF_PP);//MIS0
        GPIO_init('B',5,AF_PP);//MOSI
        
    }
    if(SPIx==SPI3)
    {
        crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK , TRUE);//SPI3时钟使能
        
        crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);//开启复用时钟
        gpio_pin_remap_config(SWJTAG_MUX_010,TRUE);//关闭掉jtag,保留swd
        gpio_pin_remap_config(SPI3_GMUX_0001,TRUE);//spi io复用到pa4,pc10，pc11,pc12
        
        GPIO_init('C',10,AF_PP);//SCL
        GPIO_init('C',11,AF_PP);//MIS0
        GPIO_init('C',12,AF_PP);//MOSI
        
    }
    if(SPIx==SPI4)
    {
        crm_periph_clock_enable(CRM_SPI4_PERIPH_CLOCK , TRUE);//SPI3时钟使能
        
        crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);//开启复用时钟
        gpio_pin_remap_config(SWJTAG_MUX_010,TRUE);//关闭掉jtag,保留swd
        gpio_pin_remap_config(SPI4_GMUX_0011,TRUE);//spi io复用到pa6,pb7，pb8,pb9
        
        GPIO_init('B',7,AF_PP);//SCL
        GPIO_init('B',8,AF_PP);//MIS0
        GPIO_init('B',9,AF_PP);//MOSI
    }
    spi_init_struct.transmission_mode=(spi_transmission_mode_type)direction;//数据传输模式
    spi_init_struct.master_slave_mode=(spi_master_slave_mode_type)mode;//SPI模式
    spi_init_struct.frame_bit_num=(spi_frame_bit_num_type)datasize;//SPI数据大小
    spi_init_struct.clock_polarity=(spi_clock_polarity_type)cpol;//同步时钟空闲电平
    spi_init_struct.clock_phase=(spi_clock_phase_type)cpha;//数据采样时刻
    spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;//NSS信号软件控制
    spi_init_struct.mclk_freq_division=(spi_mclk_freq_div_type)baudrateprescaler;//波特率预分频
    spi_init_struct.first_bit_transmission=(spi_first_bit_type)firstbit;//数据开始传输位
    spi_init(SPIx, &spi_init_struct);
    
//  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;		//全双工模式
//  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;				//主模式
//  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;				//时钟8分频，120M/8=15M
//  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;			//大小端，高位在前
//  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;						//数据为8bit模式
//  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;			//配置 SPI 时钟空闲时为高电平
//  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;				//配置 SPI 在第二个时钟边沿采样
//  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;			//软件cs模式
    
    spi_enable(SPIx, TRUE);//使能SPI
}

u8 Spi_RW(spi_type*SPIx,u8 data)//SPI传输数据
{
    uint8_t brxbuff;
    spi_i2s_data_transmit(SPIx,data);
    while(spi_i2s_flag_get(SPIx, SPI_I2S_RDBF_FLAG) == RESET);
    brxbuff = spi_i2s_data_receive(SPIx);
    while(spi_i2s_flag_get(SPIx, SPI_I2S_BF_FLAG) != RESET);
    return brxbuff;
}




////================================================================ADC================================================================\\


//ADC初始化
//GPIO，PIN：                        设置IO口
//ADCx：                             选择ADC号
void Adc_Init(char GPIO,u8 PIN,adc_type *ADCx)
{
    adc_base_config_type adc_base_struct;
    
    Gpio_Init(GPIO,PIN,GPIO_MODE_ANALOG,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,100);//初始化对应IO口
    
    if(ADCx==ADC1)crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);//使能ADC1时钟
    if(ADCx==ADC2)crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);//使能ADC2时钟
    if(ADCx==ADC3)crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);//使能ADC3时钟
    crm_adc_clock_div_set(CRM_ADC_DIV_6);
    
    adc_combine_mode_select(ADC_INDEPENDENT_MODE);//ADC独立模式
    adc_base_struct.sequence_mode = FALSE;
    adc_base_struct.repeat_mode = FALSE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = 1;
    adc_base_config(ADCx, &adc_base_struct);
    
    adc_ordinary_conversion_trigger_set(ADCx, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
    
    adc_enable(ADCx, TRUE);
    
    adc_calibration_init(ADCx);
    while(adc_calibration_init_status_get(ADCx));
    adc_calibration_start(ADCx);
    while(adc_calibration_status_get(ADCx));
}

//得到ADC数据
//channel：ADC通道
u16 Adc_cap(adc_type *ADCx,adc_channel_select_type channel)
{
    adc_ordinary_channel_set(ADCx, channel, 1, ADC_SAMPLETIME_239_5);
    adc_ordinary_software_trigger_enable(ADCx, TRUE);
    while(adc_ordinary_software_trigger_status_get(ADCx));
    return adc_ordinary_conversion_data_get(ADCx);//返回最近一次ADC1规则组的转换结果
}


////================================================================串口================================================================\\

//串口初始化
//USARTx：串口号（0~6）
//bound：波特率
void Uartx_Init(u8 USARTx,u32 bound)
{
    if(USARTx==1)uart_init(bound);//初始化串口1
    if(USARTx==usart2)
    {
        crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE); //使能USART2
        
        Gpio_Init('A',2,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('A',3,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        
        Nvic_Init(USART2_IRQn,3,3);
        
        //USART 初始化设置
        usart_init(USART2, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
        
        //开启中断
        usart_transmitter_enable(USART2, TRUE);
        usart_receiver_enable(USART2, TRUE);
        
        //开启相应中断
        usart_interrupt_enable(USART2, USART_IDLE_INT, TRUE);//空闲中断
        usart_interrupt_enable(USART2, USART_RDBF_INT, TRUE);//接收中断
      
        usart_enable(USART2, TRUE);                    //使能串口2
    }
    if(USARTx==usart3)
    {
        crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE); //使能USART3
        
        Gpio_Init('B',10,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('B',11,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        
        Nvic_Init(USART3_IRQn,3,3);
        
        //USART 初始化设置
        usart_init(USART3, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
        
        //开启中断
        usart_transmitter_enable(USART3, TRUE);
        usart_receiver_enable(USART3, TRUE);
        
        //开启相应中断
        usart_interrupt_enable(USART3, USART_IDLE_INT, TRUE);//空闲中断
        usart_interrupt_enable(USART3, USART_RDBF_INT, TRUE);//接收中断
      
        usart_enable(USART3, TRUE);                    //使能串口3
    }
    if(USARTx==usart4)
    {
        crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE); //使能USART4
        
        Gpio_Init('C',10,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('C',11,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        
        Nvic_Init(UART4_IRQn,3,3);
        
        //USART 初始化设置
        usart_init(UART4, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
        
        //开启中断
        usart_transmitter_enable(UART4, TRUE);
        usart_receiver_enable(UART4, TRUE);
        
        //开启相应中断
        usart_interrupt_enable(UART4, USART_IDLE_INT, TRUE);//空闲中断
        usart_interrupt_enable(UART4, USART_RDBF_INT, TRUE);//接收中断
      
        usart_enable(UART4, TRUE);                    //使能串口4
    }
    if(USARTx==usart5)
    {
        crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE); //使能USART5
        
        Gpio_Init('C',12,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('D',2,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        
        Nvic_Init(UART5_IRQn,3,3);
        
        //USART 初始化设置
        usart_init(UART5, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
        
        //开启中断
        usart_transmitter_enable(UART5, TRUE);
        usart_receiver_enable(UART5, TRUE);
        
        //开启相应中断
        usart_interrupt_enable(UART5, USART_IDLE_INT, TRUE);//空闲中断
        usart_interrupt_enable(UART5, USART_RDBF_INT, TRUE);//接收中断
      
        usart_enable(UART5, TRUE);                    //使能串口4
    }
    if(USARTx==usart6)
    {
        crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, TRUE); //使能USART6
        
        Gpio_Init('C',6,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('C',7,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        
        Nvic_Init(USART6_IRQn,3,3);
        
        //USART 初始化设置
        usart_init(USART6, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
        
        //开启中断
        usart_transmitter_enable(USART6, TRUE);
        usart_receiver_enable(USART6, TRUE);
        
        //开启相应中断
        usart_interrupt_enable(USART6, USART_IDLE_INT, TRUE);//空闲中断
        usart_interrupt_enable(USART6, USART_RDBF_INT, TRUE);//接收中断
      
        usart_enable(USART6, TRUE);                    //使能串口6
    }
    if(USARTx==usart7)
    {
        crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, TRUE); //使能USART7
        
        Gpio_Init('E',8,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('E',7,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        
        Nvic_Init(UART7_IRQn,3,3);
        
        //USART 初始化设置
        usart_init(UART7, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
        
        //开启中断
        usart_transmitter_enable(UART7, TRUE);
        usart_receiver_enable(UART7, TRUE);
        
        //开启相应中断
        usart_interrupt_enable(UART7, USART_IDLE_INT, TRUE);//空闲中断
        usart_interrupt_enable(UART7, USART_RDBF_INT, TRUE);//接收中断
      
        usart_enable(UART7, TRUE);                    //使能串口7
    }
}

//串口发送数据
//USARTx：串口号
//BUF：发送数据缓存
//Len：发送数据长度（大于0发送该长度数据；小于等于零发送strlen统计的长度的数据）
void USARTx_Tx(u8 USARTx,char*BUF,u16 Len)
{
    u8 t;
    u16 len;
    if(Len>0)len=Len;
    else len=strlen((char*)BUF);
    for(t=0;t<len;t++)
    {
        if(USARTx==usart1)
        {
            usart_data_transmit(USART1, *(BUF+t));//向串口1发送数据
            while((USART1->sts&0X40)==0);//等待发送结束
        }
        if(USARTx==usart2)
        {
            usart_data_transmit(USART2, *(BUF+t));//向串口2发送数据
            while((USART2->sts&0X40)==0);//等待发送结束
        }
        if(USARTx==usart3)
        {
            usart_data_transmit(USART3, *(BUF+t));//向串口3发送数据
            while((USART3->sts&0X40)==0);//等待发送结束
        }
        if(USARTx==usart4)
        {
            usart_data_transmit(UART4, *(BUF+t));//向串口4发送数据
            while((UART4->sts&0X40)==0);//等待发送结束
        }
        if(USARTx==usart5)
        {
            usart_data_transmit(UART5, *(BUF+t));//向串口5发送数据
            while((UART5->sts&0X40)==0);//等待发送结束
        }
        if(USARTx==usart6)
        {
            usart_data_transmit(USART6, *(BUF+t));//向串口6发送数据
            while((USART6->sts&0X40)==0);//等待发送结束
        }
        if(USARTx==usart7)
        {
            usart_data_transmit(UART7, *(BUF+t));//向串口7发送数据
            while((UART7->sts&0X40)==0);//等待发送结束
        }
    }
}


extern u8 DMA_RX1;//串口1空闲标志
u8 RX2=0;//串口2空闲标志
u8 RX3=0;//串口3空闲标志
u8 RX4=0;//串口4空闲标志
u8 RX5=0;//串口5空闲标志
u8 RX6=0;//串口6空闲标志
u8 RX7=0;//串口6空闲标志
u8 USART2_BUF[129];//串口2接收数据缓存
u8 USART3_BUF[129];//串口3接收数据缓存
u8 USART4_BUF[129];//串口4接收数据缓存
u8 USART5_BUF[129];//串口5接收数据缓存
u8 USART6_BUF[129];//串口6接收数据缓存
u8 USART7_BUF[129];//串口6接收数据缓存

//串口接收函数
//USARTx：串口号
//BUF：接收数据缓存
//Len：接收数据长度（大于0从缓冲区复制该长度数据；小于等于零通过strcpy从缓冲区复制数据）
u8 USARTx_Rx(u8 USARTx,char*BUF,u16 Len)
{
    u16 i;
    if(DMA_RX1&&USARTx==usart1)
    {
        if(Len>0)for(i=0;i< Len;i++)*(BUF+i)=*(USART_RX_BUF+i);
        else strcpy((char*)BUF,(char*)USART_RX_BUF);
        DMA_RX1=0;//数据复制完成，标记为未接收
        return 1;
    }
    if(RX2&&USARTx==usart2)
    {
        if(Len>0)for(i=0;i<Len;i++)*(BUF+i)=*(USART2_BUF+i);
        else strcpy((char*)BUF,(char*)USART2_BUF);
        RX2=0;//数据复制完成，标记为未接收
        return 1;
    }
    if(RX3&&USARTx==usart3)
    {
        if(Len>0)for(i=0;i<Len;i++)*(BUF+i)=*(USART3_BUF+i);
        else strcpy((char*)BUF,(char*)USART3_BUF);
        RX3=0;//数据复制完成，标记为未接收
        return 1;
    }
    if(RX4&&USARTx==usart4)
    {
        if(Len>0)for(i=0;i<Len;i++)*(BUF+i)=*(USART4_BUF+i);
        else strcpy((char*)BUF,(char*)USART4_BUF);
        RX4=0;//数据复制完成，标记为未接收
        return 1;
    }
    if(RX5&&USARTx==usart5)
    {
        if(Len>0)for(i=0;i<Len;i++)*(BUF+i)=*(USART5_BUF+i);
        else strcpy((char*)BUF,(char*)USART5_BUF);
        RX5=0;//数据复制完成，标记为未接收
        return 1;
    }
    if(RX6&&USARTx==usart6)
    {
        if(Len>0)for(i=0;i<Len;i++)*(BUF+i)=*(USART6_BUF+i);
        else strcpy((char*)BUF,(char*)USART6_BUF);
        RX6=0;//数据复制完成，标记为未接收
        return 1;
    }
    if(RX7&&USARTx==usart7)
    {
        if(Len>0)for(i=0;i<Len;i++)*(BUF+i)=*(USART7_BUF+i);
        else strcpy((char*)BUF,(char*)USART7_BUF);
        RX7=0;//数据复制完成，标记为未接收
        return 1;
    }
    return 0;//未接收到数据返回0
}


u8 Rx2_data_bit=0;
extern int dma_data_rx_p2;
extern int dma_data_rx_len2;
void (*USART2_IDLE_IRQ)(void)=NULL;
void (*USART2_RX_IRQ)(u8 data)=NULL;
void USART2_IRQHandler(void)//串口2中断服务函数
{
    u8 Res;
    if(usart_flag_get(USART2, USART_IDLEF_FLAG) != RESET)//空闲中断
    {
        RX2=1;//数据接收完成，标记为空闲
        Rx2_data_bit=0;
        if(dma_data_rx_p2>=0)
        {
            dma_data_rx_p2+=dma_data_rx_len2;
            if(dma_data_rx_p2>=DMA_BUF_SIZE)dma_data_rx_p2-=DMA_BUF_SIZE;
            dma_data_rx_len2=DMA_BUF_SIZE-dma_data_number_get(DMA2_CHANNEL2)-dma_data_rx_p2;
            if(dma_data_rx_len2<=0)dma_data_rx_len2+=DMA_BUF_SIZE;
        }
        if(USART2_IDLE_IRQ!=NULL)
        {
            USART2_IDLE_IRQ();
        }
        usart_flag_clear(USART2, USART_IDLEF_FLAG);
    }
    if(usart_flag_get(USART2, USART_RDBF_FLAG) != RESET)
    {
        Res =usart_data_receive(USART2);
        if(USART2_RX_IRQ!=NULL)
        {
            USART2_RX_IRQ(Res);
            usart_flag_clear(USART2, USART_RDBF_FLAG);
            return ;
        }
        USART2_BUF[Rx2_data_bit]=Res;
        Rx2_data_bit++;
        usart_flag_clear(USART2, USART_RDBF_FLAG);
    }
}

u8 Rx3_data_bit=0;
extern int dma_data_rx_p3;
extern int dma_data_rx_len3;
void (*USART3_IDLE_IRQ)(void)=NULL;
void (*USART3_RX_IRQ)(u8 data)=NULL;
void USART3_IRQHandler(void)//串口3中断服务函数
{
    u8 Res;
    if(usart_flag_get(USART3, USART_IDLEF_FLAG) != RESET)//空闲中断
    {
        RX3=1;//数据接收完成，标记为空闲
        Rx3_data_bit=0;
        if(dma_data_rx_p3>=0)
        {
            dma_data_rx_p3+=dma_data_rx_len3;
            if(dma_data_rx_p3>=DMA_BUF_SIZE)dma_data_rx_p3-=DMA_BUF_SIZE;
            dma_data_rx_len3=DMA_BUF_SIZE-dma_data_number_get(DMA2_CHANNEL3)-dma_data_rx_p3;
            if(dma_data_rx_len3<=0)dma_data_rx_len3+=DMA_BUF_SIZE;
        }
        if(USART3_IDLE_IRQ!=NULL)USART3_IDLE_IRQ();
        usart_flag_clear(USART3, USART_IDLEF_FLAG);
    }
    if(usart_flag_get(USART3, USART_RDBF_FLAG) != RESET)
    {
        Res =usart_data_receive(USART3);
        if(USART3_RX_IRQ!=NULL)
        {
            USART3_RX_IRQ(Res);
            usart_flag_clear(USART3, USART_RDBF_FLAG);
            return ;
        }
        USART3_BUF[Rx3_data_bit]=Res;
        Rx3_data_bit++;
        usart_flag_clear(USART3, USART_RDBF_FLAG);
    }
}

u8 Rx4_data_bit=0;
extern int dma_data_rx_p4;
extern int dma_data_rx_len4;
void (*USART4_IDLE_IRQ)(void)=NULL;
void (*USART4_RX_IRQ)(u8 data)=NULL;
void UART4_IRQHandler(void)//串口4中断服务函数
{
    u8 Res;
    if(usart_flag_get(UART4, USART_IDLEF_FLAG) != RESET)//空闲中断
    {
        RX4=1;//数据接收完成，标记为空闲
        Rx4_data_bit=0;
        if(dma_data_rx_p4>=0)
        {
            dma_data_rx_p4+=dma_data_rx_len4;
            if(dma_data_rx_p4>=DMA_BUF_SIZE)dma_data_rx_p4-=DMA_BUF_SIZE;
            dma_data_rx_len4=DMA_BUF_SIZE-dma_data_number_get(DMA2_CHANNEL4)-dma_data_rx_p4;
            if(dma_data_rx_len4<=0)dma_data_rx_len4+=DMA_BUF_SIZE;
        }
        if(USART4_IDLE_IRQ!=NULL)USART4_IDLE_IRQ();
        usart_flag_clear(UART4, USART_IDLEF_FLAG);
    }
    if(usart_flag_get(UART4, USART_RDBF_FLAG) != RESET)
    {
        Res =usart_data_receive(UART4);
        if(USART4_RX_IRQ!=NULL)
        {
            USART4_RX_IRQ(Res);
            usart_flag_clear(UART4, USART_RDBF_FLAG);
            return ;
        }
        USART4_BUF[Rx4_data_bit]=Res;
        Rx4_data_bit++;
        usart_flag_clear(UART4, USART_RDBF_FLAG);
    }
}

u8 Rx5_data_bit=0;
extern int dma_data_rx_p5;
extern int dma_data_rx_len5;
void (*USART5_IDLE_IRQ)(void)=NULL;
void (*USART5_RX_IRQ)(u8 data)=NULL;
void UART5_IRQHandler(void)//串口5中断服务函数
{
    u8 Res;
    if(usart_flag_get(UART5, USART_IDLEF_FLAG) != RESET)//空闲中断
    {
        RX5=1;//数据接收完成，标记为空闲
        Rx5_data_bit=0;
        if(dma_data_rx_p5>=0)
        {
            dma_data_rx_p5+=dma_data_rx_len5;
            if(dma_data_rx_p5>=DMA_BUF_SIZE)dma_data_rx_p5-=DMA_BUF_SIZE;
            dma_data_rx_len5=DMA_BUF_SIZE-dma_data_number_get(DMA2_CHANNEL5)-dma_data_rx_p5;
            if(dma_data_rx_len5<=0)dma_data_rx_len5+=DMA_BUF_SIZE;
        }
        if(USART5_IDLE_IRQ!=NULL)USART5_IDLE_IRQ();
        usart_flag_clear(UART5, USART_IDLEF_FLAG);
    }
    if(usart_flag_get(UART5, USART_RDBF_FLAG) != RESET)
    {
        Res =usart_data_receive(UART5);
        if(USART5_RX_IRQ!=NULL)
        {
            USART5_RX_IRQ(Res);
            usart_flag_clear(UART5, USART_RDBF_FLAG);
            return ;
        }
        USART5_BUF[Rx5_data_bit]=Res;
        Rx5_data_bit++;
        usart_flag_clear(UART5, USART_RDBF_FLAG);
    }
}

u8 Rx6_data_bit=0;
extern int dma_data_rx_p6;
extern int dma_data_rx_len6;
void (*USART6_IDLE_IRQ)(void)=NULL;
void (*USART6_RX_IRQ)(u8 data)=NULL;
void USART6_IRQHandler(void)//串口6中断服务函数
{
    u8 Res;
    if(usart_flag_get(USART6, USART_IDLEF_FLAG) != RESET)//空闲中断
    {
        RX6=1;//数据接收完成，标记为空闲
        Rx6_data_bit=0;
        if(dma_data_rx_p6>=0)
        {
            dma_data_rx_p6+=dma_data_rx_len6;
            if(dma_data_rx_p6>=DMA_BUF_SIZE)dma_data_rx_p6-=DMA_BUF_SIZE;
            dma_data_rx_len6=DMA_BUF_SIZE-dma_data_number_get(DMA2_CHANNEL6)-dma_data_rx_p6;
            if(dma_data_rx_len6<=0)dma_data_rx_len6+=DMA_BUF_SIZE;
        }
        if(USART6_IDLE_IRQ!=NULL)USART6_IDLE_IRQ();
        usart_flag_clear(USART6, USART_IDLEF_FLAG);
    }
    if(usart_flag_get(USART6, USART_RDBF_FLAG) != RESET)
    {
        Res =usart_data_receive(USART6);
        if(USART6_RX_IRQ!=NULL)
        {
            USART6_RX_IRQ(Res);
            usart_flag_clear(USART6, USART_RDBF_FLAG);
            return ;
        }
        USART6_BUF[Rx6_data_bit]=Res;
        Rx6_data_bit++;
        usart_flag_clear(USART6, USART_RDBF_FLAG);
    }
}

u8 Rx7_data_bit=0;
extern int dma_data_rx_p7;
extern int dma_data_rx_len7;
void (*USART7_IDLE_IRQ)(void)=NULL;
void (*USART7_RX_IRQ)(u8 data)=NULL;
void UART7_IRQHandler(void)          //串口6中断服务函数
{
    u8 Res;
    if(usart_flag_get(UART7, USART_IDLEF_FLAG) != RESET)//空闲中断
    {
        RX7=1;//数据接收完成，标记为空闲
        Rx7_data_bit=0;
        if(dma_data_rx_p7>=0)
        {
            dma_data_rx_p7+=dma_data_rx_len7;
            if(dma_data_rx_p7>=DMA_BUF_SIZE)dma_data_rx_p7-=DMA_BUF_SIZE;
            dma_data_rx_len7=DMA_BUF_SIZE-dma_data_number_get(DMA2_CHANNEL7)-dma_data_rx_p7;
            if(dma_data_rx_len7<=0)dma_data_rx_len7+=DMA_BUF_SIZE;
        }
        if(USART7_IDLE_IRQ!=NULL)USART7_IDLE_IRQ();
        usart_flag_clear(UART7, USART_IDLEF_FLAG);
    }
    if(usart_flag_get(UART7, USART_RDBF_FLAG) != RESET)
    {
        Res =usart_data_receive(UART7);
        if(USART7_RX_IRQ!=NULL)
        {
            USART7_RX_IRQ(Res);
            usart_flag_clear(UART7, USART_RDBF_FLAG);
            return ;
        }
        USART7_BUF[Rx7_data_bit]=Res;
        Rx7_data_bit++;
        usart_flag_clear(UART7, USART_RDBF_FLAG);
    }
}

////=======================================usart=======================================//



// 为每个串口定义发送函数
void USART1_SendByte(uint8_t Byte)
{
    usart_data_transmit(USART1, Byte);
    while (usart_flag_get(USART1, USART_TDBE_FLAG) == RESET); // 使用AT32的标志
}

void USART2_SendByte(uint8_t Byte)
{
    usart_data_transmit(USART2, Byte);
    while (usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
}

void USART3_SendByte(uint8_t Byte)
{
    usart_data_transmit(USART3, Byte);
    while (usart_flag_get(USART3, USART_TDBE_FLAG) == RESET);
}

void UART4_SendByte(uint8_t Byte)
{
    usart_data_transmit(UART4, Byte);
    while (usart_flag_get(UART4, USART_TDBE_FLAG) == RESET);
}

void UART5_SendByte(uint8_t Byte)
{
    usart_data_transmit(UART5, Byte);
    while (usart_flag_get(UART5, USART_TDBE_FLAG) == RESET);
}

void USART6_SendByte(uint8_t Byte)
{
    usart_data_transmit(USART6, Byte);
    while (usart_flag_get(USART6, USART_TDBE_FLAG) == RESET);
}

void UART7_SendByte(uint8_t Byte)
{
    usart_data_transmit(UART7, Byte);
    while (usart_flag_get(UART7, USART_TDBE_FLAG) == RESET);
}

// 获取对应串口的发送函数
UART_SendByte_Func Get_UART_SendFunc(u8 USARTx)
{
    switch(USARTx)
    {
        case 1: return USART1_SendByte;
        case 2: return USART2_SendByte;
        case 3: return USART3_SendByte;
        case 4: return UART4_SendByte;
        case 5: return UART5_SendByte;
        case 6: return USART6_SendByte;
        case 7: return UART7_SendByte;
        default: return USART1_SendByte;
    }
}

// 可重用的串口发送字符串函数
void Uart_SendString(u8 USARTx, char *String)
{
    UART_SendByte_Func SendFunc = Get_UART_SendFunc(USARTx);
    
    while (*String != '\0')
    {
        SendFunc((uint8_t)*String);
        String++;
    }
}

// 可重用的printf函数，支持多个串口和多种数据类型
void Uart_Printf(u8 USARTx, char *format, ...)
{
    char String[512];//串口发送的字节数
    va_list arg;
    
    va_start(arg, format);
    vsprintf(String, format, arg);  // 使用vsprintf
    va_end(arg);
    
    Uart_SendString(USARTx, String);
}

// 带自动换行的printf函数
void Uart_PrintfLn(u8 USARTx, char *format, ...)
{
    char String[256];
    va_list arg;
    
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    
    Uart_SendString(USARTx, String);
    Uart_SendString(USARTx, "\r\n");
}

// 发送十六进制数据
void Uart_SendHex(u8 USARTx, uint8_t *data, uint16_t len)
{
    UART_SendByte_Func SendFunc = Get_UART_SendFunc(USARTx);
    
    for (uint16_t i = 0; i < len; i++)
    {
        // 手动转换十六进制
        uint8_t high_nibble = (data[i] >> 4) & 0x0F;
        uint8_t low_nibble = data[i] & 0x0F;
        
        // 发送高4位
        SendFunc(high_nibble < 10 ? '0' + high_nibble : 'A' + high_nibble - 10);
        // 发送低4位
        SendFunc(low_nibble < 10 ? '0' + low_nibble : 'A' + low_nibble - 10);
        
        // 在字节之间添加空格（除了最后一个字节）
        if (i < len - 1)
        {
            SendFunc(' ');
        }
    }
}

// 发送带描述的十六进制数据
void Uart_PrintHex(u8 USARTx, char *description, uint8_t *data, uint16_t len)
{
    Uart_Printf(USARTx, "%s: ", description);
    Uart_SendHex(USARTx, data, len);
    Uart_SendString(USARTx, "\r\n");
}

// 发送数字（整数）
void Uart_SendNumber(u8 USARTx, int32_t number)
{
    char buffer[12]; // 足够存储-2147483648到2147483647
    sprintf(buffer, "%ld", number);
    Uart_SendString(USARTx, buffer);
}

// 发送浮点数
void Uart_SendFloat(u8 USARTx, float number, uint8_t decimal_places)
{
    char format[10];
    char buffer[20];
    
    sprintf(format, "%%.%df", decimal_places);
    sprintf(buffer, format, number);
    Uart_SendString(USARTx, buffer);
}


////================================================================CAN================================================================\\



//CAN初始化函数
//mode：CAN模式   1:回环；0:普通
//CAN_RSAW：同步段
//CAN_BTS1：时间段1
//CAN_BTS2：时间段2
//can_prescaler：预分频
//CAN_FilterNumber：过滤器号
//CAN_FILTER_BIT：过滤器位宽
//CAN_Id_Mode：ID格式
//CAN_FilterId：ID
//CAN_FilterMaskId：掩码
//CAN_FILTER_FIFO：过滤器关联邮箱
void Can_Init(u8 CANx,u8 mode,u8 CAN_RSAW,u8 CAN_BTS1,u8 CAN_BTS2,u8 can_prescaler,u8 CAN_FilterNumber,u8 CAN_FILTER_BIT,u8 CAN_Id_Mode,unsigned int CAN_FilterId,unsigned int CAN_FilterMaskId,u8 CAN_FILTER_FIFO)
{
    can_base_type        can_base_struct;
    can_baudrate_type    can_baudrate_struct;
    can_filter_init_type can_filter_init_struct;
    
    crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK, TRUE);//使能CAN1时钟
    crm_periph_clock_enable(CRM_CAN2_PERIPH_CLOCK, TRUE);//使能CAN2时钟
    
    if(CANx==1)
    {
        Gpio_Init('A',11,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('A',12 ,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
    }
    if(CANx==2)
    {
        Gpio_Init('B',12,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
        Gpio_Init('B',13,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);
    }
    
    delay_init();
    
    can_base_struct.mode_selection = (can_mode_type)mode;/* 模式设置： mode:0,普通模式;1,回环模式 */
    can_base_struct.ttc_enable = FALSE;/* CAN 时间触发模式（时间戳）：关闭 */
    can_base_struct.aebo_enable = TRUE;/* 自动退出离线功能：开启 */
    can_base_struct.aed_enable = TRUE;/* 自动退出睡眠功能：开启 */
    can_base_struct.prsf_enable = FALSE;/* 禁止自动重传：关闭（即开启自动重传—同 CAN 标准协议） */
    can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;		/* 报文溢出丢弃规则：丢弃之前收到的报文，保留最新收到的报文 */
    can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;/* 报文发送优先级：根据 ID（ID 小的先发送） */
    if(CANx==1)can_base_init(CAN1, &can_base_struct);
    if(CANx==2)can_base_init(CAN2, &can_base_struct);
    
    can_baudrate_struct.baudrate_div = can_prescaler;
    can_baudrate_struct.rsaw_size = (can_rsaw_type)CAN_RSAW;
    can_baudrate_struct.bts1_size = (can_bts1_type)CAN_BTS1;
    can_baudrate_struct.bts2_size = (can_bts2_type)CAN_BTS2;
    if(CANx==1)can_baudrate_set(CAN1, &can_baudrate_struct);          //波特率=120000/((swj+1)+(bs1+1)+(bs2+1))/prescaler  Kpbs
    if(CANx==2)can_baudrate_set(CAN2, &can_baudrate_struct);
    
    can_filter_init_struct.filter_number=CAN_FilterNumber;//过滤器X(0~13)
    can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK; //指定了过滤器将被初始化到的模式为标识符屏蔽位模式
    can_filter_init_struct.filter_bit = (can_filter_bit_width_type)CAN_FILTER_BIT;//过滤器位宽
    if(CAN_Id_Mode==CAN_Id_Mode_StdId)//数据ID（标准格式）
    {
        can_filter_init_struct.filter_id_high=CAN_FilterId<<5;
        can_filter_init_struct.filter_id_low=0x00;
    }
    if(CAN_Id_Mode==CAN_Id_Mode_ExtId)//数据ID（扩展格式）
    {
        can_filter_init_struct.filter_id_high=(CAN_FilterId>>16)&0xffff;
        can_filter_init_struct.filter_id_low=((CAN_FilterId<<3)&0xffff)|0x04;
    }
    can_filter_init_struct.filter_mask_high=(CAN_FilterMaskId>>16)&0xffff;//数据掩码
    can_filter_init_struct.filter_mask_low=(CAN_FilterMaskId&0xffff);
    can_filter_init_struct.filter_fifo = (can_filter_fifo_type)CAN_FILTER_FIFO;//过滤器关联邮箱
    can_filter_init_struct.filter_activate_enable = TRUE;//激活过滤器
    if(CANx==1)can_filter_init(CAN1, &can_filter_init_struct);
    if(CANx==2)can_filter_init(CAN2, &can_filter_init_struct);
    
    if(CAN_FILTER_FIFO==CAN_FILTER_FIFO0)
    {
        if(CANx==1)//CAN1
        {
            can_interrupt_enable(CAN1, CAN_RF0MIEN_INT, TRUE);//FIFO0消息挂号中断允许
            Nvic_Init(USBFS_L_CAN1_RX0_IRQn,1,1);//开启接收中断
        }
        if(CANx==2)//CAN2
        {
            can_interrupt_enable(CAN2, CAN_RF0MIEN_INT, TRUE);//FIFO0消息挂号中断允许
            Nvic_Init(CAN2_RX0_IRQn,1,1);//开启接收中断
        }
    }
    if(CAN_FILTER_FIFO==CAN_FILTER_FIFO1)
    {
        if(CANx==1)//CAN1
        {
            can_interrupt_enable(CAN1, CAN_RF1MIEN_INT, TRUE);//FIFO1消息挂号中断允许
            Nvic_Init(CAN1_RX1_IRQn,1,1);//开启接收中断
        }
        if(CANx==2)//CAN2
        {
            can_interrupt_enable(CAN2, CAN_RF1MIEN_INT, TRUE);//FIFO1消息挂号中断允许
            Nvic_Init(CAN2_RX1_IRQn,1,1);//开启接收中断
        }
    }
}



//CAN的IO接口重复用
//必须放在CAN1初始化前
//CANx：CAN号（1~2）
//GPIO；PIN：IO口
/***************

CAN1：
Rx:PA11 ； Tx:PA12
Rx:PB8  ； Tx:PB9 
Rx:PD0  ； Tx:PD1 
Rx:PI9  ； Tx:PH13

CAN2：
Rx:PB5  ； Tx:PB6 
Rx:PB12 ； Tx:PB13


***************/



//CAN发送数据
//IDE：ID格式
//ID：ID
//data：发送的数据
//len：数据长度（len=0时，通过strlen识别长度）
//end_sign： 1:发送结束标志符，0:不发送结束标志符
void Can_Tx(u8 CANx,uint32_t IDE,unsigned int ID,u8*data,u32 len,u8 end_sign)
{
    u8 mbox;
    u32 i,j,k=0,LEN;
    can_tx_message_type TxMessage;
    if(IDE==CAN_ID_STANDARD)TxMessage.standard_id=ID;//标准ID
    if(IDE==CAN_ID_EXTENDED)TxMessage.extended_id=ID;//扩展ID
    TxMessage.id_type=(can_identifier_type)IDE;//ID格式
    TxMessage.frame_type=CAN_TFT_DATA;//数据帧
    if(len==0)LEN=strlen((char*)data);//通过strlen识别长度数据长度
    else LEN=len;//手动输入长度
    
    if(CANx==1)
    {
        if(end_sign)//发送结束符
        {
            LEN+=3;//增加3个长度用来存放标识符
            for(i=0;i<=(LEN-1)/8;i++)//CAN一次最多发送8字节，超过8字节的数据通过多次发送
            {
                for(j=0;j<8;j++)//将8字节数据存入CAN的数据缓存区
                {
                    if(k<LEN-3)TxMessage.data[j]=data[i*8+j];//将要发送的数据存入CAN的数据缓存区
                    else if(k==LEN-3)TxMessage.data[j]='\r';//倒数第3位打上结束标识'\r'
                    else if(k==LEN-2)TxMessage.data[j]='\n';//倒数第2位打上结束标识'\n'
                    else if(k==LEN-1)TxMessage.data[j]=LEN-3;//倒数第1位写入数据长度，用于判断是否接收错误
                    k++;
                    if(k==LEN)break;//数据打包完成，跳出循环
                }
                TxMessage.dlc=k-i*8;//当前数据包大小
                mbox=can_message_transmit(CAN1, &TxMessage);//发送数据
                j=0;
                while((can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)mbox) != CAN_TX_STATUS_SUCCESSFUL)&&j<0xFFF)j++;//等待数据发送
            }
        }
        else//不发送结束符
        {
            for(i=0;i<=(LEN-1)/8;i++)//CAN一次最多发送8字节，超过8字节的数据通过多次发送
            {
                for(j=0;j<8;j++)//将8字节数据存入CAN的数据缓存区
                {
                    if(k<LEN)TxMessage.data[j]=data[i*8+j];//将要发送的数据存入CAN的数据缓存区
                    k++;
                    if(k==LEN)break;//数据打包完成，跳出循环
                }
                TxMessage.dlc=k-i*8;//当前数据包大小
                mbox=can_message_transmit(CAN1, &TxMessage);//发送数据
                j=0;
                while((can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)mbox) != CAN_TX_STATUS_SUCCESSFUL)&&j<0xFFF)j++;//等待数据发送
            }
        }
    }
    if(CANx==2)
    {
        if(end_sign)//发送结束符
        {
            LEN+=3;//增加3个长度用来存放标识符
            for(i=0;i<=(LEN-1)/8;i++)//CAN一次最多发送8字节，超过8字节的数据通过多次发送
            {
                for(j=0;j<8;j++)//将8字节数据存入CAN的数据缓存区
                {
                    if(k<LEN-3)TxMessage.data[j]=data[i*8+j];//将要发送的数据存入CAN的数据缓存区
                    else if(k==LEN-3)TxMessage.data[j]='\r';//倒数第3位打上结束标识'\r'
                    else if(k==LEN-2)TxMessage.data[j]='\n';//倒数第2位打上结束标识'\n'
                    else if(k==LEN-1)TxMessage.data[j]=LEN-3;//倒数第1位写入数据长度，用于判断是否接收错误
                    k++;
                    if(k==LEN)break;//数据打包完成，跳出循环
                }
                TxMessage.dlc=k-i*8;//当前数据包大小
                mbox=can_message_transmit(CAN2, &TxMessage);//发送数据
                j=0;
                while((can_transmit_status_get(CAN2, (can_tx_mailbox_num_type)mbox) != CAN_TX_STATUS_SUCCESSFUL)&&j<0xFFF)j++;//等待数据发送
            }
        }
        else//不发送结束符
        {
            for(i=0;i<=(LEN-1)/8;i++)//CAN一次最多发送8字节，超过8字节的数据通过多次发送
            {
                for(j=0;j<8;j++)//将8字节数据存入CAN的数据缓存区
                {
                    if(k<LEN)TxMessage.data[j]=data[i*8+j];//将要发送的数据存入CAN的数据缓存区
                    k++;
                    if(k==LEN)break;//数据打包完成，跳出循环
                }
                TxMessage.dlc=k-i*8;//当前数据包大小
                mbox=can_message_transmit(CAN2, &TxMessage);//发送数据
                j=0;
                while((can_transmit_status_get(CAN2, (can_tx_mailbox_num_type)mbox) != CAN_TX_STATUS_SUCCESSFUL)&&j<0xFFF)j++;//等待数据发送
            }
        }
    }
}



u8 CAN1_DATA_BUF0[129]={0};//CAN数据缓存（最后一位为存放数据的长度）
u8 CAN1_DATA_BUF1[129]={0};//CAN数据缓存（最后一位为存放数据的长度）
u8 CAN1_RX0_AIM_LEN=8;
u8 CAN1_RX1_AIM_LEN=8;
u8 CAN2_DATA_BUF0[129]={0};//CAN数据缓存（最后一位为存放数据的长度）
u8 CAN2_DATA_BUF1[129]={0};//CAN数据缓存（最后一位为存放数据的长度）
u8 CAN2_RX0_AIM_LEN=8;
u8 CAN2_RX1_AIM_LEN=8;
//CAN接收数据
//buf：接收数据存放地址
//len：接收数据长度  len=0时,使用结束标志符判断是否接收完成
u32 Can_Rx(u8 CANx,u8 FIFOx,u8*buf,u8 len)
{
    u8 i,Len;
    if(CANx==1)
    {
        if(FIFOx==0)CAN1_RX0_AIM_LEN=len;
        if(FIFOx==1)CAN1_RX1_AIM_LEN=len;
        if(CAN1_DATA_BUF0[128]&&FIFOx==0)//接收到数据
        {
            if(len==0)Len=CAN1_DATA_BUF0[128];//得到数据长度
            else Len=len;
            CAN1_DATA_BUF0[128]=0;//清空数据长度
            for(i=0;i<Len;i++)buf[i]=CAN1_DATA_BUF0[i];//将数据复制到接收数据存放地址
            return Len;//返回数据长度
        }
        if(CAN1_DATA_BUF1[128]&&FIFOx==1)//接收到数据
        {
            if(len==0)Len=CAN1_DATA_BUF1[128];//得到数据长度
            else Len=len;
            CAN1_DATA_BUF1[128]=0;//清空数据长度
            for(i=0;i<Len;i++)buf[i]=CAN1_DATA_BUF1[i];//将数据复制到接收数据存放地址
            return Len;//返回数据长度
        }
    }
    if(CANx==2)
    {
        if(FIFOx==0)CAN2_RX0_AIM_LEN=len;
        if(FIFOx==1)CAN2_RX1_AIM_LEN=len;
        if(CAN2_DATA_BUF0[128]&&FIFOx==0)//接收到数据
        {
            if(len==0)Len=CAN2_DATA_BUF0[128];//得到数据长度
            else Len=len;
            CAN2_DATA_BUF0[128]=0;//清空数据长度
            for(i=0;i<Len;i++)buf[i]=CAN2_DATA_BUF0[i];//将数据复制到接收数据存放地址
            return Len;//返回数据长度
        }
        if(CAN2_DATA_BUF1[128]&&FIFOx==1)//接收到数据
        {
            if(len==0)Len=CAN2_DATA_BUF1[128];//得到数据长度
            else Len=len;
            CAN2_DATA_BUF1[128]=0;//清空数据长度
            for(i=0;i<Len;i++)buf[i]=CAN2_DATA_BUF1[i];//将数据复制到接收数据存放地址
            return Len;//返回数据长度
        }
    }
    return 0;//没有数据，则返回0
}




uint32_t CAN1_RX_ID0=0;//CAN1_FIFO0接收id
u8 CAN1_RX_BUF0[128];//CAN1_FIFO0接收缓存
u8 CAN1_LEN0=0;//CAN1_FIFO0接收长度
void(*CAN1_RX0_IRQ)(uint32_t id,u8*data)=NULL;//CAN1_RX0函数指针   dm
void USBFS_L_CAN1_RX0_IRQHandler(void)//CAN1邮箱0接收中断服务函数
{
    u32 i,j;
    can_rx_message_type RxMessage;
    can_message_receive(CAN1, CAN_RX_FIFO0, &RxMessage);//接收数据
    if(RxMessage.id_type==CAN_ID_STANDARD)CAN1_RX_ID0=RxMessage.standard_id;//得到标准id
    else CAN1_RX_ID0=RxMessage.extended_id;//得到扩展id
    if(CAN1_RX0_IRQ!=NULL)//装载了其他函数，将为其他函数服务
    {
        (CAN1_RX0_IRQ)(CAN1_RX_ID0,RxMessage.data);
        can_flag_clear(CAN1,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
        return;
    }
    for(i=0;i<RxMessage.dlc;i++)
    {
        CAN1_RX_BUF0[CAN1_LEN0]=RxMessage.data[i];//将接收的数据复制到接收缓存
        CAN1_LEN0++;//接收长度+1
        if(CAN1_RX0_AIM_LEN)
        {
            if(CAN1_RX0_AIM_LEN==CAN1_LEN0)//指定长度接收
            {
                for(j=0;j<CAN1_LEN0;j++)CAN1_DATA_BUF0[j]=CAN1_RX_BUF0[j];//将数据复制到CAN数据缓存区
                CAN1_DATA_BUF0[128]=CAN1_LEN0;//将数据长度存放到数据缓冲区最后一位
                CAN1_LEN0=0;//清空接收长度
                can_flag_clear(CAN1,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
                return;
            }
        }
        else//结束标志符接收
        {
            if(CAN1_RX_BUF0[CAN1_LEN0-1]==(CAN1_LEN0-3)&&CAN1_RX_BUF0[CAN1_LEN0-2]=='\n'&&CAN1_RX_BUF0[CAN1_LEN0-3]=='\r')//接收到"\r\n"，且接收的长度与发送长度一致，说明数据接收完成且正确
            {
                for(j=0;j<CAN1_LEN0-3;j++)CAN1_DATA_BUF0[j]=CAN1_RX_BUF0[j];//将数据复制到CAN数据缓存区
                CAN1_DATA_BUF0[128]=CAN1_LEN0-3;//将数据长度存放到数据缓冲区最后一位
                CAN1_LEN0=0;//清空接收长度
                can_flag_clear(CAN1,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
                return;
            }
            else if (CAN1_RX_BUF0[CAN1_LEN0-1]!=(CAN1_LEN0-3)&&CAN1_RX_BUF0[CAN1_LEN0-2]=='\n'&&CAN1_RX_BUF0[CAN1_LEN0-3]=='\r')CAN1_LEN0=0;//接收到"\r\n"，但接收的长度与发送长度不一致，说明数据接收完成但接收错误,把该数据丢弃
        }
        if(CAN1_LEN0>=128)CAN1_LEN0=0;//清空接收长度
    }
   can_flag_clear(CAN1,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
}


uint32_t CAN1_RX_ID1=0;//CAN1_FIFO1,接收id
u8 CAN1_RX_BUF1[128];//CAN1_FIFO1接收缓存
u8 CAN1_LEN1=1;//CAN1_FIFO1接收长度
void CAN1_RX1_IRQHandler(void)//CAN1邮箱1接收中断服务函数
{
    u32 i,j;
    can_rx_message_type RxMessage;
    can_message_receive(CAN1, CAN_RX_FIFO1, &RxMessage);//接收数据
    if(RxMessage.id_type==CAN_ID_STANDARD)CAN1_RX_ID1=RxMessage.standard_id;//得到标准id
    else CAN1_RX_ID1=RxMessage.extended_id;//得到扩展id
    for(i=0;i<RxMessage.dlc;i++)
    {
        CAN1_RX_BUF1[CAN1_LEN1]=RxMessage.data[i];//将接收的数据复制到接收缓存
        CAN1_LEN1++;//接收长度+1
        if(CAN1_RX1_AIM_LEN)//指定长度接收
        {
            if(CAN1_RX1_AIM_LEN==CAN1_LEN1)
            {
                for(j=0;j<CAN1_LEN1;j++)CAN1_DATA_BUF1[j]=CAN1_RX_BUF1[j];//将数据复制到CAN数据缓存区
                CAN1_DATA_BUF1[128]=CAN1_LEN1;//将数据长度存放到数据缓冲区最后一位
                CAN1_LEN1=0;//清空接收长度
                can_flag_clear(CAN1,CAN_RF1FF_FLAG);//清除FIFO1，从而清除中断标志
                return;
            }
        }
        else//结束标志符接收
        {
            if(CAN1_RX_BUF1[CAN1_LEN1-1]==(CAN1_LEN1-3)&&CAN1_RX_BUF1[CAN1_LEN1-2]=='\n'&&CAN1_RX_BUF1[CAN1_LEN1-3]=='\r')//接收到"\r\n"，且接收的长度与发送长度一致，说明数据接收完成且正确
            {
                for(j=0;j<CAN1_LEN1-3;j++)CAN1_DATA_BUF1[j]=CAN1_RX_BUF1[j];//将数据复制到CAN数据缓存区
                CAN1_DATA_BUF1[128]=CAN1_LEN1-3;//将数据长度存放到数据缓冲区最后一位
                CAN1_LEN1=0;//清空接收长度
                can_flag_clear(CAN1,CAN_RF1FF_FLAG);//清除FIFO1，从而清除中断标志
                return;
            }
            else if (CAN1_RX_BUF1[CAN1_LEN1-1]!=(CAN1_LEN1-3)&&CAN1_RX_BUF1[CAN1_LEN1-2]=='\n'&&CAN1_RX_BUF1[CAN1_LEN1-3]=='\r')CAN1_LEN1=0;//接收到"\r\n"，但接收的长度与发送长度不一致，说明数据接收完成但接收错误,把该数据丢弃
        }
        if(CAN1_LEN1>=128)CAN1_LEN1=0;//清空接收长度
    }
    can_flag_clear(CAN1,CAN_RF1FF_FLAG);//清除FIFO1，从而清除中断标志
}

uint32_t CAN2_RX_ID0=0;//CAN2_FIFO0接收id
u8 CAN2_RX_BUF0[128];//CAN2_FIFO0接收缓存
u8 CAN2_LEN0=0;//CAN2FIFO0接收长度
void CAN2_RX0_IRQHandler(void)//CAN2邮箱0接收中断服务函数
{
    u32 i,j;
    can_rx_message_type RxMessage;
    can_message_receive(CAN2, CAN_RX_FIFO0, &RxMessage);//接收数据
    if(RxMessage.id_type==CAN_ID_STANDARD)CAN2_RX_ID0=RxMessage.standard_id;//得到标准id
    else CAN2_RX_ID0=RxMessage.extended_id;//得到扩展id
    for(i=0;i<RxMessage.dlc;i++)
    {
        CAN2_RX_BUF0[CAN2_LEN0]=RxMessage.data[i];//将接收的数据复制到接收缓存
        CAN2_LEN0++;//接收长度+1
        if(CAN2_RX0_AIM_LEN)
        {
            if(CAN2_RX0_AIM_LEN==CAN2_LEN0)//指定长度接收
            {
                for(j=0;j<CAN2_LEN0;j++)CAN2_DATA_BUF0[j]=CAN2_RX_BUF0[j];//将数据复制到CAN数据缓存区
                CAN2_DATA_BUF0[128]=CAN2_LEN0;//将数据长度存放到数据缓冲区最后一位
                CAN2_LEN0=0;//清空接收长度
                can_flag_clear(CAN2,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
                return;
            }
        }
        else//结束标志符接收
        {
            if(CAN2_RX_BUF0[CAN2_LEN0-1]==(CAN2_LEN0-3)&&CAN2_RX_BUF0[CAN2_LEN0-2]=='\n'&&CAN2_RX_BUF0[CAN2_LEN0-3]=='\r')//接收到"\r\n"，且接收的长度与发送长度一致，说明数据接收完成且正确
            {
                for(j=0;j<CAN2_LEN0-3;j++)CAN2_DATA_BUF0[j]=CAN2_RX_BUF0[j];//将数据复制到CAN数据缓存区
                CAN2_DATA_BUF0[128]=CAN2_LEN0-3;//将数据长度存放到数据缓冲区最后一位
                CAN2_LEN0=0;//清空接收长度
                can_flag_clear(CAN2,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
                return;
            }
            else if (CAN2_RX_BUF0[CAN2_LEN0-1]!=(CAN2_LEN0-3)&&CAN2_RX_BUF0[CAN2_LEN0-2]=='\n'&&CAN2_RX_BUF0[CAN2_LEN0-3]=='\r')CAN2_LEN0=0;//接收到"\r\n"，但接收的长度与发送长度不一致，说明数据接收完成但接收错误,把该数据丢弃
        }
        if(CAN2_LEN0>=128)CAN2_LEN0=0;//清空接收长度
    }
    can_flag_clear(CAN2,CAN_RF0FF_FLAG);//清除FIFO0，从而清除中断标志
}

uint32_t CAN2_RX_ID1=0;//CAN2_FIFO1,接收id
u8 CAN2_RX_BUF1[256];//CAN2_FIFO1接收缓存  //128
u8 CAN2_LEN1=1;//CAN2_FIFO1接收长度
void(*CAN2_RX1_IRQ)(uint32_t id,u8*data)=NULL;//CAN2_RX1函数指针
void CAN2_RX1_IRQHandler(void)//CAN2邮箱1接收中断服务函数         rm电机数据接收中断
{
    u32 i,j;
    can_rx_message_type RxMessage;
    can_message_receive(CAN2, CAN_RX_FIFO1, &RxMessage);//接收数据
    if(RxMessage.id_type==CAN_ID_STANDARD)CAN2_RX_ID1=RxMessage.standard_id;//得到标准id
    else CAN2_RX_ID1=RxMessage.extended_id;//得到扩展id
    if(CAN2_RX1_IRQ!=NULL)//装载了其他函数，将为其他函数服务
    {
        (CAN2_RX1_IRQ)(CAN2_RX_ID1,RxMessage.data);
        can_flag_clear(CAN2,CAN_RF1FF_FLAG);//清除FIFO0，从而清除中断标志
        return;
    }
    for(i=0;i<RxMessage.dlc;i++)
    {
        CAN2_RX_BUF1[CAN2_LEN1]=RxMessage.data[i];//将接收的数据复制到接收缓存
        CAN2_LEN1++;//接收长度+1
        if(CAN2_RX1_AIM_LEN)//指定长度接收
        {
            if(CAN2_RX1_AIM_LEN==CAN2_LEN1)
            {
                for(j=0;j<CAN2_LEN1;j++)CAN2_DATA_BUF1[j]=CAN2_RX_BUF1[j];//将数据复制到CAN数据缓存区
                CAN2_DATA_BUF1[128]=CAN2_LEN1;//将数据长度存放到数据缓冲区最后一位
                CAN2_LEN1=0;//清空接收长度
                can_flag_clear(CAN2,CAN_RF1FF_FLAG);//清除FIFO1，从而清除中断标志
                return;
            }
        }
        else//结束标志符接收
        {
            if(CAN2_RX_BUF1[CAN2_LEN1-1]==(CAN2_LEN1-3)&&CAN2_RX_BUF1[CAN2_LEN1-2]=='\n'&&CAN2_RX_BUF1[CAN2_LEN1-3]=='\r')//接收到"\r\n"，且接收的长度与发送长度一致，说明数据接收完成且正确
            {
                for(j=0;j<CAN2_LEN1-3;j++)CAN2_DATA_BUF1[j]=CAN2_RX_BUF1[j];//将数据复制到CAN数据缓存区
                CAN2_DATA_BUF1[128]=CAN2_LEN1-3;//将数据长度存放到数据缓冲区最后一位
                CAN2_LEN1=0;//清空接收长度
                return;
            }
            else if (CAN2_RX_BUF1[CAN2_LEN1-1]!=(CAN2_LEN1-3)&&CAN2_RX_BUF1[CAN2_LEN1-2]=='\n'&&CAN2_RX_BUF1[CAN2_LEN1-3]=='\r')CAN2_LEN1=0;//接收到"\r\n"，但接收的长度与发送长度不一致，说明数据接收完成但接收错误,把该数据丢弃
        }
        if(CAN2_LEN1>=128)CAN2_LEN1=0;//清空接收长度
    }
    can_flag_clear(CAN2,CAN_RF1FF_FLAG);//清除FIFO1，从而清除中断标志
}


////=================================================================RS485====================================================================\\


volatile unsigned long*de_gpio[3];//RS485发送使能IO口映射


//RS485初始化
//USARTx：串口号
//bound：波特率
//DE_GPIO：发送使能IO口GPIO
//DE_PIN：发送使能IO口PIN
void RS485_Init(u8 USARTx,u32 bound,u8 DE_GPIO,u8 DE_PIN)
{
    Uartx_Init(USARTx,bound);
    Gpio_Init(DE_GPIO,DE_PIN,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,100);//初始化发送使能口
    switch(DE_GPIO)
    {
        case 'A':de_gpio[USARTx-1]=&PAout(DE_PIN);break;
        case 'B':de_gpio[USARTx-1]=&PBout(DE_PIN);break;
        case 'C':de_gpio[USARTx-1]=&PCout(DE_PIN);break;
        case 'D':de_gpio[USARTx-1]=&PDout(DE_PIN);break;
        case 'E':de_gpio[USARTx-1]=&PEout(DE_PIN);break;
    }
    *de_gpio[USARTx-1]=0;//启动接收
}

//RS485发送
//USARTx：串口号
//data：发送数据
//len：数据长度
void RS485_Tx(u8 USARTx,char*data,u32 len)
{
    *de_gpio[USARTx-1]=1;//开启RS485发送
    USARTx_Tx(USARTx,data,len);
    *de_gpio[USARTx-1]=0;//结束发送，开启RS485接收
}

//RS485发接收
//USARTx：串口号
//data：发送数据
//len：数据长度
//接收到数据：返回串口号，反之返回0
u8 RS485_Rx(u8 USARTx,char*buf,u32 len)
{
    return USARTx_Rx(USARTx,buf,len);
}
