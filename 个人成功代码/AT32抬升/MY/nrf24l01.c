#include<init.h>
#include<delay.h>
#include<nrf24l01.h>



char NRF_CE_GPIO;//NRF24L01的CE口
u8   NRF_CE_PIN;
char NRF_CSN_GPIO;//NRF24L01的CSD口
u8   NRF_CSN_PIN;
char NRF_IRQ_GPIO;//NRF24L01的IRQ口
u8   NRF_IRQ_PIN;

u8 NRF_MODE;//NRF24L01传输模式
u8 NRF_RT_MODE;//NRF24L01当前处于发送还是接收
u8 DATA_LEN;



//向NRF24L01写入数据
//reg：寄存器地址
//data：要写入的数据
//len：写入的数据长度
u8 NRF24L01_Write(u8 reg,u8*data,u8 len)
{
    u8 status,u8_ctr;
    Pout(NRF_CSN_GPIO,NRF_CSN_PIN) = 0;//使能SPI传输
    status = Spi_RW(SPI1,reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)Spi_RW(SPI1,data[u8_ctr]); //写入数据
    Pout(NRF_CSN_GPIO,NRF_CSN_PIN) = 1;//关闭SPI传输
    return status;//返回读到的状态值
}


//向NRF24L01读取数据
//reg：寄存器地址
//buf：要读取的数据
//len：读取的数据长度
u8 NRF24L01_Read(u8 reg,u8 *buf,u8 len)
{
  u8 status,u8_ctr;	       
  Pout(NRF_CSN_GPIO,NRF_CSN_PIN) = 0;//使能SPI传输
  status=Spi_RW(SPI1,reg);//发送寄存器值(位置),并读取状态值   	   
  for(u8_ctr=0;u8_ctr<len;u8_ctr++)buf[u8_ctr]=Spi_RW(SPI1,0XFF);//读出数据
   Pout(NRF_CSN_GPIO,NRF_CSN_PIN) = 1;//关闭SPI传输
  return status;        //返回读到的状态值
}



//设置NRF发送或者接收
//NRF_RT_Mode：1:发送模式；0:接收模式
void NRF24L01_Mode(u8 NRF_RT_Mode)
{
    u8 mode=0x0f-NRF_RT_Mode;//得到NRF发送或者接收指令
    Pout(NRF_CE_GPIO,NRF_CE_PIN)=0;
    NRF24L01_Write(NRF_WRITE_REG+CONFIG,&mode,1);//配置基本工作模式的参数
    Pout(NRF_CE_GPIO,NRF_CE_PIN)=1;//CE为高
    NRF_RT_MODE=NRF_RT_Mode;//标记NRF当前处于发送还是接收
}



