#include<sys.h>
#include<init.h>
#include<mellen.h>
#include<math.h>
#include<delay.h>


volatile unsigned long*gpio_A1_out1;//A
volatile unsigned long*gpio_A1_out2;

volatile unsigned long*gpio_B1_out1;//B
volatile unsigned long*gpio_B1_out2;

volatile unsigned long*gpio_B2_out1;//B
volatile unsigned long*gpio_B2_out2;

volatile unsigned long*gpio_A2_out1;//A
volatile unsigned long*gpio_A2_out2;


//初始化
void mellen_init(u8 GPIO_A1_out1,u8 PIN_A1_out1 , u8 GPIO_A1_out2,u8 PIN_A1_out2,
                 u8 GPIO_B1_out1,u8 PIN_B1_out1 , u8 GPIO_B1_out2,u8 PIN_B1_out2,
                 u8 GPIO_B2_out1,u8 PIN_B2_out1 , u8 GPIO_B2_out2,u8 PIN_B2_out2,
                 u8 GPIO_A2_out1,u8 PIN_A2_out1 , u8 GPIO_A2_out2,u8 PIN_A2_out2)
{
    Gpio_Init(GPIO_A1_out1,PIN_A1_out1,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    Gpio_Init(GPIO_A1_out2,PIN_A1_out2,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    
    Gpio_Init(GPIO_B1_out1,PIN_B1_out1,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    Gpio_Init(GPIO_B1_out2,PIN_B1_out2,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    
    Gpio_Init(GPIO_B2_out1,PIN_B2_out1,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    Gpio_Init(GPIO_B2_out2,PIN_B2_out2,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    
    Gpio_Init(GPIO_A2_out1,PIN_A2_out1,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    Gpio_Init(GPIO_A2_out2,PIN_A2_out2,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);
    switch(GPIO_A1_out1)
    {
        case 'A':gpio_A1_out1=&PAout(PIN_A1_out1);break;
        case 'B':gpio_A1_out1=&PBout(PIN_A1_out1);break;
        case 'C':gpio_A1_out1=&PCout(PIN_A1_out1);break;
        case 'D':gpio_A1_out1=&PDout(PIN_A1_out1);break;
        case 'E':gpio_A1_out1=&PEout(PIN_A1_out1);break;
    }
    switch(GPIO_A1_out2)
    {
        case 'A':gpio_A1_out2=&PAout(PIN_A1_out2);break;
        case 'B':gpio_A1_out2=&PBout(PIN_A1_out2);break;
        case 'C':gpio_A1_out2=&PCout(PIN_A1_out2);break;
        case 'D':gpio_A1_out2=&PDout(PIN_A1_out2);break;
        case 'E':gpio_A1_out2=&PEout(PIN_A1_out2);break;
    }
    
    switch(GPIO_B1_out1)
    {
        case 'A':gpio_B1_out1=&PAout(PIN_B1_out1);break;
        case 'B':gpio_B1_out1=&PBout(PIN_B1_out1);break;
        case 'C':gpio_B1_out1=&PCout(PIN_B1_out1);break;
        case 'D':gpio_B1_out1=&PDout(PIN_B1_out1);break;
        case 'E':gpio_B1_out1=&PEout(PIN_B1_out1);break;
    }
    switch(GPIO_B1_out2)
    {
        case 'A':gpio_B1_out2=&PAout(PIN_B1_out2);break;
        case 'B':gpio_B1_out2=&PBout(PIN_B1_out2);break;
        case 'C':gpio_B1_out2=&PCout(PIN_B1_out2);break;
        case 'D':gpio_B1_out2=&PDout(PIN_B1_out2);break;
        case 'E':gpio_B1_out2=&PEout(PIN_B1_out2);break;
    }
    
    switch(GPIO_B2_out1)
    {
        case 'A':gpio_B2_out1=&PAout(PIN_B2_out1);break;
        case 'B':gpio_B2_out1=&PBout(PIN_B2_out1);break;
        case 'C':gpio_B2_out1=&PCout(PIN_B2_out1);break;
        case 'D':gpio_B2_out1=&PDout(PIN_B2_out1);break;
        case 'E':gpio_B2_out1=&PEout(PIN_B2_out1);break;
    }
    switch(GPIO_B2_out2)
    {
        case 'A':gpio_B2_out2=&PAout(PIN_B2_out2);break;
        case 'B':gpio_B2_out2=&PBout(PIN_B2_out2);break;
        case 'C':gpio_B2_out2=&PCout(PIN_B2_out2);break;
        case 'D':gpio_B2_out2=&PDout(PIN_B2_out2);break;
        case 'E':gpio_B2_out2=&PEout(PIN_B2_out2);break;
    }
    
    switch(GPIO_A2_out1)
    {
        case 'A':gpio_A2_out1=&PAout(PIN_A2_out1);break;
        case 'B':gpio_A2_out1=&PBout(PIN_A2_out1);break;
        case 'C':gpio_A2_out1=&PCout(PIN_A2_out1);break;
        case 'D':gpio_A2_out1=&PDout(PIN_A2_out1);break;
        case 'E':gpio_A2_out1=&PEout(PIN_A2_out1);break;
    }
    switch(GPIO_A2_out2)
    {
        case 'A':gpio_A2_out2=&PAout(PIN_A2_out2);break;
        case 'B':gpio_A2_out2=&PBout(PIN_A2_out2);break;
        case 'C':gpio_A2_out2=&PCout(PIN_A2_out2);break;
        case 'D':gpio_A2_out2=&PDout(PIN_A2_out2);break;
        case 'E':gpio_A2_out2=&PEout(PIN_A2_out2);break;
    }
    delay_init();
}


//麦轮状态·
void mellen_status(u8 status)
{
    switch(status)
    {
        case Stop:                  {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//停止

        case Front:                 {*gpio_A1_out1=1;*gpio_A1_out2=0 ; *gpio_B1_out1=1;*gpio_B1_out2=0 ; *gpio_B2_out1=1;*gpio_B2_out2=0 ; *gpio_A2_out1=1;*gpio_A2_out2=0;break;}//向前移动
        case Back:                  {*gpio_A1_out1=0;*gpio_A1_out2=1 ; *gpio_B1_out1=0;*gpio_B1_out2=1 ; *gpio_B2_out1=0;*gpio_B2_out2=1 ; *gpio_A2_out1=0;*gpio_A2_out2=1;break;}//向后移动
        case Left:                  {*gpio_A1_out1=0;*gpio_A1_out2=1 ; *gpio_B1_out1=1;*gpio_B1_out2=0 ; *gpio_B2_out1=1;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=1;break;}//向左移动
        case Right:                 {*gpio_A1_out1=1;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=1 ; *gpio_B2_out1=0;*gpio_B2_out2=1 ; *gpio_A2_out1=1;*gpio_A2_out2=0;break;}//向右移动
            
        case Front_Left:            {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=1;*gpio_B1_out2=0 ; *gpio_B2_out1=1;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//向左前移动
        case Front_Right:           {*gpio_A1_out1=1;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=1;*gpio_A2_out2=0;break;}//向右前移动
        case Back_Left:             {*gpio_A1_out1=0;*gpio_A1_out2=1 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=1;break;}//向左后移动
        case Back_Right:            {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=1 ; *gpio_B2_out1=0;*gpio_B2_out2=1 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//向右后移动
            
        case Front_Clockwise:       {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=1;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=1;break;}//以前轮为轴顺时针转动
        case Front_Anticlockwise:   {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=1 ; *gpio_A2_out1=1;*gpio_A2_out2=0;break;}//以前轮为轴逆时针转动
       
        case Back_Clockwise:        {*gpio_A1_out1=1;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=1 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//以后轮为轴顺时针转动
        case Back_Anticlockwise:    {*gpio_A1_out1=0;*gpio_A1_out2=1 ; *gpio_B1_out1=1;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//以后轮为轴逆时针转动
            
        case Left_Clockwise:        {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=1;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=1;*gpio_A2_out2=0;break;}//以左轮为轴顺时针转动
        case Left_Anticlockwise:    {*gpio_A1_out1=0;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=1 ; *gpio_B2_out1=0;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=1;break;}//以左轮为轴逆时针转动
            
        case Right_Clockwise:       {*gpio_A1_out1=1;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=1;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//以右轮为轴顺时针转动
        case Right_Anticlockwise:   {*gpio_A1_out1=0;*gpio_A1_out2=1 ; *gpio_B1_out1=0;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=1 ; *gpio_A2_out1=0;*gpio_A2_out2=0;break;}//以右轮为轴逆时针转动
            
        case Clockwise:             {*gpio_A1_out1=1;*gpio_A1_out2=0 ; *gpio_B1_out1=0;*gpio_B1_out2=1 ; *gpio_B2_out1=1;*gpio_B2_out2=0 ; *gpio_A2_out1=0;*gpio_A2_out2=1;break;}//原地顺时针转动
        case Anticlockwise:         {*gpio_A1_out1=0;*gpio_A1_out2=1 ; *gpio_B1_out1=1;*gpio_B1_out2=0 ; *gpio_B2_out1=0;*gpio_B2_out2=1 ; *gpio_A2_out1=1;*gpio_A2_out2=0;break;}//原地逆时针转动
            
    }
}

//麦轮全方位移动（测试）
void  mellen_move(int x,int y,int*A,int*B)
{
    float AO,BO;
    AO=(y-x)/pow(2,0.5);
    BO=(y+x)/pow(2,0.5);
    *A=(int)AO;
    *B=(int)BO;
}


//制动（通过电机反转实现）
//ms:制动时间（必须设置合理，否则可能出现反向移动）
void mellen_brake(u32 ms)
{
    u16 i;
    *gpio_A1_out1=!*gpio_A1_out1 ; *gpio_A1_out2=!*gpio_A1_out2;
    *gpio_B1_out1=!*gpio_B1_out1 ; *gpio_B1_out2=!*gpio_B1_out2;
    *gpio_B2_out1=!*gpio_B2_out1 ; *gpio_B2_out2=!*gpio_B2_out2;
    *gpio_A2_out1=!*gpio_A2_out1 ; *gpio_A2_out2=!*gpio_A2_out2;
    for(i=0;i<=ms/1000;i++)delay_ms(ms%1000);
    *gpio_A1_out1=0 ; *gpio_A1_out2=0;
    *gpio_B1_out1=0 ; *gpio_B1_out2=0;
    *gpio_B2_out1=0 ; *gpio_B2_out2=0;
    *gpio_A2_out1=0 ; *gpio_A2_out2=0;
}

