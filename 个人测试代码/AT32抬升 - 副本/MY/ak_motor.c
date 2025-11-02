#include<dm_motor.h>
#include<ak_motor.h>
#include<init.h>
#include<usart.h>
#include<dma_usart.h>


AK_RX_DATA*AK_MIT_Data;//AK电机MIT接收数据存储
AK_RX_DATA*AK_SERVO_Data;//AK电机伺服接收数据存储
u8 AK_MIT_Len;//AK电机MIT接收数据存储长度
u8 AK_SERVO_Len;//AK电机伺服接收数据存储长度
u8 AK_MIT_CANx;
u8 AK_SERVO_CANx;



void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index)
{
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}


void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index) 
{
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}


int16_t buffer_get_int16(u8*buffer)
{
    int16_t number=0;
    number|=(int16_t)*(buffer+0)<<8;
    number|=(int16_t)*(buffer+1)<<0;
    return number;
}


int32_t buffer_get_int32(u8*buffer)
{
    int32_t number=0;
    number|=(int32_t)*(buffer+0)<<24;
    number|=(int32_t)*(buffer+1)<<16;
    number|=(int32_t)*(buffer+2)<<8;
    number|=(int32_t)*(buffer+3)<<0;
    return number;
}


//AK电机MIT命令
//id：电机id
//cmd：指令
void AK_Motor_MIT_Cmd(u8 id,u8 cmd)
{
    uint16_t ID=id;//获得控制ID(电机控制报文+电机ID)
    u8 data[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};//指令数据格式
    data[7]=cmd;//加入指令
    Can_Tx(AK_MIT_CANx,CAN_ID_STANDARD,ID,data,8,0);//发送指令（CAN1，标准ID，ID:控制ID，数据:data，长度:8，句尾不加换行）
}


//AK电机初始化
//id：电机id
void AK_MIT_Init(u8 CANx,AK_RX_DATA*AK_Rx_Buf,u8 Buf_Len)
{
    uart_init(115200);
    
    AK_MIT_Data=AK_Rx_Buf;//得到电机数据缓存区
    AK_MIT_Len=Buf_Len;//得到电机数据缓存区长度
    AK_MIT_CANx=CANx;
    
    if(CANx==1)
    {
        Can_Init(1,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS1_2TQ,15,0,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0,0,CAN_FILTER_FIFO0);//初始化CAN
        //CAN1，普通模式，波特率=120M/(1+5+2)/15=1M，过滤器0，32位过滤器，标准ID，ID:0，掩码:0，邮箱FIFO0
        CAN1_RX0_IRQ=&AK_Motor_MIT_RX;
    }
    if(CANx==2)
    {
        Can_Init(2,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS1_2TQ,15,13,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0,0,CAN_FILTER_FIFO1);//初始化CAN
        //CAN2，普通模式，波特率=120M/(1+5+2)/15=1M，过滤器13，32位过滤器，标准ID，ID:0，掩码:0，邮箱FIFO1
        CAN2_RX1_IRQ=&AK_Motor_MIT_RX;//将DM电机接收函数通过函数指针装载到CAN1中断服务函数
    }
}


void AK_SERVO_Init(u8 CANx,AK_RX_DATA*AK_Rx_Buf,u8 Buf_Len)
{
    u8 i;
    uart_init(115200);
    
    AK_SERVO_Data=AK_Rx_Buf;//得到电机数据缓存区
    AK_SERVO_Len=Buf_Len;
    AK_SERVO_CANx=CANx;
    
    if(CANx==1)
    {
        Can_Init(1,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS1_2TQ,15,0,CAN_FILTER_32BIT,CAN_Id_Mode_ExtId,0,0,CAN_FILTER_FIFO0);//初始化CAN
        //CAN1，普通模式，波特率=120M/(1+5+2)/15=1M，过滤器0，32位过滤器，扩展ID，ID:0，掩码:0，邮箱FIFO0
        CAN1_RX0_IRQ=&AK_Motor_SERVO_RX;
    }
    if(CANx==2)
    {
        Can_Init(2,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS1_2TQ,15,13,CAN_FILTER_32BIT,CAN_Id_Mode_ExtId,0,0,CAN_FILTER_FIFO1);//初始化CAN
        //CAN2，普通模式，波特率=120M/(1+5+2)/15=1M，过滤器13，32位过滤器，扩展ID，ID:0，掩码:0，邮箱FIFO1
        CAN2_RX1_IRQ=&AK_Motor_SERVO_RX;//将DM电机接收函数通过函数指针装载到CAN1中断服务函数
    }
    
    for(i=0;i<Buf_Len;i++)(AK_SERVO_Data+i)->last_pos=-1.0f;
}



