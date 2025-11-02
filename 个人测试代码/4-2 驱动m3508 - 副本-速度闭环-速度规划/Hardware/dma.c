#include "dma.h"
#include "F1_dma.h"
/**
********************(C)COPYRIGHT 2024 行者********************
* @author      陈楚莹
* @Date        2024.10.31
* @function    基于F1的串口+DMA，STM32F103ZET6的串口1-4，STM32F103C8T6的串口1-3
*
* @note				 直接移植时，请注意：
							 1.需要进行串口DMA初始化，即	USART_TXRX_DMA_Init();
							 2.查需要的串口使能标志位是否为1，如：USART1_DMA_EN
							 3.视觉数据接收时，DMA接收模式最好为循环接收，并且不需要再中断中加数据移出函数
							 4.检查发送数组和接收数组的数组长度有没有设置的太小，如：SENDBUFF_SIZE、REBUFF_SIZE
* @remark
=============================================================
=============================================================
********************(C)COPYRIGHT 2023 行者********************
**/
//注意根据需求，修改数组类型/*注意同步修改头文件的扩展 /*用于字符,16进制,int形式处理

//DMA专用数组
uint8_t SendBuff[SENDBUFF_SIZE] = {0};		 //USART1发送缓冲数组/*
uint8_t SendBuff2[SENDBUFF2_SIZE] = {0};   //USART2发送缓冲数组/*
uint8_t SendBuff3[SENDBUFF3_SIZE] = {0};   //USART3发送缓冲数组/*
uint8_t SendBuff4[SENDBUFF4_SIZE] = {0};   //UART4发送缓冲数组/*

uint8_t ReBuff[REBUFF_SIZE] = {0};				 //USART1接收缓冲数组/*
uint8_t ReBuff2[REBUFF2_SIZE] = {0};  		 //USART2接收缓冲数组/*
uint8_t ReBuff3[REBUFF3_SIZE] = {0}; 		 	 //USART3接收缓冲数组/*
uint8_t ReBuff4[REBUFF4_SIZE] = {0};			 //UART4接收缓冲数组/*

//调试使用的串口1数组存放数组
uint8_t usart_data[REBUFF_SIZE];
uint8_t usart_data2[REBUFF2_SIZE];
uint8_t usart_data3[REBUFF3_SIZE];
uint8_t usart_data4[REBUFF4_SIZE];


//循环队列使用的当前队列尾指针
volatile uint8_t* usart2_end = usart_data2;
volatile uint8_t* usart3_end = usart_data3;


/**
  * @brief  对数据进行解码处理  这一部分根据自己的协议DIY，只用取走自己需要的数据即可
  * @param  选择 处理的串口数据
  * @retval 无
  */
void USART_DATA(USART_TypeDef* USARTx)
{
	int i;
    if(USARTx == USART1)
    {
			for(i=0;i<REBUFF_SIZE;i++)usart_data[i]=ReBuff[i];//复制数据
    }
		
    else if(USARTx == USART2)
    {
			//for(i=0;i<20;i++) printf("%d\r\n",SendBuff2[i]);
			for(i=0;i<REBUFF_SIZE;i++)usart_data2[i]=ReBuff2[i]; //复制数据
			//for(i=0;i<REBUFF_SIZE;i++)DMA_Printf(USART1,"%x\t",usart_data2[i]); //向串口1发送
    }

    else if(USARTx == USART3)
    {
			//for(i=0;i<20;i++) printf("%d\r\n",SendBuff3[i]);
//			Data_update(ReBuff3,&usart_st[1]);    //将串口2缓冲区数据更新到串口3结构体中
//			for(i=0;i<REBUFF3_SIZE;i++)usart_data3[i]=ReBuff3[i]; //复制数据
    }
		else if(USARTx == UART4)
    {

    }
}


/**
  * @brief  USARTx TXRx DMA 初始化
  * @param  无
  * @retval 无
  */
