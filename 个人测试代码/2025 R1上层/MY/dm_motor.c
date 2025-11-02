#include<usart.h>
#include<init.h>
#include<dm_motor.h>


DM_RX_DATA*DM_Rx_Data;//达妙电机接收数据存储
u8 DM_Rx_Len;//达妙电机接收数据存储长度
u8 DM_CANx;



//浮点数转化为整数
//x：输入浮点数
//x_min：最小值
//x_max：最大值
//bits：位数
//返回转化后的整数
int float_to_uint(float x, float x_min, float x_max, int bits)
{ 
  float span = x_max - x_min;
  float offset = x_min;
  return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}


//整数转化为浮点数
//x：输入整数
//x_min：最小值
//x_max：最大值
//bits：位数
//返回转化后的浮点数
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}


uint16_t DM_Motor_Mode;//存储达妙电机模式



//达妙电机初始化
//DM_Rx_Buf：达妙电机接收数据存储
//Buf_Len：达妙电机接收数据存储长度
//mode：电机模式
void DM_Motor_Init(u8 CANx,DM_RX_DATA*DM_Rx_Buf,u8 Buf_Len,uint16_t mode)
{
    u8 i;
    u8 enable_cmd[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc};//使能指令
    uart_init(115200);//初始化串口
    
    DM_Rx_Data=DM_Rx_Buf;//得到电机数据缓存区
    DM_Rx_Len=Buf_Len;//得到电机数据缓存区长度
    DM_CANx=CANx;
    
    if(CANx==1)
    {
        //CAN1，普通模式，波特率=120M/(1+5+2)/15=1M，过滤器0，32位过滤器，标准ID，ID:0，掩码:0，邮箱FIFO0
        Can_Init(1,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS1_2TQ,15,0,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0,0,CAN_FILTER_FIFO0);//初始化CAN
        CAN1_RX0_IRQ=&DM_Motor_Rx;//将DM电机接收函数通过函数指针装载到CAN1中断服务函数
    }
    if(CANx==2)
    {
        //CAN2，普通模式，波特率=120M/(1+5+2)/15=1M，过滤器13，32位过滤器，标准ID，ID:0，掩码:0，邮箱FIFO1
        Can_Init(2,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS1_2TQ,15,13,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0,0,CAN_FILTER_FIFO1);//初始化CAN
        CAN2_RX1_IRQ=&DM_Motor_Rx;//将DM电机接收函数通过函数指针装载到CAN1中断服务函数
    }
    
    for(i=0;i<Buf_Len;i++)Can_Tx(CANx,CAN_ID_STANDARD,mode+(DM_Rx_Buf+i)->id,enable_cmd,8,0);//CAN发送使能数据（CAN1，标准ID，ID:电机控制报文+电机ID，数据:使能指令，长度:8，句尾不加换行）
    
    DM_Motor_Mode=mode;//获取电机模式
}



