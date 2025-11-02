#include<init.h>
#include<vesc.h>
#include<usart.h>



VESC_DATA_BUF*VESC_RX;
u8 vesc_sign=0;
u8 VESC_CANx;

void VESC_Init(u8 CANx,VESC_DATA_BUF*VESC,u8 VESC_Num)
{
    if(CANx==1)
    {
        //CAN初始化：CAN1，普通模式，波特率=120M/（1+5+2）/15=1M，过滤器0，32bit过滤器，扩展帧，id：0，掩码：0，邮箱FIFO0
        Can_Init(1,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS2_2TQ,15,0,CAN_FILTER_32BIT,CAN_Id_Mode_ExtId,0x0000,0x0000,CAN_FILTER_FIFO0);
        CAN1_RX0_IRQ=&vesc_can_rx;//将VESC接收函数通过函数指针装载到CAN1中断服务函数
    }
    if(CANx==2)
    {
        //CAN初始化：CAN2，普通模式，波特率=120M/（1+5+2）/15=1M，过滤器13，32bit过滤器，扩展帧，id：0，掩码：0，邮箱FIFO1
        Can_Init(2,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS2_2TQ,15,13,CAN_FILTER_32BIT,CAN_Id_Mode_ExtId,0x0000,0x0000,CAN_FILTER_FIFO1);
        CAN2_RX1_IRQ=&vesc_can_rx;//将VESC接收函数通过函数指针装载到CAN2中断服务函数
    }
    VESC_RX=VESC;
    vesc_sign=VESC_Num;
    VESC_CANx=CANx;
}



//VESC的CAN数据接收
//CAN_ExtId：接收地址
//CAN_Rx_Data：接收数据
void vesc_can_rx(uint32_t CAN_ExtId,u8*CAN_Rx_Data)
{
    u8 i=0,j=0;
    while(1)//寻找id吻合的VESC数据存储结构体
    {
        if(i>=vesc_sign)return;//找不到id吻合的VESC数据存储结构体，跳出函数
        if((VESC_RX+i)->vesc_id==(CAN_ExtId&0xff))break;//找到id吻合的VESC数据存储结构体
        i++;
    }
    switch(CAN_ExtId>>8)//根据Packet Status将数据存储到相应位置
    {
        case CAN_PACKET_STATUS:for(j=0;j<8;j++)(VESC_RX+i)->vesc_data_9[j]=*(CAN_Rx_Data+j);break;
        case CAN_PACKET_STATUS_2:for(j=0;j<8;j++)(VESC_RX+i)->vesc_data_14[j]=*(CAN_Rx_Data+j);break;
        case CAN_PACKET_STATUS_3:for(j=0;j<8;j++)(VESC_RX+i)->vesc_data_15[j]=*(CAN_Rx_Data+j);break;
        case CAN_PACKET_STATUS_4:for(j=0;j<8;j++)(VESC_RX+i)->vesc_data_16[j]=*(CAN_Rx_Data+j);break;
        case CAN_PACKET_STATUS_5:for(j=0;j<8;j++)(VESC_RX+i)->vesc_data_27[j]=*(CAN_Rx_Data+j);break;
    }
    return;
}



//VESC发送数据
//id：vesc的id
//VESC_CAN_COMMAND_ID：控制报文
//Data：数据
void VESC_Tx(uint8_t id,VESC_CAN_COMMAND_ID mode,int32_t Data)
{
    u8 i;
    u8 data[8]={0};
    unsigned short can_id=0xff;
    for(i=0;i<4;i++)data[i]=(uint8_t)((Data>>((3-i)*8))&0xff);//数据处理
    can_id=id|(mode<<8);//id处理
    Can_Tx(VESC_CANx,CAN_ID_EXTENDED,can_id,data,8,0);//CAN发送（CAN1，扩展帧，ID:can_id，数据:data，长度8）
}


//VESC速度解算
int VESC_RPM(VESC_DATA_BUF VESC_DATA)
{
    return ((int)VESC_DATA.vesc_data_9[0]<<24)|((int)VESC_DATA.vesc_data_9[1]<<16)|((int)VESC_DATA.vesc_data_9[2]<<8)|((int)VESC_DATA.vesc_data_9[3]);
}

//VESC位置解算
int VESC_POS(VESC_DATA_BUF VESC_DATA)
{
    return ((int)VESC_DATA.vesc_data_16[0]<<8)|((int)VESC_DATA.vesc_data_16[1]);
}


//VESC电流解算
int VESC_CUR(VESC_DATA_BUF VESC_DATA)
{
    return ((int16_t)VESC_DATA.vesc_data_9[4]<<8)|((int16_t)VESC_DATA.vesc_data_9[5]);
}



//void vesc_pos(int id ,int speed,float pos,Chassis_MOTO MOTO,float cnt)//vesc位置环  如：vesc_pos(1 ,1000,2.0,MOTO[0]);    位置环发送数据为：(200*1000000)) 乘1000000为固定格式
//{
//    static float i;
//    static float ab_angle;
//    
//    
//    
////  if(first_state[id]==0)//处于解锁态才运动
////  {
//        if(pos-MOTO.all_round>cnt||pos-MOTO.all_round<-cnt)		//状态1执行，不在同一圈里
//        {
//            if(pos>=MOTO.all_round)
//                VESC_Tx(id+1,CAN_PACKET_SET_RPM,speed);//speed原本没有负号
//            
//            else if(pos<MOTO.all_round)
//                VESC_Tx(id+1,CAN_PACKET_SET_RPM,-speed);//speed原本有负号
//        }
//        else
//        {
//            i=pos;
//            i=i-(int)i;
//            ab_angle=i*360;
////            setangle[id]=MOTO.offset_angle+ab_angle;
////            if(setangle[id]>360)setangle[id]=setangle[id]-360;
////            if(setangle[id]<0) setangle[id]=360-setangle[id];
////            
////            VESC_TX(id+1,CAN_PACKET_SET_POS,((1000000)*((int32_t)setangle[id])));

////            first_state[id]=1;//状态更新为锁定态
//        }
////    }	
////    else//处于锁定态
////    {
////        VESC_TX(id+1,CAN_PACKET_SET_POS,((1000000)*((int32_t)setangle[id])));//锁死电机
////    }
//}