//AK电机MIT模式发送
//id：电机id
//_vel：速度
//_pos：位置
//_KP：比例
//_KD：微分
//_torq：力矩
void AK_Motor_MIT_Tx(u8 id,float _vel,float _pos,float _KP,float _KD,float _torq)
{
    u8 data[8];
    int p_int,v_int,kp_int,kd_int,t_int;
    
    p_int  = float_to_uint(_pos,AK_P_MIN,AK_P_MAX,16);//转换数据类型
    v_int  = float_to_uint(_vel,AK_V_MIN,AK_V_MAX,12);
    kp_int = float_to_uint(_KP,AK_Kp_MIN,AK_Kp_MAX,12);
    kd_int = float_to_uint(_KD,AK_Kd_MIN,AK_Kd_MAX,12);
    t_int  = float_to_uint(_torq ,AK_T_MIN,AK_T_MAX,12);

    data[0] = p_int>>8;//位置高 8
    data[1] = p_int&0xFF;//位置低 8
    data[2] = v_int>>4;//速度高 8 位
    data[3] = ((v_int&0xF)<<4)|(kp_int>>8);//速度低 4 位.KP 高 4 位
    data[4] = kp_int&0xFF; //KP 低 8 位
    data[5] = kd_int>>4;//Kd 高 8 位
    data[6] = ((kd_int&0xF)<<4)|(t_int>>8);//KP 低 4 位,扭矩高 4 位
    data[7] = t_int&0xff;//扭矩低 8 位
    
    Can_Tx(AK_MIT_CANx,CAN_ID_STANDARD,id,data,8,0);//启动CAN发送（CANx，标准ID，ID:电机id，数据:Data，长度:8，句尾不加换行）
}


//占空比模式（伺服）
//duty:占空比
void AK_Duty_Tx(u8 id,float duty)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(duty * 100000.0f), &send_index);
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_DUTY<<8),buffer,4,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//电流环模式（伺服）
//current:电流（-60~60(A)）
void AK_Current_Tx(u8 id,float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0f), &send_index);
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_CURRENT<<8),buffer,4,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//电流刹车模式（伺服）
//current:电流（-60~60(A)）
void AK_CB_Tx(u8 id,float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0f), &send_index);
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_CURRENT_BRAKE<<8),buffer,4,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//速度环模式（伺服）
//rpm:转速（-100000~100000(RPM)）
void AK_RPM_Tx(u8 id,int rpm)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)rpm, &send_index);
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_RPM<<8),buffer,4,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//位置环模式（伺服）
//angle:角度（-36000°~36000°）
void AK_POS_Tx(u8 id,float angle)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(angle * 10000.0f), &send_index);
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_POS<<8),buffer,4,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//设置原点模式（伺服）
//mode:设置模式（0:临时原点；1:永久原点）
//设置永久原点时角度会设置成180°，断电后会回到12.3°
void AK_Origin_Tx(u8 id,u8 mode)
{
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_ORIGIN_HERE<<8),&mode,1,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//位置速度模式（伺服）
//angle:角度（-36000°~36000°）
//rpm:速度(-327680~327670(RPM))
//rpa:加速度（0~327670(RPM/S/S)）
void AK_POS_SPD_Tx(u8 id,float angle,int rpm,int rpa)
{
    int32_t send_index = 0;
    int16_t send_index1 = 4;
    uint8_t buffer[8];
    buffer_append_int32(buffer, (int32_t)(angle * 10000.0f), &send_index);
    buffer_append_int16(buffer,rpm/10.0f, & send_index1);
    buffer_append_int16(buffer,rpa/10.0f, & send_index1);
    Can_Tx(AK_SERVO_CANx,CAN_ID_EXTENDED,id|((uint32_t)AK_PACKET_SET_POS_SPD<<8),buffer,8,0);//启动CAN发送（CANx，扩展ID，ID:电机id，数据:buffer，长度:4，句尾不加换行）
}