//达妙电机数据发送
//id：电机id
//_vel：速度
//_pos：位置
//_KP：位置比例参数
//_KD：位置微分参数
//_torq：转矩
//注意-->在MIT模式下：_vel，_pos，_KP，_KD，_torq均有效；在位置速度模式下：_vel，_pos有效；在速度模式下：_vel有效
//MIT注意：速度环,KP=0,KD>0；位置环,KP>0,KD>0；转矩环,KP=0,KD=0;
void DM_Motor_Tx(u8 id,float _vel,float _pos,float _KP,float _KD,float _torq)
{
    u8 Data[8]={0};//发送数据缓存
    uint16_t ID=id+DM_Motor_Mode;//获得控制ID(电机控制报文+电机ID)
    if(DM_Motor_Mode==CtrlMotor_Mode_MIT)//MIT模式
    {
        uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;//位置，速度，比例，微分，转矩
        
        pos_tmp = float_to_uint(_pos, DM_P_MIN, DM_P_MAX, 16);//转换数据类型
        vel_tmp = float_to_uint(_vel, DM_V_MIN, DM_V_MAX, 12);
        kp_tmp = float_to_uint(_KP, DM_KP_MIN, DM_KP_MAX, 12);
        kd_tmp = float_to_uint(_KD, DM_KD_MIN, DM_KD_MAX, 12);
        tor_tmp = float_to_uint(_torq, DM_T_MIN, DM_T_MAX, 12);
        
        Data[0] = (pos_tmp >> 8);//将数据放入发送缓存
        Data[1] = pos_tmp;
        Data[2] = (vel_tmp >> 4);
        Data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
        Data[4] = kp_tmp;
        Data[5] = (kd_tmp >> 4);
        Data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
        Data[7] = tor_tmp;
        
        Can_Tx(DM_CANx,CAN_ID_STANDARD,ID,Data,8,0);//启动CAN发送（CAN1，标准ID，ID:MIT模式控制ID，数据:Data，长度:8，句尾不加换行）
    }
    else if(DM_Motor_Mode==CtrlMotor_Mode_POS_VEL)//位置速度模式
    {
        uint8_t *pbuf,*vbuf;//位置，速度
        pbuf=(uint8_t*)&_pos;//转换数据类型
        vbuf=(uint8_t*)&_vel;
        
        Data[0] = *pbuf;//将数据放入发送缓存
        Data[1] = *(pbuf+1);
        Data[2] = *(pbuf+2);
        Data[3] = *(pbuf+3);
        Data[4] = *vbuf;
        Data[5] = *(vbuf+1);
        Data[6] = *(vbuf+2);
        Data[7] = *(vbuf+3);
        
        Can_Tx(DM_CANx,CAN_ID_STANDARD,ID,Data,8,0);//启动CAN发送（CAN1，标准ID，ID:位置速度模式控制ID，数据:Data，长度:8，句尾不加换行）
    }
    else if(DM_Motor_Mode==CtrlMotor_Mode_VEL)//速度模式
    {
        uint8_t *vbuf;//速度
        vbuf=(uint8_t*)&_vel;//转换数据类型
        
        Data[0] = *vbuf;//将数据放入发送缓存
        Data[1] = *(vbuf+1);
        Data[2] = *(vbuf+2);
        Data[3] = *(vbuf+3);
        
        Can_Tx(DM_CANx,CAN_ID_STANDARD,ID,Data,4,0);//启动CAN发送（CAN1，标准ID，ID:速度模式控制ID，数据:Data，长度:4，句尾不加换行）
    }
}




//达妙电机指令控制
//电机id
//电机指令
void DM_Motor_Cmd(u8 id,u8 cmd)
{
    uint16_t ID=id+DM_Motor_Mode;//获得控制ID(电机控制报文+电机ID)
    u8 data[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};//指令数据格式
    data[7]=cmd;//加入指令
    Can_Tx(DM_CANx,CAN_ID_STANDARD,ID,data,8,0);//发送指令（CAN1，标准ID，ID:控制ID，数据:data，长度:8，句尾不加换行）
}





void DM_Motor_Rx(uint32_t id,u8*buf)
{
    u8 i;
    for(i=0;i<DM_Rx_Len;i++)if(id==(DM_Rx_Data+i)->id)break;
    if(i==DM_Rx_Len)return;
    
    (DM_Rx_Data+i)->pos=uint_to_float((uint16_t)buf[1]<<8|buf[2],DM_P_MIN,DM_P_MAX,16);//得到位置
    (DM_Rx_Data+i)->vel=uint_to_float((uint16_t)buf[3]<<4|buf[4]>>4, DM_V_MIN, DM_V_MAX, 12);//得到速度
    (DM_Rx_Data+i)->t=uint_to_float((buf[4]&0xf)<<8|buf[5], DM_T_MIN, DM_T_MAX, 12);//得到转矩
    (DM_Rx_Data+i)->t_mos=buf[6];//得到MOS管平均温度
    (DM_Rx_Data+i)->t_rotor=buf[7];//得到电机内部线圈的平均温度
    
    if((DM_Rx_Data+i)->last_pos - (DM_Rx_Data+i)->pos>6)(DM_Rx_Data+i)->round++;
    if((DM_Rx_Data+i)->last_pos - (DM_Rx_Data+i)->pos<-6)(DM_Rx_Data+i)->round--;
    (DM_Rx_Data+i)->place = (DM_Rx_Data+i)->round * 25 + (DM_Rx_Data+i)->pos;
    (DM_Rx_Data+i)->last_pos=(DM_Rx_Data+i)->pos;
    
    //故障报告
    if(buf[0]<<4==0x08)printf("电机过压\n");
    if(buf[0]<<4==0x09)printf("电机欠压\n");
    if(buf[0]<<4==0x0A)printf("电机过流\n");
    if(buf[0]<<4==0x0B)printf("MOS过温\n");
    if(buf[0]<<4==0x0C)printf("电机线圈过温\n");
    if(buf[0]<<4==0x0D)printf("通讯丢失\n");
    if(buf[0]<<4==0x0E)printf("电机过载\n");
}




