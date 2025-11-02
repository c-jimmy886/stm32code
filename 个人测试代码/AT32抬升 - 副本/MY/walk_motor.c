#include<sys.h>
#include<init.h>
#include<delay.h>
#include<math.h>
#include<walk_motor.h>

volatile unsigned long*walk_motor_gpio1;//相位1 GPIO口
volatile unsigned long*walk_motor_gpio2;//相位2 GPIO口
volatile unsigned long*walk_motor_gpio3;//相位2 GPIO口
volatile unsigned long*walk_motor_gpio4;//相位2 GPIO口

void walk_motor_init(u8 GPIO1,u8 PIN1,u8 GPIO2,u8 PIN2,u8 GPIO3,u8 PIN3,u8 GPIO4,u8 PIN4)//初始化步进电机
{
    switch(GPIO1)
    {
        case 'A':walk_motor_gpio1=&PAout(PIN1);break;
        case 'B':walk_motor_gpio1=&PBout(PIN1);break;
        case 'C':walk_motor_gpio1=&PCout(PIN1);break;
        case 'D':walk_motor_gpio1=&PDout(PIN1);break;
        case 'E':walk_motor_gpio1=&PEout(PIN1);break;
    }
    switch(GPIO2)
    {
        case 'A':walk_motor_gpio2=&PAout(PIN2);break;
        case 'B':walk_motor_gpio2=&PBout(PIN2);break;
        case 'C':walk_motor_gpio2=&PCout(PIN2);break;
        case 'D':walk_motor_gpio2=&PDout(PIN2);break;
        case 'E':walk_motor_gpio2=&PEout(PIN2);break;
    }
    switch(GPIO3)
    {
        case 'A':walk_motor_gpio3=&PAout(PIN3);break;
        case 'B':walk_motor_gpio3=&PBout(PIN3);break;
        case 'C':walk_motor_gpio3=&PCout(PIN3);break;
        case 'D':walk_motor_gpio3=&PDout(PIN3);break;
        case 'E':walk_motor_gpio3=&PEout(PIN3);break;
    }
    switch(GPIO4)
    {
        case 'A':walk_motor_gpio4=&PAout(PIN4);break;
        case 'B':walk_motor_gpio4=&PBout(PIN4);break;
        case 'C':walk_motor_gpio4=&PCout(PIN4);break;
        case 'D':walk_motor_gpio4=&PDout(PIN4);break;
        case 'E':walk_motor_gpio4=&PEout(PIN4);break;
    }
    delay_init();//初始化延时函数
    Gpio_Init(GPIO1,PIN1,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);//初始化相位1 GPIO口
    Gpio_Init(GPIO2,PIN2,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);//初始化相位2 GPIO口
    Gpio_Init(GPIO3,PIN3,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);//初始化相位3 GPIO口
    Gpio_Init(GPIO4,PIN4,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);//初始化相位4 GPIO口
    *walk_motor_gpio1=0;
    *walk_motor_gpio2=0;
    *walk_motor_gpio3=0;
    *walk_motor_gpio4=0;
}

//单四拍
u8 walk_motor_i=1;//记录当前步拍
void walk_motor_go_4(u8 FX)//发出一个脉冲信号，FX>0电机顺时针转动，FX<=0电机逆时针转动
{
    switch(walk_motor_i)
    {
        case 1:*walk_motor_gpio1=1;//1
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_i=2;
               else walk_motor_i=4;
               break;
        case 2:*walk_motor_gpio1=0;//2
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_i=3;
               else walk_motor_i=1;
               break;
        case 3:*walk_motor_gpio1=0;//3
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=1;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_i=4;
               else walk_motor_i=2;
               break;
        case 4:*walk_motor_gpio1=0;//4
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=1;
               if(FX)walk_motor_i=1;
               else walk_motor_i=3;
               break;
    }
    
}

//双四拍
u8 walk_motor_j=1;//记录当前步拍
void walk_motor_go_2_4(u8 FX)//发出一个脉冲信号，FX>0电机顺时针转动，FX<=0电机逆时针转动
{
    switch(walk_motor_j)
    {
        case 1:*walk_motor_gpio1=1;//1
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_j=2;
               else walk_motor_j=4;
               break;
        case 2:*walk_motor_gpio1=0;//2
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=1;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_j=3;
               else walk_motor_j=1;
               break;
        case 3:*walk_motor_gpio1=0;//3
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=1;
               *walk_motor_gpio4=1;
               if(FX)walk_motor_j=4;
               else walk_motor_j=2;
               break;
        case 4:*walk_motor_gpio1=1;//4
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=1;
               if(FX)walk_motor_j=1;
               else walk_motor_j=3;
               break;
    }
    
}

//八拍
u8 walk_motor_k=1;//记录当前步拍
void walk_motor_go_8(u8 FX)//发出一个脉冲信号，FX>0电机顺时针转动，FX<=0电机逆时针转动
{
    switch(walk_motor_k)
    {
        case 1:*walk_motor_gpio1=1;//1
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_k=2;
               else walk_motor_k=8;
               break;
        case 2:*walk_motor_gpio1=1;//2
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_k=3;
               else walk_motor_k=1;
               break;
        case 3:*walk_motor_gpio1=0;//3
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_k=4;
               else walk_motor_k=2;
               break;
        case 4:*walk_motor_gpio1=0;//4
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=1;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_k=5;
               else walk_motor_k=3;
               break;
        case 5:*walk_motor_gpio1=0;//5
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=1;
               *walk_motor_gpio4=0;
               if(FX)walk_motor_k=6;
               else walk_motor_k=4;
               break;
        case 6:*walk_motor_gpio1=0;//6
               *walk_motor_gpio2=1;
               *walk_motor_gpio3=1;
               *walk_motor_gpio4=1;
               if(FX)walk_motor_k=7;
               else walk_motor_k=5;
               break;
        case 7:*walk_motor_gpio1=0;//7
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=1;
               if(FX)walk_motor_k=8;
               else walk_motor_k=6;
               break;
        case 8:*walk_motor_gpio1=1;//8
               *walk_motor_gpio2=0;
               *walk_motor_gpio3=0;
               *walk_motor_gpio4=1;
               if(FX)walk_motor_k=1;
               else walk_motor_k=7;
               break;
               
    }
    
}

void walk_motor_angle(float angle,int T_ms)//控制电机转过特定角度
{
    int i=2048*fabs(angle)/360;
    int j;
    for(j=0;j<i;j++)
    {
        walk_motor_go_4(angle);
        delay_ms(T_ms);
    }
}
