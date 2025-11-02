#include<init.h>
#include<tcs3200.h>



char tcs_s2_gpio;//S2引脚GPIO
char tcs_s3_gpio;//S2引脚GPIO
u8 tcs_s2_pin;//S2引脚PIN
u8 tcs_s3_pin;//S3引脚PIN



//初始化tcs3200
//S0_GPIO,S0_PIN：S0引脚IO口
//S1_GPIO,S1_PIN：S1引脚IO口
//S2_GPIO,S2_PIN：S2引脚IO口
//S3_GPIO,S3_PIN：S2引脚IO口
//OUT_GPIO,OUT_PIN：OUT引脚IO口
//tcs_out_speed：输出速度
void tcs_init(char S0_GPIO,u8 S0_PIN,char S1_GPIO,u8 S1_PIN,char S2_GPIO,u8 S2_PIN,char S3_GPIO,u8 S3_PIN,u8 tcs_out_speed)
{
    Gpio_Init(S0_GPIO,S0_PIN,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,100);//初始化相应IO口
    Gpio_Init(S1_GPIO,S1_PIN,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,100);
    Gpio_Init(S2_GPIO,S2_PIN,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,100);
    Gpio_Init(S3_GPIO,S3_PIN,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,100);
    
    tcs_s2_gpio=S2_GPIO;//记录S2,S3对应IO口
    tcs_s2_pin=S2_PIN;
    tcs_s3_gpio=S3_GPIO;
    tcs_s3_pin=S3_PIN;
    
    switch(tcs_out_speed)//设置输出速度
    {
        case 0:  BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S0_GPIO - 'A') * 0x400 , S0_PIN)=0;//000%输出，000KHz；S0=0,S1=0
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S1_GPIO - 'A') * 0x400 , S1_PIN)=0;
                 break;
        case 2:  BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S0_GPIO - 'A') * 0x400 , S0_PIN)=0;//002%输出，012KHz；S0=0,S1=1
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S1_GPIO - 'A') * 0x400 , S1_PIN)=1;
                 break;
        case 20: BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S0_GPIO - 'A') * 0x400 , S0_PIN)=1;//020%输出，120KHz；S0=1,S1=0
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S1_GPIO - 'A') * 0x400 , S1_PIN)=0;
                 break;
        case 100:BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S0_GPIO - 'A') * 0x400 , S0_PIN)=1;//100%输出，600KHz；S0=1,S1=1
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (S1_GPIO - 'A') * 0x400 , S1_PIN)=1;
                 break;
    }
}


//RGB滤波选择
void tcs_RGB_mode(u8 R_G_B)
{
    switch(R_G_B)
    {
        case 'R':BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s2_gpio - 'A') * 0x400 , tcs_s2_pin)=0;//红光滤波；S2=0,S3=0
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s3_gpio - 'A') * 0x400 , tcs_s3_pin)=0;
                 break;
        case 'G':BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s2_gpio - 'A') * 0x400 , tcs_s2_pin)=1;//绿光滤波；S2=1,S3=1
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s3_gpio - 'A') * 0x400 , tcs_s3_pin)=1;
                 break;
        case 'B':BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s2_gpio - 'A') * 0x400 , tcs_s2_pin)=0;//蓝光滤波；S2=0,S3=1
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s3_gpio - 'A') * 0x400 , tcs_s3_pin)=1;
                 break;
        case '0':BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s2_gpio - 'A') * 0x400 , tcs_s2_pin)=1;//无滤波；S2=1,S3=0
                 BIT_ADDR(APB2PERIPH_BASE + 0x0800 + 20 + (tcs_s3_gpio - 'A') * 0x400 , tcs_s3_pin)=0;
                 break;
    }
}