void USART_TXRX_DMA_Init(void)
{
    #if USART1_DMA_EN
    //USART1 TXRX DMA 配置/*
    USART1_TX_DMA_Init();
    USART1_RX_DMA_Init();

    //USART1 DMA 接收使能/*
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    //USART1 DMA 发送使能/*
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    #endif

    #if USART2_DMA_EN
    //USART2 TXRX DMA 配置/*
		USART2_TX_DMA_Init();
    USART2_RX_DMA_Init();
    //USART2 DMA 接收使能/*
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    //USART2 DMA 发送使能/*
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    #endif

    #if USART3_DMA_EN
    //USART3 TXRX DMA 配置/*
		USART3_TX_DMA_Init();
    USART3_RX_DMA_Init();
    //USART3 DMA 接收使能/*
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
    //USART3 DMA 发送使能/*
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    #endif
		
		#if UART4_DMA_EN
    //USART4 TXRX DMA 配置/*
		UART4_TX_DMA_Init();
    UART4_RX_DMA_Init();
    //USART4 DMA 接收使能/*
    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
    //USART4 DMA 发送使能/*
    USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
    #endif
		
}


/**
  * @brief  USART1 TX DMA 配置,内存到外设(USART1->DR)
  * @param  无
  * @retval 无
		DMA1通道4:USART1_TX
  */
static void USART1_TX_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel4);   //将DMA1的通道4,是USART1_TX
	//while (DMA_GetFlagStatus(DMA1_FLAG_TC1) != DISABLE){}//等待DMA可配置 


	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 1;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel4, ENABLE);	
} 




/**
  * @brief  USART2 TX DMA 配置,内存到外设(USART2->DR)
						DMA1通道7
  * @param  无
  * @retval 无
  */
static void USART2_TX_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel7);   //将DMA1的通道7,是USART2_TX

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff2;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 1;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel7, ENABLE);	
}

/**
  * @brief  USART3 TX DMA 配置,内存到外设
						DMA1通道2
  * @param  无
  * @retval 无
  */
static void USART3_TX_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel2);   //将DMA1的通道2,是USART3_TX

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff3;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 1;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel2, ENABLE);	
}

/**
  * @brief  USART4 TX DMA 配置,内存到外设
				DMA2通道5
  * @param  无
  * @retval 无
  */
static void UART4_TX_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA2_Channel5);   //将DMA2的通道5,是USART4_TX

	DMA_InitStructure.DMA_PeripheralBaseAddr = UART4_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff4;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 1;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA2_Channel5, ENABLE);	
}



/**
  * @brief  USART1 RX DMA 配置，外设(USART1->DR)到内存
			DMA1通道5
  * @param  无
  * @retval 无
  */
static void USART1_RX_DMA_Init(void)
{
	
	DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel5);   //将DMA1的通道5,是USART1_RX

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ReBuff;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，外设->内存
	DMA_InitStructure.DMA_BufferSize = REBUFF_SIZE;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel5, ENABLE);	
}

/**
  * @brief  USART2 RX DMA 配置，外设(USART2->DR)到内存
						DMA1通道6
  * @param  无
  * @retval 无
  */
static void USART2_RX_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel6);   //将DMA1的通道6,是USART2_RX

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ReBuff2;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，外设->内存
	DMA_InitStructure.DMA_BufferSize = REBUFF2_SIZE;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel6, ENABLE);	
}
/**
  * @brief  USART3 RX DMA 配置，外设(USART3->DR)到内存
			DMA1通道3
  * @param  无
  * @retval 无
  */
static void USART3_RX_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel3);   //将DMA1的通道3,是USART3_RX

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ReBuff3;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，外设->内存
	DMA_InitStructure.DMA_BufferSize = REBUFF3_SIZE;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel3, ENABLE);	
}

/**
  * @brief  USART4 RX DMA 配置，外设(USART3->DR)到内存
				DMA2通道3
	* @param  无
  * @retval 无
  */
