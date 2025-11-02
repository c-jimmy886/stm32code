#ifndef __DMA_H
#define __DMA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "sys.h"
#include "stm32f10x.h"

//适用 裸机 RTOS移植/*RTOS环境代码会做稍微调整
//Address/*地址需要强转32
#define USART1_SR_ADDRESS  ((u32)(&USART1->SR)) //状态寄存器/*
#define USART1_DR_ADDRESS  ((u32)(&USART1->DR)) //数据寄存器/*
#define USART2_SR_ADDRESS  ((u32)(&USART2->SR)) //状态寄存器/*
#define USART2_DR_ADDRESS  ((u32)(&USART2->DR)) //数据寄存器/*
#define USART3_SR_ADDRESS  ((u32)(&USART3->SR)) //状态寄存器/*
#define USART3_DR_ADDRESS  ((u32)(&USART3->DR)) //数据寄存器/*
#define UART4_SR_ADDRESS  ((u32)(&UART4->SR)) //状态寄存器/*
#define UART4_DR_ADDRESS  ((u32)(&UART4->DR)) //数据寄存器/*


//使能EN/*      1为使能,0为未使能
#define USART1_DMA_EN 1
#define USART2_DMA_EN 1
#define USART3_DMA_EN 1
#define UART4_DMA_EN 	1
//Send DATA/*
#define SENDBUFF_SIZE  100
#define SENDBUFF2_SIZE 100
#define SENDBUFF3_SIZE 100
#define SENDBUFF4_SIZE 100
//Re DATA/*//参考长度,根据实际情况修改，在循环队列中，大小应设为所有指令大小的公倍数，避免指针反向寻址失败/*
#define REBUFF_SIZE   100      
#define REBUFF2_SIZE 	100  
#define REBUFF3_SIZE 	100    
#define REBUFF4_SIZE  100



//串口数据缓冲区 /*
extern uint8_t SendBuff[SENDBUFF_SIZE];
extern uint8_t SendBuff2[SENDBUFF2_SIZE];
extern uint8_t SendBuff3[SENDBUFF3_SIZE];
extern uint8_t SendBuff4[SENDBUFF4_SIZE];

extern uint8_t ReBuff[REBUFF_SIZE];
extern uint8_t ReBuff2[REBUFF2_SIZE];
extern uint8_t ReBuff3[REBUFF3_SIZE];
extern uint8_t ReBuff4[REBUFF4_SIZE];


extern uint8_t usart_data[REBUFF_SIZE];
extern uint8_t usart_data2[REBUFF2_SIZE];
extern uint8_t usart_data3[REBUFF3_SIZE];
extern uint8_t usart_data4[REBUFF4_SIZE];


volatile extern uint8_t* usart2_end;
volatile extern uint8_t* usart3_end;


//应用函数/*DMA
void DMA_Printf(USART_TypeDef* USARTx,const char *format,...);//printf dma 调试/*

void USART_DATA_DMA(USART_TypeDef* USARTx);//串口DMA接收判断/*
void USART_DATA(USART_TypeDef* USARTx);//串口接收数据处理/*

//初始化函数/*DMA
static void USART1_TX_DMA_Init(void);//USART1 TX DMA 初始化/*
static void USART1_RX_DMA_Init(void);//USART1 RX DMA 初始化/*
static void USART2_TX_DMA_Init(void);//USART2 TX DMA 初始化/*
static void USART2_RX_DMA_Init(void);//USART2 RX DMA 初始化/*
static void USART3_TX_DMA_Init(void);//USART3 TX DMA 初始化/*
static void USART3_RX_DMA_Init(void);//USART3 RX DMA 初始化/*
static void UART4_TX_DMA_Init(void); //USART3 TX DMA 初始化/*
static void UART4_RX_DMA_Init(void); //USART3 RX DMA 初始化/*


void USART_TXRX_DMA_Init(void);  //USART DMA 初始化/* 总的调用函数，宏定义确定使能

//功能函数/*DMA
void USART1_Transmit_DMA(uint16_t size); //USART1 DMA 发送/*
void USART2_Transmit_DMA(uint16_t size);
void USART3_Transmit_DMA(uint16_t size);
void UART4_Transmit_DMA(uint16_t size) ;

static void USART1_Receive_DMA(void);//USART1 DMA 接收/*
__weak void USART2_Receive_DMA(void );//USART2 DMA 接收/*
__weak void USART3_Receive_DMA(void );//USART3 DMA 接收/*
__weak void UART4_Receive_DMA(void );//USART4 DMA 接收/* 

void My_memset(uint8_t *Buff,int len);

#endif





