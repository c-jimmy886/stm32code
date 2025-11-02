#include<iic.h>
#include<sys.h>
#include<delay.h>
#include<init.h>


u8 IIC_SDA_GPIO,IIC_SDA_PIN;
volatile unsigned long*iic_scl_gpio;//SCL
volatile unsigned long*iic_sda_gpio_out;//SDA
volatile unsigned long*iic_sda_gpio_in;
void iic_init(char SCL_GPIO,int SCL_PIN,char SDA_GPIO,int SDA_PIN)
{
    delay_init(168);
    Gpio_Init(SCL_GPIO,SCL_PIN,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,100);
    Gpio_Init(SDA_GPIO,SDA_PIN,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,100);
    if(SCL_GPIO=='A')iic_scl_gpio=&PAout(SCL_PIN);
    if(SCL_GPIO=='B')iic_scl_gpio=&PBout(SCL_PIN);
    if(SCL_GPIO=='C')iic_scl_gpio=&PCout(SCL_PIN);
    if(SCL_GPIO=='D')iic_scl_gpio=&PDout(SCL_PIN);
    if(SCL_GPIO=='E')iic_scl_gpio=&PEout(SCL_PIN);
    if(SCL_GPIO=='F')iic_scl_gpio=&PFout(SCL_PIN);
    if(SCL_GPIO=='G')iic_scl_gpio=&PGout(SCL_PIN);
    
    if(SDA_GPIO=='A')iic_sda_gpio_out=&PAout(SDA_PIN);
    if(SDA_GPIO=='B')iic_sda_gpio_out=&PBout(SDA_PIN);
    if(SDA_GPIO=='C')iic_sda_gpio_out=&PCout(SDA_PIN);
    if(SDA_GPIO=='D')iic_sda_gpio_out=&PDout(SDA_PIN);
    if(SDA_GPIO=='E')iic_sda_gpio_out=&PEout(SDA_PIN);
    if(SDA_GPIO=='F')iic_sda_gpio_out=&PFout(SDA_PIN);
    if(SDA_GPIO=='G')iic_sda_gpio_out=&PGout(SDA_PIN);
    
    if(SDA_GPIO=='A')iic_sda_gpio_in=&PAin(SDA_PIN);
    if(SDA_GPIO=='B')iic_sda_gpio_in=&PBin(SDA_PIN);
    if(SDA_GPIO=='C')iic_sda_gpio_in=&PCin(SDA_PIN);
    if(SDA_GPIO=='D')iic_sda_gpio_in=&PDin(SDA_PIN);
    if(SDA_GPIO=='E')iic_sda_gpio_in=&PEin(SDA_PIN);
    if(SDA_GPIO=='F')iic_sda_gpio_in=&PFin(SDA_PIN);
    if(SDA_GPIO=='G')iic_sda_gpio_in=&PGin(SDA_PIN);
    
    IIC_SDA_GPIO=SDA_GPIO;
    IIC_SDA_PIN=SDA_PIN;
}

void iic_delay(void){u8 i=0;while(i--);}

void SDA_OUT()
{
    switch(IIC_SDA_GPIO)
    {
        case 'A':if(IIC_SDA_PIN<8){GPIOA->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOA->MODER|=1<<IIC_SDA_PIN*2;}break;
        case 'B':if(IIC_SDA_PIN<8){GPIOB->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOB->MODER|=1<<IIC_SDA_PIN*2;}break;
        case 'C':if(IIC_SDA_PIN<8){GPIOC->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOC->MODER|=1<<IIC_SDA_PIN*2;}break;
        case 'D':if(IIC_SDA_PIN<8){GPIOD->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOD->MODER|=1<<IIC_SDA_PIN*2;}break;
        case 'E':if(IIC_SDA_PIN<8){GPIOE->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOE->MODER|=1<<IIC_SDA_PIN*2;}break;
        case 'F':if(IIC_SDA_PIN<8){GPIOF->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOF->MODER|=1<<IIC_SDA_PIN*2;}break;
        case 'G':if(IIC_SDA_PIN<8){GPIOG->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOG->MODER|=1<<IIC_SDA_PIN*2;}break;
    }
}