static void UART4_RX_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA2_Channel3);   //将DMA1的通道3,是USART4_RX

	DMA_InitStructure.DMA_PeripheralBaseAddr = UART4_DR_ADDRESS;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ReBuff4;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，外设->内存
	DMA_InitStructure.DMA_BufferSize = REBUFF4_SIZE;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA2_Channel3, ENABLE);	
}



/**
  * @brief  USART1_DMA发送设置
  * @param  size：发送字节长度
  * @retval 无
  */
void USART1_Transmit_DMA(uint16_t size)
{
    DMA_Cmd(DMA1_Channel4, DISABLE); //先关闭通道/*

//    DMA_ClearFlag(DMA1_FLAG_TC4); 
	
    DMA_SetCurrDataCounter(DMA1_Channel4, size); //写入传输个数/*

    DMA_Cmd(DMA1_Channel4, ENABLE); //开启通道/*
}


void USART2_Transmit_DMA(uint16_t size)
{
    DMA_Cmd(DMA1_Channel7, DISABLE); //先关闭通道/*

//	    DMA_ClearFlag(DMA1_FLAG_TC7); 

    DMA_SetCurrDataCounter(DMA1_Channel7, size); //写入传输个数/*

    DMA_Cmd(DMA1_Channel7, ENABLE); //开启通道/*
}

void USART3_Transmit_DMA(uint16_t size)
{
   DMA_Cmd(DMA1_Channel2, DISABLE); //先关闭通道/*
//    while (DMA_GetCmdStatus(USART1_TX_DMA_Stream) != DISABLE);   //确保DMA可以被配置/*
	
//    DMA_ClearFlag(DMA1_FLAG_TC2); 
	
    DMA_SetCurrDataCounter(DMA1_Channel2, size); //写入传输个数/*

    DMA_Cmd(DMA1_Channel2, ENABLE); //开启通道/*
}

void UART4_Transmit_DMA(uint16_t size)
{
   DMA_Cmd(DMA2_Channel5, DISABLE); //先关闭通道/*
//    while (DMA_GetCmdStatus(USART1_TX_DMA_Stream) != DISABLE);   //确保DMA可以被配置/*
	
//    DMA_ClearFlag(DMA2_FLAG_TC5); 
	
    DMA_SetCurrDataCounter(DMA2_Channel5, size); //写入传输个数/*

    DMA_Cmd(DMA2_Channel5, ENABLE); //开启通道/*
}


/**
  * @brief  USART1_DMA接收设置
  * @param  无
  * @retval 无
  */
static void USART1_Receive_DMA(void)
{
    DMA_Cmd(DMA1_Channel5, DISABLE); //先关闭通道/*

//    while (DMA_GetCmdStatus(USART1_RX_DMA_Stream) != DISABLE);   //确保DMA可以被配置/*
	
    DMA_ClearFlag(DMA1_FLAG_TC5); //清除事件标志位/*

//    //DMA缓存大小-DMA缓存剩余大小/*
//    len = REBUFF_SIZE - DMA_GetCurrDataCounter (USART1_RX_DMA_Stream) ;

    DMA_SetCurrDataCounter(DMA1_Channel5, SENDBUFF_SIZE); //写入传输个数/*

    DMA_Cmd(DMA1_Channel5, ENABLE); //开启通道/*
}

static void USART2_Receive_DMA(void)
{
    DMA_Cmd(DMA1_Channel7, DISABLE); //先关闭通道/*

//    while (DMA_GetCmdStatus(USART2_RX_DMA_Stream) != DISABLE);   //确保DMA可以被配置/*

    DMA_ClearFlag(DMA1_FLAG_TC7); //清除事件标志位/*

    DMA_SetCurrDataCounter(DMA1_Channel7, SENDBUFF2_SIZE); //写入传输个数/*

    DMA_Cmd(DMA1_Channel7, ENABLE); //开启通道/*
}

