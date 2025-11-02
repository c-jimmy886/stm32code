#ifndef __DMA_USART_H
#define __DMA_USART_H	 
#include<sys.h>

#define DMA_BUF_SIZE 256


void dma_uart_init(u8 USARTx,u32 bound);//初始化DMA_USART1
u8 dma_tx(u8 USARTx,char*dma_data,u32 len);//发送数据
u8 dma_rx(u8 USARTx,char*dma_data,u32 len);//接收数据
u8 dma_printf(char* str,...) __attribute__((format(printf,1,2)));
u8 Dma_printf(char* str,...) __attribute__((format(printf,1,2)));
void dma_485_init(u8 USARTx,u32 bound,u8 DE_GPIO,u8 DE_PIN);
u8 dma_485_tx(u8 USARTx,char*data,u32 len);
u8 dma_485_rx(u8 USARTx,char*buf,u32 len);

#endif