void SDA_IN()
{
    switch(IIC_SDA_GPIO)
    {
        case 'A':if(IIC_SDA_PIN<8){GPIOA->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOA->MODER|=0<<IIC_SDA_PIN*2;}break;
        case 'B':if(IIC_SDA_PIN<8){GPIOB->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOB->MODER|=0<<IIC_SDA_PIN*2;}break;
        case 'C':if(IIC_SDA_PIN<8){GPIOC->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOC->MODER|=0<<IIC_SDA_PIN*2;}break;
        case 'D':if(IIC_SDA_PIN<8){GPIOD->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOD->MODER|=0<<IIC_SDA_PIN*2;}break;
        case 'E':if(IIC_SDA_PIN<8){GPIOE->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOE->MODER|=0<<IIC_SDA_PIN*2;}break;
        case 'F':if(IIC_SDA_PIN<8){GPIOF->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOF->MODER|=0<<IIC_SDA_PIN*2;}break;
        case 'G':if(IIC_SDA_PIN<8){GPIOG->MODER&=~(3<<(IIC_SDA_PIN*2));GPIOG->MODER|=0<<IIC_SDA_PIN*2;}break;
    }
}



void iic_start()
{
    SDA_OUT();
    *iic_scl_gpio=1;
    *iic_sda_gpio_out=1;
    iic_delay();
    *iic_sda_gpio_out=0;
    iic_delay();
    *iic_scl_gpio=0;
}

void iic_stop()
{
    SDA_OUT();
    *iic_scl_gpio=0;
    *iic_sda_gpio_out=0;
    iic_delay();
    *iic_scl_gpio=1;
    *iic_sda_gpio_out=1;
    iic_delay();
}


//deal=1:处理应答信号，deal=0:不处理应答信号
int iic_wait_ack(u8 deal)
{
    int time=0;
    SDA_IN();
    *iic_scl_gpio=0;
    *iic_sda_gpio_out=1;iic_delay();
    *iic_scl_gpio=1;iic_delay();
    while(time<250&&deal)
    {
        time++;
        if(*iic_sda_gpio_in==0)
        {
            *iic_scl_gpio=0;
            return 0;
        }
    }
    *iic_scl_gpio=0;
    return 1;
}

void iic_send(char str)
{
    int i;
    SDA_OUT();
    *iic_scl_gpio=0;
    for(i=0;i<8;i++)
    {
        *iic_sda_gpio_out=str>>7;
        str<<=1;
        iic_delay();
        *iic_scl_gpio=1;
        iic_delay();
        *iic_scl_gpio=0;
        iic_delay();
    }
//	*iic_scl_gpio=1;	//额外的一个时钟，不处理应答信号
//	*iic_scl_gpio=0;
}

void iic_ack()
{
    *iic_scl_gpio=0;
    *iic_sda_gpio_out=0;
    iic_delay();
    *iic_sda_gpio_out=1;
    iic_delay();
    *iic_sda_gpio_out=0;
}

void iic_nack()
{
    *iic_scl_gpio=0;
    *iic_sda_gpio_out=1;
    iic_delay();
    *iic_sda_gpio_out=1;
    iic_delay();
    *iic_sda_gpio_out=0;
}

int iic_read(u8 ack)
{
    int i;
    int str=0;
    SDA_IN();
    for(i=0;i<8;i++)
    {
        *iic_scl_gpio=0;
        iic_delay();
        *iic_scl_gpio=1;
        str<<=1;
        if(*iic_sda_gpio_in)str++;
        iic_delay();;
    }
    if(ack)iic_ack();
    else iic_nack();
    return str;
}