static void USART3_Receive_DMA(void)
{
    DMA_Cmd(DMA1_Channel2, DISABLE); //先关闭通道/*

//    while (DMA_GetCmdStatus(USART3_RX_DMA_Stream) != DISABLE);   //确保DMA可以被配置/*

    DMA_ClearFlag(DMA1_FLAG_TC2); //清除事件标志位/*

    DMA_SetCurrDataCounter(DMA1_Channel2, SENDBUFF3_SIZE); //写入传输个数/*

    DMA_Cmd(DMA1_Channel2, ENABLE); //开启通道/*
}

static void UART4_Receive_DMA(void)
{
    DMA_Cmd(DMA2_Channel5, DISABLE); //先关闭通道/*

//    while (DMA_GetCmdStatus(UART4_RX_DMA_Stream) != DISABLE);   //确保DMA可以被配置/*

    DMA_ClearFlag(DMA2_FLAG_TC5); //清除事件标志位/*

    DMA_SetCurrDataCounter(DMA2_Channel5, SENDBUFF4_SIZE); //写入传输个数/*

    DMA_Cmd(DMA2_Channel5, ENABLE); //开启通道/*
}


/**
  * @brief  printf的DMA实现
  * @param  无
  * @retval 无
  */
void DMA_Printf(USART_TypeDef* USARTx,const char *format, ...) //传递函数/*形成参数列表
{
    u32 len;
    //定义指针变量/*
    va_list args;   //args:字符串变量名（引用变量）/*
    //使指针 指向 参数列表的第一个参数0*
    va_start(args, format);
	
    //将format按字节转到数组SendBuff/*返回值为字节长度/*vsprintf (不按字节)
	if(USARTx == USART1) 
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4)==RESET);//等待传输完成置位
		len = vsnprintf((char*)SendBuff, sizeof(SendBuff), (char*)format, args);
		USART1_Transmit_DMA(len);
	}
	else if(USARTx == USART2) 
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET);//等待传输完成置位
		len = vsnprintf((char*)SendBuff2, sizeof(SendBuff2), (char*)format, args);
		USART2_Transmit_DMA(len);
	}
	else if(USARTx == USART3) 
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC2)==RESET);
		len = vsnprintf((char*)SendBuff3, sizeof(SendBuff3), (char*)format, args);
		USART3_Transmit_DMA(len);
	}
	else if(USARTx == UART4) 
	{
		while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);
		len = vsnprintf((char*)SendBuff4, sizeof(SendBuff4), (char*)format, args);
		UART4_Transmit_DMA(len);
	}
    //关闭指针变量/*释放资源
    va_end(args);
}



/**
  * @brief  USART串口中断DMA数据处理
  * @param  选择 处理的串口数据
  * @retval 无
  */
void USART_DATA_DMA(USART_TypeDef* USARTx)
{
	
    if(USARTx == USART1)
    {
        USART_DATA(USARTx);              //数据解码/*
				My_memset(ReBuff,sizeof(ReBuff));//解放数组/*

			
        //重设置DMA传输/*
        USART1_Receive_DMA();
    }

    else if(USARTx == USART2)
    {
        USART_DATA(USARTx);                //数据解码/
				My_memset(ReBuff2,sizeof(ReBuff2));//解放数组/*
			
        //重设置DMA传输/*
        USART2_Receive_DMA();
    }

    else if(USARTx == USART3)
    {

       USART_DATA(USARTx);                //数据解码/*
		 	 My_memset(ReBuff3,sizeof(ReBuff3));//解放数组/*
			
        //重设置DMA传输/*
      USART3_Receive_DMA();
    }
		
		else if(USARTx == UART4)
    {
        USART_DATA(USARTx);                //数据解码/*
				My_memset(ReBuff4,sizeof(ReBuff4));//解放数组/*
			
        //重设置DMA传输/*
      UART4_Receive_DMA();
    }
}

/**
  * @brief  数组清空函数
  * @param  数组首地址，数组长度（字节数）
  * @retval 无
  */
void My_memset(uint8_t *Buff,int len)
{
	int i;
	u8* buf;
	buf=(u8*)Buff;
  for(i=0;i<len;i++)*buf++=0;
}

