//AK电机MIT模式接收
void AK_Motor_MIT_RX(uint32_t id,u8*buf)
{
    u8 i;
    for(i=0;i<AK_MIT_Len;i++)
    {
        if((AK_MIT_Data+i)->id==id)
        {
            (AK_MIT_Data+i)->pos =uint_to_float(((int)buf[1]<<8)|buf[2], AK_P_MIN, AK_P_MAX, 16);//电机位置数据
            (AK_MIT_Data+i)->vel =uint_to_float(((int)buf[3]<<4)|(buf[4]>>4), AK_V_MIN, AK_V_MAX, 12);//电机速度数据
            (AK_MIT_Data+i)->cur =uint_to_float((((int)buf[4]&0x0f)<<8)|buf[5], AK_T_MIN, AK_T_MAX, 12);//电机扭矩数据
            (AK_MIT_Data+i)->temp=buf[6]-40;
            
            if((AK_MIT_Data+i)->last_pos - (AK_MIT_Data+i)->pos>6)(AK_MIT_Data+i)->round++;
            if((AK_MIT_Data+i)->last_pos - (AK_MIT_Data+i)->pos<-6)(AK_MIT_Data+i)->round--;
            (AK_MIT_Data+i)->place = (AK_MIT_Data+i)->round * 25 + (AK_MIT_Data+i)->pos;
            (AK_MIT_Data+i)->last_pos=(AK_MIT_Data+i)->pos;
            break;
        }
    }
    switch(buf[7])//错误报告
    {
        case FAULT_CODE_OVER_VOLTAGE:printf("Error：过压\n");break;
        case FAULT_CODE_UNDER_VOLTAGE:printf("Error：欠压\n");break;
        case FAULT_CODE_DRV:printf("Error：驱动故障\n");break;
        case FAULT_CODE_ABS_OVER_CURRENT:printf("Error：电机过流\n");break;
        case FAULT_CODE_OVER_TEMP_FET:printf("Error：MOS过温\n");break;
        case FAULT_CODE_OVER_TEMP_MOTOR:printf("Error：电机过温\n");break;
        case FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE:printf("Error：驱动过压\n");break;
        case FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE:printf("Error：驱动欠压\n");break;
        case FAULT_CODE_MCU_UNDER_VOLTAGE:printf("Error：MCU欠压\n");break;
        case FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET:printf("Error：欠压\n");break;
        case FAULT_CODE_ENCODER_SPI:printf("Error：SPI编码器故障\n");break;
        case FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE:printf("Error：编码器超过最小限制\n");break;
        case FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE:printf("Error：编码器超过最大限制\n");break;
        case FAULT_CODE_FLASH_CORRUPTION:printf("Error：FLASH故障\n");break;
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1:printf("Error：电流采样通道 1 故障\n");break;
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2:printf("Error：电流采样通道 2 故障\n");break;
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3:printf("Error：电流采样通道 3 故障\n");break;
        case FAULT_CODE_UNBALANCED_CURRENTS:printf("Error：电流不平衡\n");break;
    }
}


//AK伺服模式数据接收
void AK_Motor_SERVO_RX(uint32_t id,u8*buf)
{
    u8 i;
    for(i=0;i<AK_SERVO_Len;i++)
    {
        if((AK_SERVO_Data+i)->id==(id&0xFF))
        {
            (AK_SERVO_Data+i)->pos=(float)((int16_t)(buf[0]<<8|buf[1]))/10.0f;//电机位置数据
            (AK_SERVO_Data+i)->vel=(float)((int16_t)(buf[2]<<8|buf[3]))*10.0f;//电机速度数据
            (AK_SERVO_Data+i)->cur=(float)((int16_t)(buf[5]<<8|buf[5]))/100.0f;//电机扭矩数据
            (AK_SERVO_Data+i)->temp=buf[6];
            
            if((AK_SERVO_Data+i)->last_pos - (AK_SERVO_Data+i)->pos>180 && (AK_SERVO_Data+i)->last_pos>=0)(AK_SERVO_Data+i)->round++;
            if((AK_SERVO_Data+i)->last_pos - (AK_SERVO_Data+i)->pos<-180 && (AK_SERVO_Data+i)->last_pos>=0)(AK_SERVO_Data+i)->round--;
            (AK_SERVO_Data+i)->place = (AK_SERVO_Data+i)->round * 360 + (AK_SERVO_Data+i)->pos;
            (AK_SERVO_Data+i)->last_pos=(AK_SERVO_Data+i)->pos;
            break;
        }
    }
    switch(buf[7])//错误报告
    {
        case FAULT_CODE_OVER_VOLTAGE:printf("Error：过压\n");break;
        case FAULT_CODE_UNDER_VOLTAGE:printf("Error：欠压\n");break;
        case FAULT_CODE_DRV:printf("Error：驱动故障\n");break;
        case FAULT_CODE_ABS_OVER_CURRENT:printf("Error：电机过流\n");break;
        case FAULT_CODE_OVER_TEMP_FET:printf("Error：MOS过温\n");break;
        case FAULT_CODE_OVER_TEMP_MOTOR:printf("Error：电机过温\n");break;
        case FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE:printf("Error：驱动过压\n");break;
        case FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE:printf("Error：驱动欠压\n");break;
        case FAULT_CODE_MCU_UNDER_VOLTAGE:printf("Error：MCU欠压\n");break;
        case FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET:printf("Error：欠压\n");break;
        case FAULT_CODE_ENCODER_SPI:printf("Error：SPI编码器故障\n");break;
        case FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE:printf("Error：编码器超过最小限制\n");break;
        case FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE:printf("Error：编码器超过最大限制\n");break;
        case FAULT_CODE_FLASH_CORRUPTION:printf("Error：FLASH故障\n");break;
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1:printf("Error：电流采样通道 1 故障\n");break;
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2:printf("Error：电流采样通道 2 故障\n");break;
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3:printf("Error：电流采样通道 3 故障\n");break;
        case FAULT_CODE_UNBALANCED_CURRENTS:printf("Error：电流不平衡\n");break;
    }
}


