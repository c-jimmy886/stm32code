#include<dma_usart.h>
#include<us.h>
#include<my_library.h>
#include<string.h>


u8 US_USARTx;//串口屏串口号


//串口屏初始化
//USARTx：串口号
//bound：波特率
void US_Init(u8 USARTx,u32 bound)
{
    dma_uart_init(USARTx,bound);//初始化dma串口
    US_USARTx=USARTx;//记录串口号
}


//串口屏数据接收
//buf：接收数据缓存
//len：接收数据长度
u8 US_Rx(u8*buf,u8 len)
{
    if(dma_rx(US_USARTx,(char*)buf,len))return 1;//接收到数据返回1
    return 0;//接收到数据返回0
}


//字节形转整形
//data：字符形数据
//mode：转换模式；0:小端模式，1:大端模式
int ByteToInt(u8*data,u8 mode)
{
    if(mode)return ((int)data[0]<<24)+((int)data[1]<<16)+((int)data[2]<<8)+((int)data[3]<<0);//大端模式
    else return ((int)data[3]<<24)+((int)data[2]<<16)+((int)data[1]<<8)+((int)data[0]<<0);//小端模式
}


//曲线数据透传(Char型)
//page_id：页面id
//widget_id：控件id
//ch：通道（0~2）
//len：数据长度
//data：数据
u16 US_addt_char(u8 page_id,u8 widget_id,u8 ch,u16 len,char*data)
{
    u16 i;
    u8 buf[128]={'a','d','d','t'};//帧头
    buf[4]=page_id;//页面id
    buf[5]=widget_id;//控件id
    buf[6]=ch;//通道
    buf[7]=len&0xff;//数据长度(低位)
    buf[8]=len>>8;//数据长度(高位)
    buf[9]=0,buf[10]=0,buf[11]=0;//保留位
    for(i=0;i<len;i++)buf[i+0+12]=data[i];//char型数据拆分
    buf[i+12]='\r';//帧尾
    i++;
    buf[i+12]='\n';
    i++;
    return dma_tx(US_USARTx,(char*)buf,i+12);//发送数据，成功返回1，失败返回0
}


//曲线数据透传(Short型)
//page_id：页面id
//widget_id：控件id
//ch：通道（0~2）
//len：数据长度
//data：数据
u16 US_addt_short(u8 page_id,u8 widget_id,u8 ch,u16 len,short*data)
{
    u16 i;
    u8 buf[128]={'a','d','d','t'};//帧头
    buf[4]=page_id;//页面id
    buf[5]=widget_id;//控件id
    buf[6]=ch;//通道
    buf[7]=(len*2)&0xff;//数据长度(低位)
    buf[8]=(len*2)>>8;//数据长度(高位)
    buf[9]=0,buf[10]=0,buf[11]=0;//保留位
    for(i=0;i<len*2;i+=2)//short型数据拆分
    {
        buf[i+0+12]=data[i/2]>>0;
        buf[i+1+12]=data[i/2]>>8;
    }
    buf[i+12]='\r';//帧尾
    i++;
    buf[i+12]='\n';
    i++;
    return dma_tx(US_USARTx,(char*)buf,i+12);//发送数据，成功返回1，失败返回0
}


//曲线数据透传(Int型)
//page_id：页面id
//widget_id：控件id
//ch：通道（0~2）
//len：数据长度
//data：数据
u16 US_addt_int(u8 page_id,u8 widget_id,u8 ch,u16 len,int*data)
{
    u16 i;
    u8 buf[128]={'a','d','d','t'};//帧头
    buf[4]=page_id;//页面id
    buf[5]=widget_id;//控件id
    buf[6]=ch;//通道
    buf[7]=(len*4)&0xff;//数据长度(低位)
    buf[8]=(len*4)>>8;//数据长度(高位)
    buf[9]=0,buf[10]=0,buf[11]=0;//保留位
    for(i=0;i<len*4;i+=4)//int型数据拆分
    {
        buf[i+0+12]=data[i/4]>>0;
        buf[i+1+12]=data[i/4]>>8;
        buf[i+2+12]=data[i/4]>>16;
        buf[i+3+12]=data[i/4]>>24;
    }
    buf[i+12]='\r';//帧尾
    i++;
    buf[i+12]='\n';
    i++;
    return dma_tx(US_USARTx,(char*)buf,i+12);//发送数据，成功返回1，失败返回0
}


