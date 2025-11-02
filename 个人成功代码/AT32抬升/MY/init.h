#ifndef __INIT_H
#define __INIT_H
#include<init_simplify.h>
#include <sys.h>



//TIM
#define Up          0x00 //TIM向上计数模式
#define Down        0x01 //TIM向下计数模式
#define CA1         0x02//TIM中央对齐计数模式1
#define CA2         0x04//TIM中央对齐计数模式2
#define CA3         0x06//TIM中央对齐计数模式3

//PWM
#define High        8//TIM输出比较极性高
#define Low         12//TIM输出比较极性低


//DMA
#define Tx DMA_DIR_MEMORY_TO_PERIPHERAL //数据传输由内存到外设
#define Rx DMA_DIR_PERIPHERAL_TO_MEMORY//数据传输由外设到内存

#define P8 DMA_PERIPHERAL_DATA_WIDTH_BYTE//外设数据宽度8位
#define P16 DMA_PERIPHERAL_DATA_WIDTH_HALFWORD//外设数据宽度16位
#define P32 DMA_PERIPHERAL_DATA_WIDTH_WORD//外设数据宽度32位

#define M8 DMA_MEMORY_DATA_WIDTH_BYTE//内存数据宽度8位
#define M16 DMA_MEMORY_DATA_WIDTH_HALFWORD//内存数据宽度16位
#define M32 DMA_MEMORY_DATA_WIDTH_WORD//内存数据宽度32位

#define very high DMA_PRIORITY_VERY_HIGH//优先级很高
#define high DMA_PRIORITY_HIGH//优先级高
#define medium DMA_PRIORITY_MEDIUM//优先级中
#define low DMA_PRIORITY_LOW////优先级低



////输入捕获
#define Rising TMR_INPUT_RISING_EDGE
#define Falling TMR_INPUT_FALLING_EDGE



//串口
#define usart1 1//串口1
#define usart2 2//串口2
#define usart3 3//串口3
#define usart4 4//串口4
#define usart5 5//串口5
#define usart6 6//串口6
#define usart7 7//串口7

//CAN ID格式
#define CAN_Id_Mode_StdId 0//标准ID
#define CAN_Id_Mode_ExtId 1//扩展ID


extern void (*USART2_IDLE_IRQ)(void);//串口2空闲函数指针
extern void (*USART3_IDLE_IRQ)(void);//串口3空闲函数指针
extern void (*USART4_IDLE_IRQ)(void);//串口4空闲函数指针
extern void (*USART5_IDLE_IRQ)(void);//串口5空闲函数指针
extern void (*USART6_IDLE_IRQ)(void);//串口6空闲函数指针
extern void (*USART7_IDLE_IRQ)(void);//串口7空闲函数指针
extern void (*USART2_RX_IRQ)(u8 data);//串口2接收函数指针
extern void (*USART3_RX_IRQ)(u8 data);//串口3接收函数指针
extern void (*USART4_RX_IRQ)(u8 data);//串口4接收函数指针
extern void (*USART5_RX_IRQ)(u8 data);//串口5接收函数指针
extern void (*USART6_RX_IRQ)(u8 data);//串口6接收函数指针
extern void (*USART7_RX_IRQ)(u8 data);//串口7接收函数指针

extern void(*CAN1_RX0_IRQ)(uint32_t id,u8*data);//CAN1_RX0函数指针
extern void(*CAN2_RX1_IRQ)(uint32_t id,u8*data);//CAN2_RX1函数指针



void Gpio_Init(char GPIO,int PIN,int GPIO_MODE,int GPIO_OUTPUT,int GPIO_PULL,int GPIO_DRIVE);//IO口初始化

void Exti_Init(char*GPIO_PIN,u8 EXINT_LINE,u8 EXINT_TRIGGER);//外部中断初始化

void Nvic_Init(int IRQn,int x,int y);//设置中断

void IWDG_Init(int reload_value,int WDT_CLK_DIV);//看门狗初始化
void IWDG_Feed(void);//喂狗

void Tim_Init(int tim,uint32_t arr,uint32_t psc,int mode);//定时器初始化

void Pwm_Init(u8 GPIO,u8 PIN,int mode,int pol,tmr_type* TIMx,int oc);//设置PWM模式

void Dma_Init(dma_type *DMAx,dma_channel_type * DMAx_CHANNELx,u32 cpar,u32 P_size,u32 cmar,u32 M_size,u32 len,u32 DIR,u32 loop_mode,u32 priority,u8 DMA_FLEXIBLE);//设置DMA
void DMA_NO(dma_channel_type * DMAx_CHANNELx,u32 Len);//开启DMA

void Cap_Init(u8 GPIO,u8 PIN,tmr_type*TMRx,u8 Channel,u8 TMR_INPUT,u8 ICFilter);//定时器捕获初始化

void Spi_Init(spi_type*SPIx,u16 direction,u16 mode,u16 datasize,u16 cpol,u16 cpha,u16 baudrateprescaler,u16 firstbit);//初始化SPI
u8 Spi_RW(spi_type*SPIx,u8 data);//SPI传输数据

void Adc_Init(char GPIO,u8 PIN,adc_type *ADCx);//初始化ADC
u16 Adc_cap(adc_type *ADCx,adc_channel_select_type channel);//得到ADC数据

void Uartx_Init(u8 USARTx,u32 bound);//初始化串口
void USARTx_Tx(u8 USARTx,char*BUF,u16 Len);//串口发送
u8 USARTx_Rx(u8 USARTx,char*BUF,u16 Len);//串口接收

void Can_Init(u8 CANx,u8 mode,u8 CAN_RSAW,u8 CAN_BTS1,u8 CAN_BTS2,u8 can_prescaler,u8 CAN_FilterNumber,u8 CAN_FILTER_BIT,u8 CAN_Id_Mode,unsigned int CAN_FilterId,unsigned int CAN_FilterMaskId,u8 CAN_FILTER_FIFO);//初始化CAN
void Can_Tx(u8 CANx,uint32_t IDE,unsigned int ID,u8*data,u32 len,u8 end_sign);//发送数据
u32 Can_Rx(u8 CANx,u8 FIFOx,u8*buf,u8 len);//接收数据
void CAN_GPIO_AF(u8 CANx,u8 Rx_GPIO,u8 Rx_PIN,u8 Tx_GPIO,u8 Tx_PIN);//CAN的IO接口重复用

void RS485_Init(u8 USARTx,u32 bound,u8 DE_GPIO,u8 DE_PIN);//RS485初始化
void RS485_Tx(u8 USARTx,char*data,u32 len);//RS485发送
u8 RS485_Rx(u8 USARTx,char*buf,u32 len);//RS485接收

#endif