/***AK80-8***/
//伺服
//V:-20000~20000
//P:0~360







/*================================================串口协议==============================================*/



#define AK_BOUND 921600//Ak电机串口波特率

#define USART_Tx(usartx,data,len)  if(DmaEnable&(1<<(usartx-1)))dma_tx(usartx,data,len);  else USARTx_Tx (usartx,data,len)
#define USART_Rx(usartx,data,len)  if(DmaEnable&(1<<(usartx-1)))Rx_flag=dma_rx(usartx,data,len);  else Rx_flag=USARTx_Rx (usartx,data,len)

u8 DmaEnable=0;


/*CRC*/
const unsigned short crc16_tab[] = {   0x0000, 0x1021, 0x2042, 0x3063, 
0x4084,0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c,
0xd1ad,0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294,
0x72f7,0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff,
0xe3de,0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
0xa56a,0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653,
0x2672,0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a,
0x9719,0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886,
0x78a7,0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af,
0x8948,0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71,
0x0a50,0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58,
0xbb3b,0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60,
0x1c41,0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
0x7e97,0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f,
0xefbe,0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9,
0xb1ca,0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2,
0x20e3,0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da,
0xc33d,0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235,
0x5214,0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f,
0xd52c,0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424,
0x4405,0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
0x26d3,0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c,
0xc96d,0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865,
0x7806,0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f,
0xfb1e,0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16,
0x0af1,0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa,
0xad8b,0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83,
0x1ce0,0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9,
0x9ff8,0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
//CRC计算
unsigned short crc16(unsigned char *buf, unsigned int len) 
{
    unsigned int i;
    unsigned short cksum = 0;
    for (i = 0; i < len; i++) 
    {
        cksum = crc16_tab[(((cksum >> 8) ^ *buf++) & 0xFF)] ^ (cksum << 8);
    }
    return cksum;
}




//AK串口初始化
//USARTx：串口号
//dma_enable：1:使用DMA，2:不使用DMA
void AK_SERVO_USART_Init(u8 USARTx,u8 dma_enable)
{
    if(dma_enable)//使用DMA
    {
        dma_uart_init(USARTx,AK_BOUND);//将对应串口初始化为DMA串口
        DmaEnable|=1<<(USARTx-1);//将对应位标记为使用dma
    }
    else Uartx_Init(USARTx,AK_BOUND);//不使用dma，直接初始化串口
    
}


//获取一次电机运行参数
void AK_SERVO_USART_GetValues(u8 USARTx)
{
    char send_data[8]={0x02,0x01,0x04,0x40,0x84,0x03};
    USART_Tx(USARTx,send_data,6);
}

//设置电机10ms返回一次位置
void AK_SERVO_USART_GetDetect(u8 USARTx)
{
    char send_data[8]={0x02,0x02,0x0B,0x04,0x9C,0x7E,0x03};
    USART_Tx(USARTx,send_data,7);
}

//设置电机原点
void AK_SERVO_USART_SetOrigin(u8 USARTx)
{
    char send_data[8]={0x02,0x02,0x5F,0x01,0x0E,0x0A,0x03};
    USART_Tx(USARTx,send_data,7);
}

//设置电机位置环为多圈运行模式 ±100 圈（!）
void AK_SERVO_USART_SetManyRound(u8 USARTx)
{
    char send_data[16]={0x02,0x05,0x5C,0x00,0x00,0x00,0x00,0x9E,0x19,0x03};
    USART_Tx(USARTx,send_data,10);
}

//设置电机位置环为单圈运行模式（!）
void AK_SERVO_USART_SetOneRound(u8 USARTx)
{
    char send_data[16]={0x02,0x05,0x5D,0x00,0x00,0x00,0x00,0x34,0x48,0x03};
    USART_Tx(USARTx,send_data,10);
}

//设置电机位置最短距离回零指令（!）
void AK_SERVO_USART_SetCypher(u8 USARTx)
{
    char send_data[16]={0x02,0x05,0x65,0x00,0x00,0x00,0x00,0x3A,0x8B,0x03};
    USART_Tx(USARTx,send_data,10);
}


//电机单个或多个参数获取指令
//DATA_MODE：设置返回的数据
void AK_SERVO_USART_GetValuesSetup(u8 USARTx,uint32_t AK_USART_RX)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    uint8_t send_data[16]={0x02,0x05,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,AK_USART_RX,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}


//电机以占空比模式运行
//DUTY：占空比
void AK_SERVO_USART_SetDuty(u8 USARTx,float DUTY)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Duty=DUTY*100000;
    uint8_t send_data[16]={0x02,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Duty,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}

//电机以电流环模式运行
//CURRENT：电流
void AK_SERVO_USART_SetCurrent(u8 USARTx,float CURRENT)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Current=CURRENT*1000;
    uint8_t send_data[16]={0x02,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Current,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}

//电机电流刹车模式模式运行
//CURRENT：电流
void AK_SERVO_USART_SetCB(u8 USARTx,float CURRENT)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Current=CURRENT*1000;
    uint8_t send_data[16]={0x02,0x05,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Current,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}

//电机以速度环模式运行
//RPM：转速
void AK_SERVO_USART_SetRPM(u8 USARTx,int RPM)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Rpm=RPM;
    uint8_t send_data[16]={0x02,0x05,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Rpm,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}

//电机以位置环模式运行
//POS：位置
void AK_SERVO_USART_SetPOS(u8 USARTx,int POS)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Pos=POS*1000000;
    uint8_t send_data[16]={0x02,0x05,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Pos,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}

//电机电流手刹模式模式运行
//CURRENT：电流
void AK_SERVO_USART_SetHB(u8 USARTx,float CURRENT)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Current=CURRENT*1000;
    uint8_t send_data[16]={0x02,0x05,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Current,&send_index);
    CRC16=crc16(send_data+2,5);
    send_data[7]=CRC16>>8;
    send_data[8]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,10);
}

