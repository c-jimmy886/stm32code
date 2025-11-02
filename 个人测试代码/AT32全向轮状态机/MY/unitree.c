#include<unitree.h>
#include<init.h>
#include<sys.h>
#include<dma_usart.h>
#include<stdio.h>


//CRC校验数据
uint16_t const crc_ccitt_table[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


//宇树接收结构体指针及长度
u8 UNITREE_USART2_RX_DATA_LEN=0;
UNITREE_DATA*UNITREE_USART2_RX_DATA=NULL;

u8 UNITREE_USART3_RX_DATA_LEN=0;
UNITREE_DATA*UNITREE_USART3_RX_DATA=NULL;

u8 UNITREE_USART4_RX_DATA_LEN=0;
UNITREE_DATA*UNITREE_USART4_RX_DATA=NULL;

u8 UNITREE_USART5_RX_DATA_LEN=0;
UNITREE_DATA*UNITREE_USART5_RX_DATA=NULL;

u8 UNITREE_USART6_RX_DATA_LEN=0;
UNITREE_DATA*UNITREE_USART6_RX_DATA=NULL;

u8 UNITREE_USART7_RX_DATA_LEN=0;
UNITREE_DATA*UNITREE_USART7_RX_DATA=NULL;



#define UNITREE_BOUND 4000000    //宇树串口波特率


//数据限幅
float limit(float val,float min,float max)
{
    if(val>max)return max;
    else if(val<min)return min;
    else return val;
}

//CRC校验
static uint16_t crc_ccitt_byte(uint16_t crc, const uint8_t c)
{
    return (crc >> 8) ^ crc_ccitt_table[(crc ^ c) & 0xff];
}
//CRC校验
static uint16_t crc_ccitt(uint16_t crc, uint8_t const *buffer, int len)
{
    uint16_t tmp = crc;
    while (len--)
    {
        tmp = crc_ccitt_byte(tmp, *buffer);
            buffer ++;
    }
    return tmp;
}


//宇树接收函数
void Unitree_Rx(UNITREE_DATA*Unitree_Data,u8 Unitree_Data_Len)
{
    u8 i;
    u8 buf[18];
    
    if(Unitree_Data->dma_enable)dma_485_rx(Unitree_Data->usartx,(char*)buf,16);//使用DMA_RS485接收
    else USARTx_Rx(Unitree_Data->usartx,(char*)buf,16);//普通RS485接收
    
    if(buf[0]==0xFD&&buf[1]==0xEE)//接收数据包头正确
    {
        for(i=0;i<Unitree_Data_Len;i++)//从传入的数据中寻找正确的电机id
        {
            if((buf[2]&0x0F)==(Unitree_Data+i)->id)//找到正确的id
            {
                //将接收到的数据解析并存入对应结构体中
                (Unitree_Data+i)->mode=(buf[2]>>4)&0xF;
                (Unitree_Data+i)->t=(float)((int16_t)buf[3]|((int16_t)buf[4]<<8))/256.0f;
                (Unitree_Data+i)->vel=(float)((int16_t)buf[5]|((int16_t)buf[6]<<8))/256.0f*2.0f*3.1415926f;
                (Unitree_Data+i)->pos=(float)((int32_t)buf[7]|((int32_t)buf[8]<<8)|((int32_t)buf[9]<<16)|((int32_t)buf[10]<<24))/32768.0f*2.0f*3.1415926f;
                (Unitree_Data+i)->temp=(int8_t)buf[11];
                (Unitree_Data+i)->error=buf[12]&0x07;
                (Unitree_Data+i)->force=(uint16_t)((((uint16_t)buf[12]>>3)&0x1F)|((uint16_t)buf[13]<<5));
                (Unitree_Data+i)->crc=(uint16_t)((uint16_t)buf[14]|((uint16_t)buf[15]<<8));
                break;
            }
        }
    }
}


//宇树接收函数装载函数
void Unitree_USART2_Rx()
{
    Unitree_Rx(UNITREE_USART2_RX_DATA,UNITREE_USART2_RX_DATA_LEN);
}
void Unitree_USART3_Rx()
{
    Unitree_Rx(UNITREE_USART3_RX_DATA,UNITREE_USART3_RX_DATA_LEN);
}
void Unitree_USART4_Rx()
{
    Unitree_Rx(UNITREE_USART4_RX_DATA,UNITREE_USART4_RX_DATA_LEN);
}
void Unitree_USART5_Rx()
{
    Unitree_Rx(UNITREE_USART5_RX_DATA,UNITREE_USART5_RX_DATA_LEN);
}
void Unitree_USART6_Rx()
{
    Unitree_Rx(UNITREE_USART6_RX_DATA,UNITREE_USART6_RX_DATA_LEN);
}   
void Unitree_USART7_Rx()
{
    Unitree_Rx(UNITREE_USART7_RX_DATA,UNITREE_USART7_RX_DATA_LEN);
}



//宇树电机初始化
//Unitree_Data：宇树电机数据结构体输入
//Unitree_Data_Len：宇树电机数据结构体长度
//USARTx：串口号
//Dma_Enable：是否使用dma
//RS485_EN_GPIO：RS485发送使能GPIO
//RS485_EN_PIN：RS485发送使能PIN
void Unitree_Init(UNITREE_DATA*Unitree_Data,u8 Unitree_Data_Len,u8 USARTx,u8 Dma_Enable,u8 RS485_EN_GPIO,u8 RS485_EN_PIN)
{
    u8 i;
    if(Dma_Enable)dma_485_init(USARTx,UNITREE_BOUND,RS485_EN_GPIO,RS485_EN_PIN);//如果使用DMA，则初始化DMA_RS485
    else RS485_Init(USARTx,UNITREE_BOUND,RS485_EN_GPIO,RS485_EN_PIN);//反之直接初始化RS485
    
    for (i=0;i<Unitree_Data_Len;i++)//将初始化数据传入结构体
    {
        Unitree_Data->usartx=USARTx;//串口号
        Unitree_Data->dma_enable=Dma_Enable;//是否使用dma
    }
    
    switch (USARTx)//根据初始化的串口把相关的接收数据及函数装载到相应串口空闲中断服务函数
    {
        case usart2:USART2_IDLE_IRQ=&Unitree_USART2_Rx;UNITREE_USART2_RX_DATA=Unitree_Data;UNITREE_USART2_RX_DATA_LEN=Unitree_Data_Len;break;
        case usart3:USART3_IDLE_IRQ=&Unitree_USART3_Rx;UNITREE_USART3_RX_DATA=Unitree_Data;UNITREE_USART3_RX_DATA_LEN=Unitree_Data_Len;break;
        case usart4:USART4_IDLE_IRQ=&Unitree_USART4_Rx;UNITREE_USART4_RX_DATA=Unitree_Data;UNITREE_USART4_RX_DATA_LEN=Unitree_Data_Len;break;
        case usart5:USART5_IDLE_IRQ=&Unitree_USART5_Rx;UNITREE_USART5_RX_DATA=Unitree_Data;UNITREE_USART5_RX_DATA_LEN=Unitree_Data_Len;break;
        case usart6:USART6_IDLE_IRQ=&Unitree_USART6_Rx;UNITREE_USART6_RX_DATA=Unitree_Data;UNITREE_USART6_RX_DATA_LEN=Unitree_Data_Len;break;
        case usart7:USART7_IDLE_IRQ=&Unitree_USART7_Rx;UNITREE_USART7_RX_DATA=Unitree_Data;UNITREE_USART7_RX_DATA_LEN=Unitree_Data_Len;break;
    }
}


//宇树电机数据发送
//Unitree_Data：宇树电机数据结构体
//宇树电机模式：0:锁定，1:FOC闭环，2:编码器校准(等待5s期间不可以给电机发送任何数据包)
void Unitree_Tx(UNITREE_DATA Unitree_Data,u8 mode,float t,float vel,float pos,float kp,float kd)
{
    u8 data[18]={0xFE,0xEE};//发送数据数组（包头）
    int16_t T=limit(t,-127.99,127.99)*256;//电机力矩[-127.99,127.99]（N.m）
    int16_t Vel=limit(vel,-804.0,804.0)/(2*3.1415926f)*256;//电机速度[-804.0,804.0]（rad/s）
    int32_t Pos=limit(pos,-411774,411774)/(2*3.1415926f)*32768;//电机位置[-411774,411774]（rad）
    int16_t Kp=limit(kp,0,25.599)*1280;//电机刚度系数[0,25.599]
    int16_t Kd=limit(kd,0,25.599)*1280;//电机阻尼系数[0,25.599]
    uint16_t Crc;//CRC校验结果
    
    //将所有数据放入发送数组
    data[2]=Unitree_Data.id|(mode<<4);
    data[3]=T;
    data[4]=T>>8;
    data[5]=Vel;
    data[6]=Vel>>8;
    data[7]=Pos;
    data[8]=Pos>>8;
    data[9]=Pos>>16;
    data[10]=Pos>>24;
    data[11]=Kp;
    data[12]=Kp>>8;
    data[13]=Kd;
    data[14]=Kd>>8;
    
    Crc=crc_ccitt(0,data,15);//计算CRC值
    data[15]=Crc;
    data[16]=Crc>>8;
    
    if(Unitree_Data.dma_enable)dma_485_tx(Unitree_Data.usartx,(char*)data,17);//使用DMA_RS485发送
    else RS485_Tx(Unitree_Data.usartx,(char*)data,17);//普通RS485发送
}

