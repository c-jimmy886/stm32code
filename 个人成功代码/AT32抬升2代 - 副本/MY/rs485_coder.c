#include<init.h>
#include<rs485_coder.h>

//RS485编码器初始化
//USARTx：串口号
//RS485_DE_GPIO：发送使能IO口GPIO
//RS485_DE_PIN：发送使能IO口PIN
void RS485_coder_init(u8 USARTx,u8 RS485_DE_GPIO,u8 RS485_DE_PIN)
{
    RS485_Init(USARTx,19200,RS485_DE_GPIO,RS485_DE_PIN);//初始化RS485（波特率19200）
}

//读取RS485编码器
//USARTx：串口号
float RS485_coder_angle(u8 USARTx)
{
    u8 buf[10];//数据缓存
    static float angle=0;
    if(RS485_Rx(USARTx,(char*)buf,10))
    {
        if(buf[0]==0xab&&buf[1]==0xcd&&buf[9]==0x3d)angle=(float)(buf[3]*256+buf[4])*360/16384;
    }
    return angle;//返回当前角度
}