//电机以位置速度环模式运行
//POS：位置
//RPM：速度
//RPA：加速度
void AK_SERVO_USART_SetPOS_SetSPD(u8 USARTx,float POS,float RPM,float RPA)
{
    uint16_t CRC16;
    int32_t send_index = 0;
    int Pos=POS*1000;
    int Rpm=RPM;
    int Rpa=RPA;
    uint8_t send_data[32]={0x02,0x0D,0x5B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03};
    
    buffer_append_int32(send_data+3,Pos,&send_index);
    buffer_append_int32(send_data+3,Rpm,&send_index);
    buffer_append_int32(send_data+3,Rpa,&send_index);
    CRC16=crc16(send_data+2,13);
    send_data[15]=CRC16>>8;
    send_data[16]=CRC16;
    
    USART_Tx(USARTx,(char*)send_data,18);
}



//AK串口接收
u8 AK_USART_RX(u8 USARTx,AK_USART_RX_DATA*AK_USART_RX_BUF)
{
    u8 Rx_flag=0;//接收标志
    u8 len;//数据长度
    u8*buf_p;
    uint32_t data_flag=0;
    u8 buf[128]={0};//数据缓存
    USART_Rx(USARTx,(char*)buf,100);//接收数据函数
    
    if(Rx_flag&&buf[0]==0x02)//接收到数据，且帧头正确
    {
        len=buf[1];//得到数据长度
        if(buf[len+1+3]==0x03)//帧尾正确
        {
            switch(buf[2])//数据帧
            {
                case 0x04:AK_USART_RX_BUF->mos_temp=(float)buffer_get_int16(buf+3)/10.0f;//MOS温度
                          AK_USART_RX_BUF->motor_temp=(float)buffer_get_int16(buf+5)/10.0f;//电机温度
                          AK_USART_RX_BUF->out_current=(float)buffer_get_int32(buf+7)/100.0f;//输出电流
                          AK_USART_RX_BUF->in_current=(float)buffer_get_int32(buf+11)/100.0f;//输入电流
                          AK_USART_RX_BUF->id_current=(float)buffer_get_int32(buf+15)/100.0f;//id电流
                          AK_USART_RX_BUF->iq_current=(float)buffer_get_int32(buf+19)/100.0f;//iq电流
                          AK_USART_RX_BUF->duty=(float)buffer_get_int16(buf+23)/1000.0f;//占空比
                          AK_USART_RX_BUF->vel=(float)buffer_get_int32(buf+25);//速度
                          AK_USART_RX_BUF->in_voltage=(float)buffer_get_int16(buf+29)/10.0f;//输入电压
                          AK_USART_RX_BUF->start=*(buf+55);//电机状态
                          AK_USART_RX_BUF->pos=(float)buffer_get_int32(buf+56)/1000000.0f;//位置
                          AK_USART_RX_BUF->id=*(buf+60);//电机ID
                          AK_USART_RX_BUF->vd_voltage=(float)buffer_get_int32(buf+67)/1000.0f;//vd电压
                          AK_USART_RX_BUF->vq_voltage=(float)buffer_get_int32(buf+71)/1000.0f;//vq电压
                          break;
                case 0x16:AK_USART_RX_BUF->pos=(float)buffer_get_int32(buf+3)/10000.0f;//电机位置
                          break;
                case 0x32:data_flag=(((uint32_t)buf[3]<<24)|((uint32_t)buf[4]<<16)|((uint32_t)buf[5]<<8)|((uint32_t)buf[6]<<0));
                          buf_p=buf+7;
                          if(data_flag&((uint32_t)(1<<(1-1)))){AK_USART_RX_BUF->mos_temp=(float)buffer_get_int16(buf_p)/10.0f;buf_p+=2;}
                          if(data_flag&((uint32_t)(1<<(2-1)))){AK_USART_RX_BUF->motor_temp=(float)buffer_get_int16(buf_p)/10.0f;buf_p+=2;}
                          if(data_flag&((uint32_t)(1<<(3-1)))){AK_USART_RX_BUF->out_current=(float)buffer_get_int32(buf_p)/100.0f;buf_p+=4;}
                          if(data_flag&((uint32_t)(1<<(4-1)))){AK_USART_RX_BUF->in_current=(float)buffer_get_int32(buf_p)/100.0f;buf_p+=4;}
                          if(data_flag&((uint32_t)(1<<(5-1)))){AK_USART_RX_BUF->id_current=(float)buffer_get_int32(buf_p)/100.0f;buf_p+=4;}
                          if(data_flag&((uint32_t)(1<<(6-1)))){AK_USART_RX_BUF->iq_current=(float)buffer_get_int32(buf_p)/100.0f;buf_p+=4;}
                          if(data_flag&((uint32_t)(1<<(7-1)))){AK_USART_RX_BUF->duty=(float)buffer_get_int16(buf_p)/1000.0f;buf_p+=2;}
                          if(data_flag&((uint32_t)(1<<(8-1)))){AK_USART_RX_BUF->vel=(float)buffer_get_int32(buf_p);buf_p+=4;}
                          if(data_flag&((uint32_t)(1<<(9-1)))){AK_USART_RX_BUF->in_voltage=(float)buffer_get_int16(buf_p)/10.0f;buf_p+=2;}
                          if(data_flag&((uint32_t)(1<<(16-1)))){AK_USART_RX_BUF->start=*buf_p;buf_p+=1;}
                          if(data_flag&((uint32_t)(1<<(17-1)))){AK_USART_RX_BUF->pos=(float)buffer_get_int32(buf_p)/1000000.0f;buf_p+=4;}
                          if(data_flag&((uint32_t)(1<<(18-1)))){AK_USART_RX_BUF->id=*buf_p;buf_p+=1;}
                          break;
            }
        }
        return Rx_flag;
    }
    return 0;
}