//检测NRF24L01是否存在
//check_num：检测次数
//返回值：1:成功；0:失败
u8 NRF24L01_Check(u8 check_num)
{
    u8 i;
    u8 addr[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
    u8 buf[5];
    while(check_num)
    {
        NRF24L01_Write(NRF_WRITE_REG+TX_ADDR,addr,5);//写入5个字节的地址
        NRF24L01_Read(TX_ADDR,buf,5); //读出写入的地址
        for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
        if(i==5)break;
        check_num--;
    }
    if(check_num==0)return 0;//未检测到24L01
    return 1;//检测到24L01
}



//NRF24L01初始化
//CE_GPIO，CE_PIN：NRF24L01的CE口
//CSN_GPIO，CSN_PIN：NRF24L01的CSN口
//IRQ_GPIO，IRQ_PIN：NRF24L01的IRQ口
//Tx_Add：发送地址
//Rx_Add：接收地址
//NRF_CHx：NRF通道
//rate：脉冲频率
//NRF_Mode：NRF传输模式
//检测NRF正常返回1，反之返回0
u8 NRF24L01_init(char CE_GPIO,u8 CE_PIN,char CSN_GPIO,u8 CSN_PIN,char IRQ_GPIO,u8 IRQ_PIN,u8*Tx_Add,u8*Rx_Add,u8 NRF_CHx,u8 Data_Len,u8 rate,u8 NRF_Mode)
{
    u8 SETUP_RETR_Data=0x1a;//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
    u8 RF_SETUP_Data=0x0f;//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    
    GPIO_init(CE_GPIO,CE_PIN,OUT_PP);//初始化相应IO口
    GPIO_init(CSN_GPIO,CSN_PIN,OUT_PP);
    GPIO_init(IRQ_GPIO,IRQ_PIN,IPU);
    
    //初始化SPI：双线双向全双工,主SPI,SPI发送接收8位帧结构,时钟的空闲状态为低电平,时钟的第一个跳变沿数据被采样,波特率预分频值为8,数据传输从MSB位开始,CRC值计算的多项式=7
    Spi_Init(SPI1,SPI_TRANSMIT_FULL_DUPLEX,SPI_MODE_MASTER,SPI_FRAME_8BIT,SPI_CLOCK_POLARITY_LOW,SPI_CLOCK_PHASE_1EDGE,SPI_MCLK_DIV_8,SPI_FIRST_BIT_MSB);
    
    
    NRF_CE_GPIO=CE_GPIO;//获取相应IO口号
    NRF_CE_PIN=CE_PIN;
    NRF_CSN_GPIO=CSN_GPIO;
    NRF_CSN_PIN=CSN_PIN;
    NRF_IRQ_GPIO=IRQ_GPIO;
    NRF_IRQ_PIN=IRQ_PIN;
    
    DATA_LEN=Data_Len;
    
    if(!NRF24L01_Check(10))return 0;//未检测到NRF24L01，返回0
    
    Pout(NRF_CE_GPIO,NRF_CE_PIN)=0;
    NRF24L01_Write(NRF_WRITE_REG+TX_ADDR,(u8*)Tx_Add,5);//写TX节点地址 
    NRF24L01_Write(NRF_WRITE_REG+0x0A+NRF_CHx/2,(u8*)Rx_Add,5); //设置RX节点地址
    NRF24L01_Write(NRF_WRITE_REG+EN_AA,&NRF_CHx,1);//使能通道的自动应答
    NRF24L01_Write(NRF_WRITE_REG+EN_RXADDR,&NRF_CHx,1);//使能通道的接收地址
    NRF24L01_Write(NRF_WRITE_REG+SETUP_RETR,&SETUP_RETR_Data,1);//设置自动重发间隔时间,最大自动重发次数
    NRF24L01_Write(NRF_WRITE_REG+RF_CH,&rate,1);//设置RF通道频率为40
    NRF24L01_Write(NRF_WRITE_REG+RF_SETUP,&RF_SETUP_Data,1);//设置TX发射参数
    NRF24L01_Write(NRF_WRITE_REG+0x11+NRF_CHx/2,&Data_Len,1);//选择通道的有效数据宽度
    Pout(NRF_CE_GPIO,NRF_CE_PIN)=1;
    if(NRF_Mode==NRF_Mode_Tx)NRF24L01_Mode(NRF_Tx);//发送模式（只能发送）
    if(NRF_Mode==NRF_Mode_Rx)NRF24L01_Mode(NRF_Rx);//接收模式（只能接收）
    if(NRF_Mode==NRF_Mode_RTx)NRF24L01_Mode(NRF_Rx);//接收发送模式（要发送数据是切换成发送模式，数据发送完后切换接收模式）
    
    NRF_MODE=NRF_Mode;//获取NRF传输模式
    
    return 1;//NRF24L01正常，返回1
}



//NRF发送
//data：发送的数据
//len：发送的数据长度
//发送成功返回1，反之返回0
u8 NRF24L01_Tx(u8*data)
{
    u8 sta;
    if(NRF_MODE==NRF_Mode_RTx)//接收发送模式下
    {
        NRF24L01_Mode(NRF_Tx);//进入发送，切换发送模式
        delay_us(10);
    }
    if(NRF_RT_MODE!=NRF_Tx)return 0;//非发送模式直接返回0
    Pout(NRF_CE_GPIO,NRF_CE_PIN)=0;//使能24L01
    NRF24L01_Write(WR_TX_PLOAD,data,DATA_LEN);//写数据到TX BUF  32个字节
    Pout(NRF_CE_GPIO,NRF_CE_PIN)=1;//启动发送
    while(Pin(NRF_IRQ_GPIO,NRF_IRQ_PIN)!=0);//等待发送完成
    NRF24L01_Read(STATUS,&sta,1);
    NRF24L01_Write(NRF_WRITE_REG+STATUS,&sta,1);//清除TX_DS或MAX_RT中断标志
    if(sta&MAX_TX)//达到最大重发次数
    {
        NRF24L01_Write(FLUSH_TX,(u8*)0xff,1);//清除TX FIFO寄存器 
        if(NRF_MODE==NRF_Mode_RTx)//接收发送模式下
        {
            delay_us(10);
            NRF24L01_Mode(NRF_Rx);//发送失败，切换接收模式
        }
        return 0; 
    }
    if(sta&TX_OK)//发送完成
    {
        if(NRF_MODE==NRF_Mode_RTx)//接收发送模式下
        {
            delay_us(10);
            NRF24L01_Mode(NRF_Rx);//发送成功，切换接收模式
        }
        return 1;
    }
    if(NRF_MODE==NRF_Mode_RTx)//接收发送模式下
    {
        delay_us(300);
        NRF24L01_Mode(NRF_Rx);//发送失败，切换接收模式
    }
    return 0;//其他原因发送失败
}



//NRF接收
//data：接收数据缓存
//len：要接收的数据长度
//接收到数据返回1，反之返回0
u8 NRF24L01_Rx(u8*buf)
{
    u8 sta;
    if(NRF_RT_MODE!=NRF_Rx)return 0;//非接收模式直接返回0
    NRF24L01_Read(STATUS,&sta,1);
    NRF24L01_Write(NRF_WRITE_REG+STATUS,&sta,1); //清除TX_DS或MAX_RT中断标志
    if(sta&RX_OK)//接收到数据
    {
        NRF24L01_Read(RD_RX_PLOAD,buf,DATA_LEN);//读取数据
        NRF24L01_Write(FLUSH_RX,(u8*)0xff,1);//清除RX FIFO寄存器 
        return 1;
    }
    return 0;//没收到任何数据
}

