#include<sys.h>
#include<usart.h>
#include<init.h>
#include<string.h>
#include<dma_usart.h>
#include<stdarg.h>
#include<delay.h>

char dma_data_tx1[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx1[DMA_BUF_SIZE];//接收数据缓冲区
char dma_data_tx2[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx2[DMA_BUF_SIZE];//接收数据缓冲区
char dma_data_tx3[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx3[DMA_BUF_SIZE];//接收数据缓冲区
char dma_data_tx4[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx4[DMA_BUF_SIZE];//接收数据缓冲区
char dma_data_tx5[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx5[DMA_BUF_SIZE];//接收数据缓冲区
char dma_data_tx6[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx6[DMA_BUF_SIZE];//接收数据缓冲区
char dma_data_tx7[DMA_BUF_SIZE];//发送数据缓冲区
char dma_data_rx7[DMA_BUF_SIZE];//接收数据缓冲区

int  dma_data_rx_p1=-1;//最新一次接收数据在缓冲区的起始位置
int  dma_data_rx_p2=-1;//最新一次接收数据在缓冲区的起始位置
int  dma_data_rx_p3=-1;//最新一次接收数据在缓冲区的起始位置
int  dma_data_rx_p4=-1;//最新一次接收数据在缓冲区的起始位置
int  dma_data_rx_p5=-1;//最新一次接收数据在缓冲区的起始位置
int  dma_data_rx_p6=-1;//最新一次接收数据在缓冲区的起始位置
int  dma_data_rx_p7=-1;//最新一次接收数据在缓冲区的起始位置

int  dma_data_rx_len1=0;//最新一次接收数据长度
int  dma_data_rx_len2=0;//最新一次接收数据长度
int  dma_data_rx_len3=0;//最新一次接收数据长度
int  dma_data_rx_len4=0;//最新一次接收数据长度
int  dma_data_rx_len5=0;//最新一次接收数据长度
int  dma_data_rx_len6=0;//最新一次接收数据长度
int  dma_data_rx_len7=0;//最新一次接收数据长度

extern u8 DMA_RX1;//DMA接收空闲标志，1空闲，0数据传输中
extern u8 RX2;//DMA接收空闲标志，1空闲，0数据传输中
extern u8 RX3;//DMA接收空闲标志，1空闲，0数据传输中
extern u8 RX4;//DMA接收空闲标志，1空闲，0数据传输中
extern u8 RX5;//DMA接收空闲标志，1空闲，0数据传输中
extern u8 RX6;//DMA接收空闲标志，1空闲，0数据传输中
extern u8 RX7;//DMA接收空闲标志，1空闲，0数据传输中

void dma_uart_init(u8 USARTx,u32 bound)//初始化DMA_USART
{
    delay_init();//初始化延时函数
    if(USARTx==1)
    {
        DMA_RX1=0;
        uart_init(bound);//初始化串口1；
        Dma_Init(DMA1,DMA1_CHANNEL1,(uint32_t)&USART1->dt,P8,(uint32_t)dma_data_tx1,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART1_TX);//初始化DMA1，通道1，外设地址USART1->DR，外设传输宽度8位，内存地址dma_data_tx1，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART1_Tx
        Dma_Init(DMA2,DMA2_CHANNEL1,(uint32_t)&USART1->dt,P8,(uint32_t)dma_data_rx1,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART1_RX);//初始化DMA2，通道1，外设地址USART1->DR，外设传输宽度8位，内存地址dma_data_rx1，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART1_Rx
        usart_dma_transmitter_enable(USART1, TRUE);//使能DMA发送
        usart_dma_receiver_enable(USART1, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL1传输完成中断
        Nvic_Init(DMA1_Channel1_IRQn,1,0);//配置中断
        usart_interrupt_enable(USART1, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL1,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p1=0;
    }
    if(USARTx==2)
    {
        RX2=0;
        Uartx_Init(usart2,bound);
        Dma_Init(DMA1,DMA1_CHANNEL2,(uint32_t)&USART2->dt,P8,(uint32_t)dma_data_tx2,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART2_TX);//初始化DMA1，通道2，外设地址USART2->DR，外设传输宽度8位，内存地址dma_data_tx2，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART2_Tx
        Dma_Init(DMA2,DMA2_CHANNEL2,(uint32_t)&USART2->dt,P8,(uint32_t)dma_data_rx2,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART2_RX);//初始化DMA2，通道2，外设地址USART2->DR，外设传输宽度8位，内存地址dma_data_rx2，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART2_Rx
        usart_dma_transmitter_enable(USART2, TRUE);//使能DMA发送
        usart_dma_receiver_enable(USART2, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL2, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL2传输完成中断
        Nvic_Init(DMA1_Channel2_IRQn,1,0);//配置中断
        usart_interrupt_enable(USART2, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL2,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p2=0;
    }
    if(USARTx==3)
    {
        RX3=0;
        Uartx_Init(usart3,bound);
        Dma_Init(DMA1,DMA1_CHANNEL3,(uint32_t)&USART3->dt,P8,(uint32_t)dma_data_tx3,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART3_TX);//初始化DMA1，通道3，外设地址USART3->DR，外设传输宽度8位，内存地址dma_data_tx3，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART3_Tx
        Dma_Init(DMA2,DMA2_CHANNEL3,(uint32_t)&USART3->dt,P8,(uint32_t)dma_data_rx3,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART3_RX);//初始化DMA2，通道3，外设地址USART3->DR，外设传输宽度8位，内存地址dma_data_rx3，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART3_Rx
        usart_dma_transmitter_enable(USART3, TRUE);//使能DMA发送
        usart_dma_receiver_enable(USART3, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL3, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL3传输完成中断
        Nvic_Init(DMA1_Channel3_IRQn,1,0);//配置中断
        usart_interrupt_enable(USART3, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL3,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p3=0;
    }
    if(USARTx==4)
    {
        RX4=0;
        Uartx_Init(usart4,bound);
        Dma_Init(DMA1,DMA1_CHANNEL4,(uint32_t)&UART4->dt,P8,(uint32_t)dma_data_tx4,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART4_TX);//初始化DMA1，通道4，外设地址USART4->DR，外设传输宽度8位，内存地址dma_data_tx4，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART4_Tx
        Dma_Init(DMA2,DMA2_CHANNEL4,(uint32_t)&UART4->dt,P8,(uint32_t)dma_data_rx4,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART4_RX);//初始化DMA2，通道4，外设地址USART4->DR，外设传输宽度8位，内存地址dma_data_rx4，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART4_Rx
        usart_dma_transmitter_enable(UART4, TRUE);//使能DMA发送
        usart_dma_receiver_enable(UART4, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL4, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL4传输完成中断
        Nvic_Init(DMA1_Channel4_IRQn,1,0);//配置中断
        usart_interrupt_enable(UART4, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL4,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p4=0;
    }
    if(USARTx==5)
    {
        RX5=0;
        Uartx_Init(usart5,bound);
        Dma_Init(DMA1,DMA1_CHANNEL5,(uint32_t)&UART5->dt,P8,(uint32_t)dma_data_tx5,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART5_TX);//初始化DMA1，通道5，外设地址USART5->DR，外设传输宽度8位，内存地址dma_data_tx5，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART5_Tx
        Dma_Init(DMA2,DMA2_CHANNEL5,(uint32_t)&UART5->dt,P8,(uint32_t)dma_data_rx5,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART5_RX);//初始化DMA2，通道5，外设地址USART5->DR，外设传输宽度8位，内存地址dma_data_rx5，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART5_Rx
        usart_dma_transmitter_enable(UART5, TRUE);//使能DMA发送
        usart_dma_receiver_enable(UART5, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL5, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL4传输完成中断
        Nvic_Init(DMA1_Channel5_IRQn,1,0);//配置中断
        usart_interrupt_enable(UART5, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL5,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p5=0;
    }
    if(USARTx==6)
    {
        RX6=0;
        Uartx_Init(usart6,bound);
        Dma_Init(DMA1,DMA1_CHANNEL6,(uint32_t)&USART6->dt,P8,(uint32_t)dma_data_tx6,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART6_TX);//初始化DMA1，通道6，外设地址USART6->DR，外设传输宽度8位，内存地址dma_data_tx6，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART6_Tx
        Dma_Init(DMA2,DMA2_CHANNEL6,(uint32_t)&USART6->dt,P8,(uint32_t)dma_data_rx6,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART6_RX);//初始化DMA2，通道6，外设地址USART6->DR，外设传输宽度8位，内存地址dma_data_rx6，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART6_Rx
        usart_dma_transmitter_enable(USART6, TRUE);//使能DMA发送
        usart_dma_receiver_enable(USART6, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL6, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL4传输完成中断
        Nvic_Init(DMA1_Channel6_IRQn,1,0);//配置中断
        usart_interrupt_enable(USART6, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL6,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p6=0;
    }
    if(USARTx==7)
    {
        RX7=0;
        Uartx_Init(usart7,bound);
        Dma_Init(DMA1,DMA1_CHANNEL7,(uint32_t)&UART7->dt,P8,(uint32_t)dma_data_tx7,M8,DMA_BUF_SIZE,Tx,FALSE,medium,DMA_FLEXIBLE_UART7_TX);//初始化DMA1，通道7，外设地址USART7->DR，外设传输宽度8位，内存地址dma_data_tx7，内存传输宽度8位，方向由内存到外设，正常模式，优先级中，连接USART7_Tx
        Dma_Init(DMA2,DMA2_CHANNEL7,(uint32_t)&UART7->dt,P8,(uint32_t)dma_data_rx7,M8,DMA_BUF_SIZE,Rx,TRUE,medium,DMA_FLEXIBLE_UART7_RX);//初始化DMA2，通道7，外设地址USART7->DR，外设传输宽度8位，内存地址dma_data_rx7，内存传输宽度8位，方向由外设到内存，正常模式，优先级中，连接USART7_Rx
        usart_dma_transmitter_enable(UART7, TRUE);//使能DMA发送
        usart_dma_receiver_enable(UART7, TRUE);//使能DMA接收
        dma_interrupt_enable(DMA1_CHANNEL7, DMA_FDT_INT, TRUE);//开启DMA1_CHANNEL7传输完成中断
        Nvic_Init(DMA1_Channel7_IRQn,1,0);//配置中断
        usart_interrupt_enable(UART7, USART_RDBF_INT, FALSE);//关闭串口接收中断
        DMA_NO(DMA2_CHANNEL7,DMA_BUF_SIZE);//开启DMA1_CHANNEL2传输
        dma_data_rx_p7=0;
    }
}

u8 DMA_TX1=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 DMA_TX2=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 DMA_TX3=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 DMA_TX4=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 DMA_TX5=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 DMA_TX6=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 DMA_TX7=1;//DMA发送空闲标志，1空闲，0数据传输中
u8 dma_tx(u8 USARTx,char*dma_data,u32 len)//发送数据
{
    int i;
    u32 len1;//发送数据的长度
    u32 lastlen1;//上次发送数据的长度
    u32 len2;//发送数据的长度
    u32 lastlen2;//上次发送数据的长度
    u32 len3;//发送数据的长度
    u32 lastlen3;//上次发送数据的长度
    u32 len4;//发送数据的长度
    u32 lastlen4;//上次发送数据的长度
    u32 len5;//发送数据的长度
    u32 lastlen5;//上次发送数据的长度
    u32 len6;//发送数据的长度
    u32 lastlen6;//上次发送数据的长度
    u32 len7;//发送数据的长度
    u32 lastlen7;//上次发送数据的长度
    if(USARTx==1)
    {
        lastlen1=strlen(dma_data_tx1);//得到上次发送数据的长度
        if(DMA_TX1==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen1;i++)dma_data_tx1[i]='\0';//清除上次数据
            if(len<=0)
            {
                len1=strlen(dma_data);//得到发送的数据长度
                if(len1==0)len1++;
            }
            else len1=len;
            for(i=0;i<len1;i++) dma_data_tx1[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL1,len1);//启动一次DMA传输
            DMA_TX1=0;//标记为数据传输中
            return 1;//成功发送一次数据，返回1
        }
        else return 0;//没发送数据，返回0
    }
    else if(USARTx==2)
    {
        lastlen2=strlen(dma_data_tx2);//得到上次发送数据的长度
        if(DMA_TX2==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen2;i++)dma_data_tx2[i]='\0';//清除上次数据
            if(len<=0)
            {
                len2=strlen(dma_data);//得到发送的数据长度
                if(len2==0)len2++;
            }
            else len2=len;
            for(i=0;i<len2;i++) dma_data_tx2[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL2,len2);//启动一次DMA传输
            DMA_TX2=0;//标记为数据传输中
            return 2;//成功发送一次数据，返回2
        }
        else return 0;//没发送数据，返回0
    }
    else if(USARTx==3)
    {
        lastlen3=strlen(dma_data_tx3);//得到上次发送数据的长度
        if(DMA_TX3==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen3;i++)dma_data_tx3[i]='\0';//清除上次数据
            if(len<=0)
            {
                len3=strlen(dma_data);//得到发送的数据长度
                if(len3==0)len3++;
            }
            else len3=len;
            for(i=0;i<len3;i++) dma_data_tx3[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL3,len3);//启动一次DMA传输
            DMA_TX3=0;//标记为数据传输中
            return 3;//成功发送一次数据，返回3
        }
        else return 0;//没发送数据，返回0
    }
    else if(USARTx==4)
    {
        lastlen4=strlen(dma_data_tx4);//得到上次发送数据的长度
        if(DMA_TX4==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen4;i++)dma_data_tx4[i]='\0';//清除上次数据
            if(len<=0)
            {
                len4=strlen(dma_data);//得到发送的数据长度
                if(len4==0)len4++;
            }
            else len4=len;
            for(i=0;i<len4;i++) dma_data_tx4[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL4,len4);//启动一次DMA传输
            DMA_TX4=0;//标记为数据传输中
            return 4;//成功发送一次数据，返回4
        }
        else return 0;//没发送数据，返回0
    }
    else if(USARTx==5)
    {
        lastlen5=strlen(dma_data_tx5);//得到上次发送数据的长度
        if(DMA_TX5==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen5;i++)dma_data_tx5[i]='\0';//清除上次数据
            if(len<=0)
            {
                len5=strlen(dma_data);//得到发送的数据长度
                if(len5==0)len5++;
            }
            else len5=len;
            for(i=0;i<len5;i++) dma_data_tx5[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL5,len5);//启动一次DMA传输
            DMA_TX5=0;//标记为数据传输中
            return 5;//成功发送一次数据，返回5
        }
        else return 0;//没发送数据，返回0
    }
    else if(USARTx==6)
    {
        lastlen6=strlen(dma_data_tx6);//得到上次发送数据的长度
        if(DMA_TX6==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen6;i++)dma_data_tx6[i]='\0';//清除上次数据
            if(len<=0)
            {
                len6=strlen(dma_data);//得到发送的数据长度
                if(len6==0)len6++;
            }
            else len6=len;
            for(i=0;i<len6;i++) dma_data_tx6[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL6,len6);//启动一次DMA传输
            DMA_TX6=0;//标记为数据传输中
            return 6;//成功发送一次数据，返回6
        }
        else return 0;//没发送数据，返回0
    }
    else if(USARTx==7)
    {
        lastlen7=strlen(dma_data_tx7);//得到上次发送数据的长度
        if(DMA_TX7==1)//DMA发送通道处于空闲
        {
            for(i=0;i<lastlen7;i++)dma_data_tx7[i]='\0';//清除上次数据
            if(len<=0)
            {
                len7=strlen(dma_data);//得到发送的数据长度
                if(len7==0)len7++;
            }
            else len7=len;
            for(i=0;i<len7;i++) dma_data_tx7[i]=dma_data[i];//将要发送的数据复制到发送缓冲区
            DMA_NO(DMA1_CHANNEL7,len7);//启动一次DMA传输
            DMA_TX7=0;//标记为数据传输中
            return 7;//成功发送一次数据，返回6
        }
        else return 0;//没发送数据，返回0
    }
    else return 0;//没发送数据，返回0
}


u8 dma_rx(u8 USARTx,char*dma_data,u32 len)//接收数据 ,可以将接收对应串口的数据赋值到dma_data。可以在main函数直接调用得到数据
{
    int i;
    u32 Len;//接收数据长度
    u32 data_p;//数据起始位置
    if(USARTx==1)
    {
        if(DMA_RX1)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p1;
            if(len<=0)Len=dma_data_rx_len1;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx1[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            DMA_RX1=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else if(USARTx==2)
    {
        if(RX2)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p2;
            if(len<=0)Len=dma_data_rx_len2;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx2[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            RX2=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else if(USARTx==3)
    {
        if(RX3)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p3;
            if(len<=0)Len=dma_data_rx_len3;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx3[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            RX3=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else if(USARTx==4)
    {
        if(RX4)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p4;
            if(len<=0)Len=dma_data_rx_len4;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx4[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            RX4=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else if(USARTx==5)
    {
        if(RX5)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p5;
            if(len<=0)Len=dma_data_rx_len5;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx5[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            RX5=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else if(USARTx==6)
    {
        if(RX6)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p6;
            if(len<=0)Len=dma_data_rx_len6;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx6[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            RX6=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else if(USARTx==7)
    {
        if(RX7)//DMA接收通道处于空闲
        {
            data_p=dma_data_rx_p7;
            if(len<=0)Len=dma_data_rx_len7;//当传入的len<=0，len=接收到的数据长度
            else Len=len;
            for(i=0;i<Len;i++)*(dma_data+i)=dma_data_rx7[((i+data_p)<DMA_BUF_SIZE ? i+data_p : i+data_p-DMA_BUF_SIZE)];//将接收的数据复制到内存
            RX7=0;//标记为数据传输中
            return 1;//成功启动接收数据，返回1
        }
        else return 0;//反之返回0
    }
    else return 0;
}



u8 Dma_printf(char* str,...)
{
    va_list p;
    u8 len;
    va_start(p,str);
    len=vsnprintf(dma_data_tx1,sizeof(dma_data_tx1),str,p);
    va_end(p);
    if(DMA_TX1==1)
    {
        DMA_NO(DMA1_CHANNEL1,len);//启动一次DMA传输
        DMA_TX1=0;
        return len;//发送成功，返回发送数据长度
    }
    else return 0;//未发送成功，返回0
}



u8 dma_printf(char* str,...)
{
    va_list p;
    u8 len;
    va_start(p,str);
    len=vsnprintf(dma_data_tx7,sizeof(dma_data_tx7),str,p);
    va_end(p);
    if(DMA_TX7==1)
    {
        DMA_NO(DMA1_CHANNEL7,len);//启动一次DMA传输
        DMA_TX7=0;
        return len;//发送成功，返回发送数据长度
    }
    else return 0;//未发送成功，返回0
}



volatile unsigned long*dma_de_gpio[7]={NULL,NULL,NULL,NULL,NULL,NULL,NULL};//DMA_RS485发送使能IO口映射

//DMA_RS485初始化
//USARTx：串口号
//bound：波特率
//DE_GPIO：发送使能IO口GPIO
//DE_PIN：发送使能IO口PIN
void dma_485_init(u8 USARTx,u32 bound,u8 DE_GPIO,u8 DE_PIN)
{
    dma_uart_init(USARTx,bound);
    Gpio_Init(DE_GPIO,DE_PIN,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,100);//初始化发送使能口
    switch(DE_GPIO)
    {
        case 'A':dma_de_gpio[USARTx-1]=&PAout(DE_PIN);break;
        case 'B':dma_de_gpio[USARTx-1]=&PBout(DE_PIN);break;
        case 'C':dma_de_gpio[USARTx-1]=&PCout(DE_PIN);break;
        case 'D':dma_de_gpio[USARTx-1]=&PDout(DE_PIN);break;
        case 'E':dma_de_gpio[USARTx-1]=&PEout(DE_PIN);break;
    }
    *dma_de_gpio[USARTx-1]=0;//启动接收
}

//DMA_RS485发送
//USARTx：串口号
//data：发送数据
//len：数据长度
u8 dma_485_tx(u8 USARTx,char*data,u32 len)
{
    u8 sta;
    *dma_de_gpio[USARTx-1]=1;//开启RS485发送
    sta=dma_tx(USARTx,data,len);
    if(!sta)//发送失败
    {
        *dma_de_gpio[USARTx-1]=0;//关闭RS485发送
        return 0;//返回0
    }
    return sta;//成功发送数据返回串口号
}

//DMA_RS485发接收
//USARTx：串口号
//data：发送数据
//len：数据长度
//接收到数据：返回串口号，反之返回0
u8 dma_485_rx(u8 USARTx,char*buf,u32 len)
{
    return dma_rx(USARTx,buf,len);
}


uint16_t RS485_Delay;

void DMA1_Channel1_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT1_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT1_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL1, FALSE);
        DMA_TX1=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[0]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[0]!=NULL)*dma_de_gpio[0]=0;
    }
}



void DMA1_Channel2_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT2_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT2_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL2, FALSE);
        DMA_TX2=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[1]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[1]!=NULL)*dma_de_gpio[1]=0;
    }
}



void DMA1_Channel3_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT3_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT3_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL3, FALSE);
        DMA_TX3=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[2]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[2]!=NULL)*dma_de_gpio[2]=0;
    }
}


void DMA1_Channel4_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT4_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT4_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL4, FALSE);
        DMA_TX4=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[3]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[3]!=NULL)*dma_de_gpio[3]=0;
    }
}


void DMA1_Channel5_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT5_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT5_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL5, FALSE);
        DMA_TX5=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[4]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[4]!=NULL)*dma_de_gpio[4]=0;
    }
}

void DMA1_Channel6_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT6_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT6_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL6, FALSE);
        DMA_TX6=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[5]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[5]!=NULL)*dma_de_gpio[5]=0;
    }
}


void DMA1_Channel7_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT7_FLAG))//判断传输是否完成
    {
        dma_flag_clear(DMA1_FDT7_FLAG);//清除通道传输完成标志
        dma_channel_enable(DMA1_CHANNEL7, FALSE);
        DMA_TX7=1;//标记为空闲
        RS485_Delay=0;
        while(RS485_Delay<0x5F&&dma_de_gpio[6]!=NULL)RS485_Delay++;//等待一段时间，确保485数据完全接收
        if(dma_de_gpio[6]!=NULL)*dma_de_gpio[6]=0;
    }
}


