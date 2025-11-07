#include<init.h>
#include<oid.h>


u8 OID_CANx;//编码器CAN号
u8 OID_Len;//编码器数量
OID_RX_DATA*OID_Rx_Data;//编码器数据接收指针;



//编码器初始化
//CANx：CAN号
//OID_Rx_Buf：数据接收缓存
//OID_Rx_Buf_Len：编码器数量
void OID_Init(u8 CANx,OID_RX_DATA*OID_Rx_Buf,u8 OID_Rx_Buf_Len)
{
    u8 i;
    if(CANx==1)
    {
        Can_Init(CANx,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS2_2TQ,15,0,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0x00000000,0x00000000,CAN_FILTER_FIFO0);
        //CAN初始化：CAN1，普通模式，波特率120/（1+5+2）/15=1M,，过滤器0，过滤器宽32bit，使用标准id，ID:0x00000000，掩码:0x00000000，过滤器关联邮箱FIFO0
        CAN1_RX0_IRQ=&OID_Rx;//将RM电机接收函数通过函数指针装载到CANx中断服务函数
    }
    if(CANx==2)
    {
        Can_Init(CANx,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS2_2TQ,15,13,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0x00000000,0x00000000,CAN_FILTER_FIFO1);
        //CAN初始化：CAN2，普通模式，波特率120/（1+5+2）/15=1M,，过滤器0，过滤器宽32bit，使用标准id，ID:0x00000000，掩码:0x00000000，过滤器关联邮箱FIFO1
        CAN2_RX1_IRQ=&OID_Rx;
    }
    OID_Rx_Data=OID_Rx_Buf;//得到数据缓存区
    OID_Len=OID_Rx_Buf_Len;//得到数据缓存区长度
    OID_CANx=CANx;//得到CAN号
    
    for(i=0;i<OID_Rx_Buf_Len;i++)(OID_Rx_Buf+i)->last_val=-1;//用于多圈累加的初始值判断
}


//编码器指令发送
//id：编码器ID
//OID_CMD:编码器指令
void OID_Cmd(uint32_t id,long long OID_CMD)
{
    u8 i;
    u8 data[8]={0};
    u8 len=OID_CMD&0xFF;
    
    data[0]=len;
    data[1]=id;
    data[2]=(OID_CMD>>8)&0xFF;
    for(i=3;i<len;i++)data[i]=(OID_CMD>>(8*(len-i+1)))&0xFF;
    
    Can_Tx(OID_CANx,CAN_ID_STANDARD,id,data,len,0);
}


//编码器数据接收
void OID_Rx(uint32_t id,u8*data)
{
    u8 i;
    for(i=0;i<OID_Len;i++)
    {
        if(id==(OID_Rx_Data+i)->id)//找到符合的ID
        {
            (OID_Rx_Data+i)->cmd=data[2];//得到指令码
            if((OID_Rx_Data+i)->cmd==0x01)//接收到的数据为编码器数据
            {
                (OID_Rx_Data+i)->val=data[3]|(data[4]<<8)|(data[5]<<16)|(data[6]<<24);//得到编码器值
                
                (OID_Rx_Data+i)->round=((OID_Rx_Data+i)->val)/PRECISION;//计算圈数
                (OID_Rx_Data+i)->angle=(double)((OID_Rx_Data+i)->val)/PRECISION*360.0f;//转化为角度
                
                //多圈角度累加
                if((OID_Rx_Data+i)->last_val-(OID_Rx_Data+i)->val>(PRECISION*ROUND_MAX)/2&&(OID_Rx_Data+i)->last_val>=0)(OID_Rx_Data+i)->add_round+=ROUND_MAX;
                if((OID_Rx_Data+i)->val-(OID_Rx_Data+i)->last_val>(PRECISION*ROUND_MAX)/2&&(OID_Rx_Data+i)->last_val>=0)(OID_Rx_Data+i)->add_round-=ROUND_MAX;
                
                (OID_Rx_Data+i)->add_round-=(OID_Rx_Data+i)->last_val/PRECISION;
                (OID_Rx_Data+i)->add_angle=(OID_Rx_Data+i)->add_round*360.0f+(OID_Rx_Data+i)->angle;
                (OID_Rx_Data+i)->add_round+=(OID_Rx_Data+i)->val/PRECISION;
                (OID_Rx_Data+i)->last_val=(OID_Rx_Data+i)->val;
            }
            else (OID_Rx_Data+i)->err=data[3];//其它情况获取错误码
            break;
        }
    }
}