//曲线数据透传(float型)
//page_id：页面id
//widget_id：控件id
//ch：通道（0~2）
//len：数据长度
//data：数据
u16 US_addt_float(u8 page_id,u8 widget_id,u8 ch,u16 len,float*data)
{
    u16 i;
    unsigned int Data;
    u8 buf[128]={'a','d','d','t'};//帧头
    buf[4]=page_id;//页面id
    buf[5]=widget_id;//控件id
    buf[6]=ch;//通道
    buf[7]=(len*4)&0xff;//数据长度(低位)
    buf[8]=(len*4)>>8;//数据长度(高位)
    buf[9]=0,buf[10]=0,buf[11]=0;//保留位
    for(i=0;i<len*4;i+=4)//float型数据拆分
    {
        Data=FloatToHex_union(data[i/4]);//浮点数转十六进制
        buf[i+0+12]=Data>>0;
        buf[i+1+12]=Data>>8;
        buf[i+2+12]=Data>>16;
        buf[i+3+12]=Data>>24;
    }
    buf[i+12]='\r';//帧尾
    i++;
    buf[i+12]='\n';
    i++;
    return dma_tx(US_USARTx,(char*)buf,i+12);//发送数据，成功返回1，失败返回0
}


//切换页面
//page_name_id：页面名/id
void US_page(char*page_name_id)
{
    u8 data[64]="page ";//帧头
    char enter[3]="\r\n";//帧尾
    strcat((char*)data,page_name_id);//字符串拼接
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
}


//复位屏幕
void US_reset()
{
    dma_tx(US_USARTx,"reset 1\r\n",9);
}


//模拟点击
//widget_name：控键名（支持 按钮，触摸热区，双态按钮）
void US_click(char*widget_name)
{
    u8 data[64]="click ";//帧头
    char enter[3]="\r\n";//帧尾
    strcat((char*)data,widget_name);//字符串拼接
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
}


//事件触发
//widget_name：控键名（支持 按钮，触摸热区，双态按钮，定时器）
//val：触发类型
void US_event(char*widget_name,u8 val)
{
    u8 data[64]="event ";//帧头
    char enter[3]="\r\n";//帧尾
    char Val[3]={' '};//触发类型字符缓存
    Val[1]=val+'0';
    strcat((char*)data,widget_name);//字符串拼接
    strcat((char*)data,Val);
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
}


//控件属性设置
//name_natue：页面/控件名.属性名
//value：属性值
void US_wset(char*name_natue,char*value)
{
    u8 data[128]="wset ";//帧头
    char enter[3]="\r\n";//帧尾
    strcat((char*)data,name_natue);//字符串拼接
    strcat((char*)data," ");
    strcat((char*)data,value);
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
}



//控件属性读取
//name_natue：页面/控件名.属性名
//reset_value：返回属性值
//返回1:接收到整数；返回2:接收到小数；返回3:接收到字符串
u8 US_wget(char*name_natue,US_Get*reset_value)
{
    int i=0xffff,j;
    u8 buf[128];
    u8 data[128]="wget ";//帧头
    char enter[3]="\r\n";//帧尾
    strcat((char*)data,name_natue);//字符串拼接
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
    while(i--)//等待数据回传
    {
        if(US_Rx((u8*)buf,128))//接收到回传数据
        {
            switch(buf[0])
            {
                case 0x01:
                reset_value->Int=0;
                reset_value->Int|=(int)buf[1]<<0;
                reset_value->Int|=(int)buf[2]<<8;
                reset_value->Int|=(int)buf[3]<<16;
                reset_value->Int|=(int)buf[4]<<24;
                return 0x01;
                
                case 0x02:
                reset_value->Int=0;
                reset_value->Int|=(int)buf[1]<<0;
                reset_value->Int|=(int)buf[2]<<8;
                reset_value->Int|=(int)buf[3]<<16;
                reset_value->Int|=(int)buf[4]<<24;
                return 0x02;
                
                case 0x03:
                for(j=0;;j++)
                {
                    if(buf[j+1]=='\r'&&buf[j+2]=='\n')break;
                    reset_value->Char[j]=buf[j+1];
                }
                return 0x03;
            }
        }
    }
    return 0;//超时未接收到数据，返回0
}



//系统变量设置
//sys_name：系统变量名
//value：系统变量值
void US_sset(char*sys_name,char*value)
{
    u8 data[128]="sset ";//帧头
    char enter[3]="\r\n";//帧尾
    strcat((char*)data,sys_name);//字符串拼接
    strcat((char*)data," ");
    strcat((char*)data,value);
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
}


//系统变量读取
//sys_name：系统变量名
//reset_value：返回系统变量值
u8 US_sget(char*sys_name,int*reset_value)
{
    int i=0xffff;
    u8 buf[8];
    u8 data[128]="sget ";//帧头
    char enter[3]="\r\n";//帧尾
    strcat((char*)data,sys_name);//字符串拼接
    strcat((char*)data,enter);
    dma_tx(US_USARTx,(char*)data,strlen((char*)data));//发送指令
    while(i--)//等待数据回传
    {
        if(US_Rx(buf,7))//接收到回传数据
        {
            *reset_value=0;
            *reset_value|=(int)buf[1]<<0;
            *reset_value|=(int)buf[2]<<8;
            *reset_value|=(int)buf[3]<<16;
            *reset_value|=(int)buf[4]<<24;
            return 1;//返回1
        }
    }
    return 0;//超时未接收到数据，返回0
}
