#include<iic.h>
#include<24cxx.h>
#include<delay.h>

void AT24Cxx_Send(uint16_t add,u8 data)//发送一个数据
{
    iic_start();
    iic_send(0xA0+((add/256)<<1));
    iic_wait_ack(1);
    iic_send(add%256);
    iic_wait_ack(1);
    iic_send(data);
    iic_wait_ack(1);
    iic_stop();
}


u8 AT24Cxx_Read(uint16_t add)//读取一个数据
{
    u8 data;
    iic_start();
    iic_send(0xA0+((add/256)<<1));
    iic_wait_ack(1);
    iic_send(add%256);
    iic_wait_ack(1);
    iic_start();
    iic_send(0xA1);
    iic_wait_ack(1);
    data=iic_read(0);
    iic_stop();
    return data;
}


void AT24Cxx_Many_Send(uint16_t first_add,char*data,uint16_t len)//发生多个数据
{
    uint16_t i;
    for(i=0;i<len;i++)
    {
        AT24Cxx_Send(i+first_add,*(data+i));
        delay_ms(2);
    }
}


void AT24CXx_Many_Read(uint16_t first_add,char*buf,uint16_t len)//读取多个数据
{
    uint16_t i;
    for(i=0;i<len;i++)*(buf+i)=AT24Cxx_Read(i+first_add);
}



u8 AT24Cxx_Init(u8 SCL_GPIO,u8 SCL_PIN,u8 SDA_GPIO,u8 SDA_PIN,uint16_t check)//24Cxx初始化
{
    u8 i=10;
    iic_init(SCL_GPIO,SCL_PIN,SDA_GPIO,SDA_PIN);//初始化io口
    while(check&&i--)//向指定地址读写数据，检测是否正常
    {
        AT24Cxx_Send(check,check%256);
        if(AT24Cxx_Read(check)==check%256)break;
    }
    return !(!i);//检测通过返回1，不通过返回0;
}
