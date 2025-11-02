#ifndef __OID_H
#define __OID_H	 
#include "sys.h"

#define PRECISION 4096 //单圈精度
#define ROUND_MAX 16      //最大圈数




typedef struct
{
    u8 id;//id
    u8 cmd;//指令码
    u8 err;//错误码
    uint16_t val;//编码器数值
    uint8_t round;//圈数
    double angle;//角度
    
    int last_val;//上一个编码器数值
    int add_round;//累加圈数
    double add_angle;//累加角度
}OID_RX_DATA;//欧艾迪编码器数据接收结构体




//欧艾迪编码器指令
#define OID_CMD_READ              ((long long)0x04)|((long long)0x01<<8)|((long long)0x00<<16)//读取一次编码器数值
#define OID_CMD_SET_ID(x)         ((long long)0x04)|((long long)0x02<<8)|((long long)x<<16)   //数组编码器ID（0x00~0xFF）
#define OID_CMD_SET_BOUND_500K    ((long long)0x04)|((long long)0x03<<8)|((long long)0x00<<16)//设置CAN波特率为500K
#define OID_CMD_SET_BOUND_1M      ((long long)0x04)|((long long)0x03<<8)|((long long)0x01<<16)//设置CAN波特率为1M
#define OID_CMD_SET_BOUND_250K    ((long long)0x04)|((long long)0x03<<8)|((long long)0x02<<16)//设置CAN波特率为250K
#define OID_CMD_SET_BOUND_125K    ((long long)0x04)|((long long)0x03<<8)|((long long)0x03<<16)//设置CAN波特率为125K
#define OID_CMD_SET_BOUND_100K    ((long long)0x04)|((long long)0x03<<8)|((long long)0x04<<16)//设置CAN波特率为100K
#define OID_CMD_SET_MODE_INQUIRY  ((long long)0x04)|((long long)0x04<<8)|((long long)0x00<<16)//设置查询模式
#define OID_CMD_SET_MODE_AUTO     ((long long)0x04)|((long long)0x04<<8)|((long long)0xAA<<16)//设置自动返回模式
#define OID_CMD_SET_AUTO_TIME(x)  ((long long)0x05)|((long long)0x05<<8)|((long long)x<<16)   //设置自动返回时间（0~0xFFFF）
#define OID_CMD_SET_ORIGIN        ((long long)0x04)|((long long)0x06<<8)|((long long)0x00<<16)//设置位置零点
#define OID_CMD_SET_CLOCKWISE     ((long long)0x04)|((long long)0x07<<8)|((long long)0x00<<16)//设置顺时针方向为正
#define OID_CMD_SET_ANTICLOCKWISE ((long long)0x04)|((long long)0x07<<8)|((long long)0x01<<16)//设置逆时针方向为正
#define OID_CMD_SET_MIDDLE        ((long long)0x04)|((long long)0x0C<<8)|((long long)0x01<<16)//设置位置中点
#define OID_CMD_SET_POS(x)        ((long long)0x07)|((long long)0x0D<<8)|((long long)x<<16)   //设置位置（0x00~0xFFFFFFFF）
#define OID_CMD_SET_5ROUND        ((long long)0x04)|((long long)0x0F<<8)|((long long)0x01<<16)//设置位置5圈
    

void OID_Init(u8 CANx,OID_RX_DATA*OID_Rx_Buf,u8 OID_Rx_Buf_Len);//编码器初始化
void OID_Cmd(uint32_t id,long long OID_CMD);//编码器发送指令
void OID_Rx(uint32_t id,u8*data);//编码器接收数据




#endif
