#include<dma_usart.h>
#include<hwt901.h>


//注意：因为是用DMA串口通讯，所以发送数据不要过快，防止HWT901接收不到


u8 HWT901_USARTx;//记录HWT901串口


//HWT901写指令
//HWT901_CMD：定义的HWT901指令
void HWT901_Cmd(uint32_t HWT901_CMD)
{
    u8 buf_cmd[5]={0xFF,0xAA,0,0,0};//指令数据结构缓存
    buf_cmd[2]=HWT901_CMD>>16;//拆解定义的指令，并放进指令数据缓存
    buf_cmd[3]=HWT901_CMD>>8;
    buf_cmd[4]=HWT901_CMD>>0;
    while(1)if(dma_tx(HWT901_USARTx,(char*)buf_cmd,5))break;//循环发送指令，直到发送成功
}



//HWT901初始化
//USARTx：串口号
//bound：波特率
void HWT901_Init(u8 USARTx,u32 bound)
{
    dma_uart_init(USARTx,bound);//初始化DMA串口
    HWT901_USARTx=USARTx;//记录初始化的串口号
    HWT901_Cmd(HWT901_CMD_LOCK0);//发送解锁指令
}



//设置HWT901输出类型
//HWT901_Output：定义的HWT901输出类型
void HWT901_Output_Mode(u8 HWT901_Output)
{
    
    uint16_t HWT901_Output_Mode_Cmd;//输出类型指令
    u8 buf_cmd[5]={0xFF,0xAA,0x02,0,0};//指令数据结构缓存
    HWT901_Output_Mode_Cmd=1<<(HWT901_Output-0x50);//得到输出类型指令
    buf_cmd[3]=(HWT901_Output_Mode_Cmd&0x0f)>>0;//输出类型指令，并放进指令数据缓存
    buf_cmd[4]=(HWT901_Output_Mode_Cmd&0xf0)>>8;
    while(1)if(dma_tx(3,(char*)buf_cmd,5))break;//拆解定义的指令，并放进指令数据缓存
}



//HWT901数据接收
//buf：数据缓存
u8 HWT901_Rx(u8*buf)
{
    if(dma_rx(HWT901_USARTx,(char*)buf,11))return buf[1];//接收到数据，返回接收数据类型
    else return 0;//未接收到数据，返回0
}



//接收读取计算HWT901三轴数据
//X：X轴数据
//Y：Y轴数据
//Z：Z轴数据
//注意：该函数只能接收 加速度、角速度、角度、磁场 的数据
u8 HWT901_XYZ_Rx(float*X,float*Y,float*Z)
{
    u8 buf[12];//数据缓存
    if(dma_rx(HWT901_USARTx,(char*)buf,11))//接收到数据
    {
        if(buf[0]==0x55)//数据帧头正确
        {
            switch(buf[1])
            {
                case HWT901_Output_Acceleration: *X=(float)((int16_t)(buf[3]<<8)|buf[2])/32768*16;//加速度的数据
                                                 *Y=(float)((int16_t)(buf[5]<<8)|buf[4])/32768*16;//计算并得到三轴加速度
                                                 *Z=(float)((int16_t)(buf[7]<<8)|buf[6])/32768*16;
                                                 return buf[1];//返回数据类型（为加速度）
                case HWT901_Output_Palstance:    *X=(float)((int16_t)(buf[3]<<8)|buf[2])/32768*200;//角速度的数据
                                                 *Y=(float)((int16_t)(buf[5]<<8)|buf[4])/32768*200;//计算并得到三轴角速度
                                                 *Z=(float)((int16_t)(buf[7]<<8)|buf[6])/32768*200;
                                                 return buf[1];//返回数据类型（为角速度）
                case HWT901_Output_Angle:        *X=(float)((int16_t)(buf[3]<<8)|buf[2])/32768*180;//角度的数据
                                                 *Y=(float)((int16_t)(buf[5]<<8)|buf[4])/32768*180;//计算并得到三轴角度
                                                 *Z=(float)((int16_t)(buf[7]<<8)|buf[6])/32768*180;
                                                 return buf[1];//返回数据类型（为角度）
                case HWT901_Output_MagneticField:*X=(float)((int16_t)(buf[3]<<8)|buf[2]);//磁场的数据
                                                 *Y=(float)((int16_t)(buf[5]<<8)|buf[4]);//计算并得到三轴磁场
                                                 *Z=(float)((int16_t)(buf[7]<<8)|buf[6]);
                                                 return buf[1];//返回数据类型（为磁场）
            }
            
        }
    }
    return 0;//未接收到数据 或 帧头不正确 或 接收到的数据不属于规定的四种数据，返回0
}
