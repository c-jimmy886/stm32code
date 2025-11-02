#include<init.h>
#include<free.h>
#include<delay.h>
#include<usart.h>
#include<dma_usart.h>
#include<dm_motor.h>
#include<rm_motor.h>
#include<vesc.h>
#include<pid.h>
#include<input.h>
#include<key.h>
#include<stdio.h>
#include<ak_motor.h>
#include<chassis.h>
#include<my_library.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<state_machine.h>




#define TAKE_BALL_KEY     PEin(3)                              //接球光电开关
#define RX_LED            PDout(13)                            //接收指示灯
#define BEEP(x)           tmr_channel_value_set(TMR3,TMR_SELECT_CHANNEL_3,50*x)//蜂鸣器开关
#define WHEEL_VESC_Set(x) {wheel_vesc_speed1=x;wheel_vesc_speed2=x;wheel_vesc_speed3=x;}//VESC摩擦轮速度设置


#define UKEY                      0xFF//按键松开
#define FINISH                    0xFE//事件完成
#define LIFT_UP                   'e'//升降抬升
#define LIFT_DOWN                 'f'//升降下降
#define PREP_DRIBBLE              'Z'//准备运球
#define DRIBBLE                   'b'//运球
#define PREP_FILE                 'g'//准备发射
#define FILE                      'h'//发射
#define RECOVER                   'H'//机构复位
#define TAKE_BALL                 'k'//接球
#define PASS                      'c'//传球
#define TAKE_AIM                  'y'//对准篮筐
#define HALF_AUTO                 'N'//半自动模式


u8 file_data_close=0;//发射数据选择标志
u8 sys_timeout=0;//系统超时记时（用于判断上层是否断电，如果上层断电，超过一定时间没有接收到如何电机的CAN消息，则单片机自动复位）
u8 WHEEL_VESC_LOCK=0;//VESC摩擦发射轮锁
u8 Rx_event=0;//接收事件（上位机）
u8 half_auto=0;//半自动模式
u8 auto_file=0;//自动发射模式
char com_buf[21]={0};//遥控数据
int FILE_ERR= -400;//发射速度偏差，默认-400
int wheel_vesc_speed1=0;//摩擦轮1速度
int wheel_vesc_speed2=0;//摩擦轮2速度
int wheel_vesc_speed3=0;//摩擦轮3速度
uint16_t wheel_vesc_speed;//3摩擦轮速度
uint16_t wheel_vesc_err=0;//3摩擦轮差速
uint16_t last_wheel_vesc_speed;//上一次摩擦轮速度
uint16_t last_wheel_vesc_err;//上一次摩擦轮差速
float classis_speed;//底盘速度
float distance=0;//篮筐距离


RM_RX_DATA CAN2_RM_BUF[2]={{0x201,-1},{0x202,-1}};//RM电机接收数据
VESC_DATA_BUF VESC_BUF[3]={{1},{2},{3}};//VESC接收数据

//推射2006数据结构体
struct PUSH_MOTOR
{
    int       *Now_Place;//当前位置
    int16_t   *Now_Speed;//当前速度
    int16_t   *Now_Current;//当前电流
    int       Zero_Place;//零点位置
    int       Aim_Place;//目标位置
    int       Aim_Speed;//目标速度
    int       Max_Current;//最大电流
}PushMotor={&CAN2_RM_BUF[1].place,&CAN2_RM_BUF[1].vel,&CAN2_RM_BUF[1].cur,0,2000,500,5000};
//直接给结构体赋值，把can的接收数据直接赋值上去
#define PushMotor_Wait (!(*PushMotor.Now_Place-PushMotor.Zero_Place>PushMotor.Aim_Place-100 && *PushMotor.Now_Place-PushMotor.Zero_Place<PushMotor.Aim_Place+100))//等待电机到达指定位置





//发射数据
struct FILE_DATA
{
    float distance;
    
    int speed_file;
    int speed_err;
}File_Data1[]={//数据1
    {0.9,4600*2,16000},
    {1.3,5300*2,16000},
    {1.7,7300*2,16000},
    {2.1,8400*2,16000},
    {2.4,9300*2,16000},
    {2.5,9600*2,16000},
    {2.6,9900*2,16000},
    {2.7,10100*2,16000},
    {2.8,10250*2,16000},
    {2.9,10400*2,16000},
    {3.0,10550*2,16000},
    {3.1,10700*2,16000},
    {3.5,11500*2,16000},
},
File_Data2[]={//数据2
    {0.8,700*2,30000},
    {0.9,850*2,30000},
    {1.0,1000*2,30000},

    {1.0,4600*2,16000},
    {1.3,5300*2,16000},
    {1.7,7300*2,16000},
    {2.1,8400*2,16000},
    {2.4,9300*2,16000},
    {2.5,9600*2,16000},
    {2.6,9900*2,16000},
    {2.7,10100*2,16000},
    {2.8,10250*2,16000},
    {2.9,10400*2,16000},
    {3.0,10550*2,16000},
    {3.1,10700*2,16000},
    {3.5,11500*2,16000},
}
;


//距离线性计算
long long DistanceLinearityCalulation()
{
    u8 i;
    u8 len;
    float K;
    float b;
    
    if(!file_data_close)//选择数据1
    {
        len=sizeof(File_Data1)/sizeof(File_Data1[0]);
        
        if(distance<=0 || distance>4)return 12000;//传入距离小于0，不符合实际情况，直接返回0
        
        if(distance<File_Data1[0].distance)i=0;//传入距离小于数据表中的最小距离
        else if(distance>File_Data1[len-1].distance)i=len-2;//传入距离小于数据表中的最大距离
        else
        {
            for(i=0;i<len-1;i++)//循环查找数据表
            {
                if(File_Data1[i+1].distance>=distance && File_Data1[i].distance<=distance)break;//找到与传入距离左右相邻的两个数据点
            }
        }
        
        K= (File_Data1[i+1].speed_file-File_Data1[i].speed_file)/(File_Data1[i+1].distance-File_Data1[i].distance);//根据那两个数据点计算一元一次函数的 K 和 b
        b= File_Data1[i].speed_file-K*File_Data1[i].distance+FILE_ERR;
        return ((long long)(K*distance+b)|((long long)File_Data1[i].speed_err<<32));//根据函数和传入距离计算当前的转速并返回（返回值的0~31bit为转速，32~63bit为差速）
    }
    else//选择数据2
    {
        len=sizeof(File_Data2)/sizeof(File_Data2[0]);
        
        if(distance<=0 || distance>4)return 12000;//传入距离小于0，不符合实际情况，直接返回0
        
        if(distance<File_Data2[0].distance)i=0;//传入距离小于数据表中的最小距离
        else if(distance>File_Data2[len-1].distance)i=len-2;//传入距离小于数据表中的最大距离
        else
        {
            for(i=0;i<len-1;i++)//循环查找数据表
            {
                if(File_Data2[i+1].distance>=distance && File_Data2[i].distance<=distance)break;//找到与传入距离左右相邻的两个数据点
            }
        }
        
        K= (File_Data2[i+1].speed_file-File_Data2[i].speed_file)/(File_Data2[i+1].distance-File_Data2[i].distance);//根据那两个数据点计算一元一次函数的 K 和 b
        b= File_Data2[i].speed_file-K*File_Data2[i].distance+FILE_ERR;
        return ((long long)(K*distance+b)|((long long)File_Data2[i].speed_err<<32));//根据函数和传入距离计算当前的转速并返回（返回值的0~31bit为转速，32~63bit为差速）
    }
    
}



/*******************************************************************条件延时*******************************************************************/


int delay_5ms=0;//延时计数
//condition：条件
//MaxDelay：最大延时
//每5ms进行一次判断，符合条件并且不超过最大延时，则继续延时，否则结束返回0
//当MaxDelay=0xFFFF时，不存在最大延时，即最大延时无限
#define CON_DELAY_5MS(condition,MaxDelay)                      \
{                                                              \
    delay_5ms=0;                                               \
    while(condition&&(delay_5ms<MaxDelay || MaxDelay==0xFFFF)) \
    {                                                          \
        delay_5ms++;                                           \
        vTaskDelay(5);                                         \
    }                                                          \
}

/***********任务句柄***********/

void*Communiction_Task_Handler;
void*Hand_Task_Handler;
void*Test_Task_Handler;
void*ActionStart_Task_Handler;
void*MotorDrive1_Task_Handler;
void*MotorDrive2_Task_Handler;
void*Auto_Task_Handler;

/*****************************/


/*==================================================================任务函数===================================================================*/

/*=<<通讯任务>>=*/
void Communiction_Task()
{
    union FloatToByte
    {
        float Float;
        char Byte[4];
    }floatTobyte;
    
    u8 wheel_speed_flag=0;//摩擦轮速度确认标志（不知道什么原因（可能是串口不稳定），有时会接收到一帧错误的数据会导致摩擦轮疯转，所以要确认几帧数据相同才可以正常使用）
    u8 wheel_err_flag=0;//摩擦轮差速确认标志
    int wheel_speed_buffer;//摩擦轮上一个速度缓存
    int wheel_err_buffer;//摩擦轮上一个差速缓存
    
    u8 timer=0;//数据流计时（用于判断蓝牙是否断连）
    TickType_t lasttick;
    lasttick = xTaskGetTickCount();
    while(1)
    {
        if(dma_rx(usart5,com_buf,20))//接收到数据
        {
            if(com_buf[0]=='['&&com_buf[19]==']')//数据包正确
            {
                //解析摩擦轮速度
                if(!WHEEL_VESC_LOCK)//摩擦轮未锁定
                {
                    if(wheel_speed_buffer==((uint16_t)com_buf[3]|((uint16_t)com_buf[4]<<8)) && wheel_speed_flag<2)wheel_speed_flag++;//本次接收与缓存的一样，确认度+1
                    else {wheel_speed_buffer=((uint16_t)com_buf[3]|((uint16_t)com_buf[4]<<8));wheel_speed_flag=0;}//反之，确认度清0，并将当前数据记录到缓冲区
                    if(last_wheel_vesc_speed!=((uint16_t)com_buf[3]|((uint16_t)com_buf[4]<<8)) && wheel_speed_flag==2)//接收的摩擦轮速度与上一次接收的不一样
                    {
                        last_wheel_vesc_speed=(uint16_t)com_buf[3]|((uint16_t)com_buf[4]<<8);//刷新摩擦轮速度数据
                        if(!half_auto)//手动模式下，立刻加载新的速度数据
                        {
                            wheel_vesc_speed=last_wheel_vesc_speed*2;
                            wheel_vesc_err=last_wheel_vesc_err;
                            
                            wheel_vesc_speed1=wheel_vesc_speed;
                            wheel_vesc_speed2=wheel_vesc_speed+wheel_vesc_err;
                            wheel_vesc_speed3=wheel_vesc_speed+wheel_vesc_err;
                        }
                    }
                    
                    //解析摩擦轮差速
                    if(wheel_err_buffer==((uint16_t)com_buf[1]|((uint16_t)com_buf[2]<<8)) && wheel_err_flag<2)wheel_err_flag++;//本次接收与缓存的一样，确认度+1
                    else {wheel_err_buffer=((uint16_t)com_buf[1]|((uint16_t)com_buf[2]<<8));wheel_err_flag=0;}//反之，确认度清0，并将当前数据记录到缓冲区
                    if(last_wheel_vesc_err!=((uint16_t)com_buf[1]|((uint16_t)com_buf[2]<<8)) && wheel_err_flag==2)//接收的摩擦轮差速与上一次接收的不一样
                    {
                        last_wheel_vesc_err=(uint16_t)com_buf[1]|((uint16_t)com_buf[2]<<8);//刷新摩擦轮差速
                        if(!half_auto)//手动模式下，立刻加载新的差速数据
                        {
                            wheel_vesc_speed=last_wheel_vesc_speed*2;
                            wheel_vesc_err=last_wheel_vesc_err;
                            
                            wheel_vesc_speed2=wheel_vesc_speed+wheel_vesc_err;
                            wheel_vesc_speed3=wheel_vesc_speed+wheel_vesc_err;
                        }
                    }
                }
                
                //解析是否开启自动发射
                if(half_auto && ((uint16_t)com_buf[1]|((uint16_t)com_buf[2]<<8))>20000)auto_file=1;//半自动模式下，差速设置大于20000则开启自动发射
                else auto_file=0;//否则关闭自动发射
                
                //解析状态数据
                if(com_buf[5]!='R'&&com_buf[5]!='L')Rx_event=com_buf[5];//获取状态数据
                
                //解析底盘速度
                memcpy(floatTobyte.Byte,(char*)com_buf+6,4);//底盘速度
                classis_speed=floatTobyte.Float;
                
                //解析篮筐距离
                memcpy(floatTobyte.Byte,(char*)com_buf+14,4);//距离
                distance=floatTobyte.Float;
                
                if(com_buf[18]==HALF_AUTO)half_auto=1;
                else if(com_buf[18]=='M')half_auto=0;
                
                //dma_printf("%d,%c,%f,%f,%f\n",wheel_vesc_speed1,Rx_event,posx,posy,distance);
                
                RX_LED=!RX_LED;//LED闪烁，提示接收到正确数据
                timer=0;//数据流计时清零
            }
        }
        
        if(timer>50)//当超过50*10ms没接收到数据
        {
            //BEEP(1);//蜂鸣器响起
            PDout(15)=1;//数据流中断指示灯亮起
            RX_LED=0;
        }
        else 
        {
            timer++;
            BEEP(0);
            PDout(15)=0;
        }
        
        vTaskDelayUntil(&lasttick, 10);
    }
}





/*=<<手动任务>>=*/
void Hand_Task()
{
    KEY key_add,key_sub;//速度增加按键，速度减小按键
    TickType_t lasttick;
    lasttick = xTaskGetTickCount();
    key_init(&key_add,KEY_MODE_DOWN);//初始化速度增加按键（按下触发模式）
    key_init(&key_sub,KEY_MODE_DOWN);//初始化速度减小按键（按下触发模式）
    while(1)
    {
        if(half_auto)//半自动模式下
        {
            if(key_scan(&key_add,Rx_event=='+') && wheel_vesc_speed<=59900)wheel_vesc_speed+=100;//按下'+'，在当前速度上+100
            if(key_scan(&key_sub,Rx_event=='-') && wheel_vesc_speed>=100)wheel_vesc_speed-=100;//按下'-'，在当前速度上-100
        }
        
        if(Rx_event=='1')FILE_ERR=-400;//采用-400速度偏差（备管）
        if(Rx_event=='2')FILE_ERR=900;//采用900速度偏差（试运行）
        if(Rx_event=='3')file_data_close=0;//采用数据1（距离远）
        if(Rx_event=='4')file_data_close=1;//采用数据2（距离近）
        
        vTaskDelayUntil(&lasttick, 10);
    }
}


#if 1
/*********************************************************************************************************************************************/
//状态机任务函数

/*空闲状态函数*/
void idle_state_task_begin()//空闲状态开始函数
{
    CON_DELAY_5MS(Rx_event==RECOVER,0xFFFF);//等待复位键松开
}
void idle_state_task_run()//空闲运行任务
{
    if(Rx_event==TAKE_AIM && half_auto)Rx_event=PREP_FILE;//半自动模式下，按下对准篮筐直接进入准备发射
}

/*准备发射状态函数*/
void prep_file_state_task_begin()//准备发射开始任务
{
    //dma_printf("准备发射\n");
    wheel_vesc_speed1=wheel_vesc_speed;//设置所有摩擦轮速度
    wheel_vesc_speed2=wheel_vesc_speed+wheel_vesc_err;//设置下面轮的差速
    wheel_vesc_speed3=wheel_vesc_speed+wheel_vesc_err;
    
    CON_DELAY_5MS(!(Rx_event==UKEY || Rx_event==TAKE_AIM),0xFFFF);//等待松开按键
}
void prep_file_state_task_run()//准备发射运行任务
{
    static long long num;
    if(Rx_event==TAKE_AIM)//按下对准篮筐或在自动模式下手动设置的转速少于5000
    {
        num=DistanceLinearityCalulation();//根据离篮筐距离以及数据表计算发射速度
        
        //num的0~31bit为基准速度，32~63bit为差速
        wheel_vesc_speed=num&0xFFFFFFFF;//解算出轮速度
        wheel_vesc_err=(num>>32);//解算出差速
        
        WHEEL_VESC_Set(20000);
    }
    
    if(Rx_event!=TAKE_AIM)//松开对准篮筐，加载最后等到的速度
    {
        wheel_vesc_speed1=wheel_vesc_speed;
        wheel_vesc_speed2=wheel_vesc_speed+wheel_vesc_err;
        wheel_vesc_speed3=wheel_vesc_speed+wheel_vesc_err;
        
        if(auto_file && half_auto && classis_speed<0.05f)Rx_event=FILE;//自动模式下，开启自动发射以及底盘速度小与0.05，则切换成发射状态
    }
    
    
}


/*发射状态函数*/
void file_state_task_run()//发射运行任务
{
    u8 i=30;
    int aim_speed1=wheel_vesc_speed1;//记录当前目标速度
    int aim_speed2=wheel_vesc_speed2;
    int aim_speed3=wheel_vesc_speed3;
    
    //dma_printf("发射\n");
    CON_DELAY_5MS(!(-VESC_RPM(VESC_BUF[0])>wheel_vesc_speed1-200 && -VESC_RPM(VESC_BUF[1])>wheel_vesc_speed2-200 && -VESC_RPM(VESC_BUF[2])>wheel_vesc_speed3-200),200);//等待确保摩擦轮转速接近目标转速
    PushMotor.Max_Current=10000;//设置推球电机最大电流为10000
    PushMotor.Aim_Place=-100000;//推球电机推动
    CON_DELAY_5MS(!(-VESC_RPM(VESC_BUF[0])<wheel_vesc_speed1-500 && -VESC_RPM(VESC_BUF[1])<wheel_vesc_speed2-500 && -VESC_RPM(VESC_BUF[2])<wheel_vesc_speed3-500),100);//电机发生掉速，说明球已经接触到摩擦轮
    while(i--)//在球接触摩擦轮期间进行速度规划，确保三轮掉速一致
    {
        wheel_vesc_speed1-=aim_speed1/2/20;//20
        wheel_vesc_speed2-=aim_speed2/2/20;
        wheel_vesc_speed3-=aim_speed3/2/20;
        vTaskDelay(5);
    }
    
    CON_DELAY_5MS(PushMotor_Wait,200);//等待推球电机推动到指定位置
    if(half_auto)//半自动模式下
    {
        WHEEL_VESC_Set(-4000);//电机直接反转
        PushMotor.Aim_Place=0;//推球电机后退
    }
    CON_DELAY_5MS(TAKE_BALL_KEY,60);//等待光电门信号消失
    CON_DELAY_5MS(!(Rx_event==UKEY),0xFFFF);//等待松开按键
}

/*接球状态函数*/
void take_ball_state_task_begin()//接球开始任务
{
    //dma_printf("开始接球\n");
    PushMotor.Max_Current=10000;//设置推球电机最大电流为10000
    PushMotor.Aim_Place=0;//推射后退
    WHEEL_VESC_Set(-4000);//设置摩擦轮速度为-4000
    CON_DELAY_5MS(!(-VESC_RPM(VESC_BUF[0])<-3800 && -VESC_RPM(VESC_BUF[1])<-3800 && -VESC_RPM(VESC_BUF[2])<-3800),100);//等待电机加速
    CON_DELAY_5MS(PushMotor_Wait,200);//等待推球电机下降到最低
    WHEEL_VESC_LOCK=1;//锁定摩擦轮速度设置
    PushMotor.Max_Current=0;//设置推球电机最大电流为0
    CON_DELAY_5MS(!(Rx_event==UKEY),0xFFFF);//等待松开按键
}
void take_ball_state_task_run()//接球运行任务
{ 
    if((-VESC_RPM(VESC_BUF[0])>-3800 && -VESC_RPM(VESC_BUF[1])>-3800 && -VESC_RPM(VESC_BUF[2])>-3800))//球接触到摩擦轮（3个轮的转速下降，说明球已经接触到）
    {
        CON_DELAY_5MS(1,50);//等待球进入发射仓
        if(half_auto)
        {
            Rx_event=PREP_FILE;//半自动模式下，直接进入准备发射
        }
        else Rx_event=FINISH;//手动模式切换到空闲状态
    }
    else if(TAKE_BALL_KEY)//光电门触发说明球以及进入发射仓
    {
        if(half_auto)
        {
            Rx_event=PREP_FILE;//半自动模式下，直接进入准备发射
        }
        else Rx_event=FINISH;//手动模式切换到空闲状态
    }
    
    if(Rx_event==TAKE_AIM && half_auto)Rx_event=PREP_FILE;//半自动模式下，按下对准篮筐直接进入准备发射
}
void take_ball_state_task_end()//接球结束任务
{
    //dma_printf("结束接球\n");
    WHEEL_VESC_Set(0);//设置摩擦轮速度为0
    WHEEL_VESC_LOCK=0;//解锁摩擦轮速度设置
    CON_DELAY_5MS(!(Rx_event==UKEY || Rx_event==TAKE_AIM),0xFFFF);//等待松开按键或按下对准篮筐
}

/*运球状态函数*/
void dribble_state_task_begin()//运球开始任务
{
    //运球开始
    wheel_vesc_speed1=-2000;//设置三摩擦轮速度
    wheel_vesc_speed2=27500;
    wheel_vesc_speed3=27500;
    WHEEL_VESC_LOCK=1;//锁定摩擦轮速度设置
    CON_DELAY_5MS(Rx_event!=UKEY,0xFFFF);//等待松开按键
}
void dribble_state_task_run()//运球运行任务
{
    u8 i=40;
    if(Rx_event==DRIBBLE)//按下运球
    {
        CON_DELAY_5MS(!(-VESC_RPM(VESC_BUF[1])>wheel_vesc_speed2-200 && -VESC_RPM(VESC_BUF[2])>wheel_vesc_speed3-200),200);//等待摩擦轮加速到指定速度
        PushMotor.Max_Current=10000;//设置推球电机最大电流为10000
        PushMotor.Aim_Place=-100000;//把球推上去
        CON_DELAY_5MS(!(-VESC_RPM(VESC_BUF[1])<wheel_vesc_speed2-500 && -VESC_RPM(VESC_BUF[2])<wheel_vesc_speed3-500),100);//等待球接触摩擦轮（摩擦轮发生掉速，说明球已经接触电机）
        while(i--)//在球接触摩擦轮期间进行减速规划，确保三轮掉速一致
        {
          wheel_vesc_speed2-=1000;
          wheel_vesc_speed3-=1000;
          vTaskDelay(5);
        }
        WHEEL_VESC_Set(-5000);//设置摩擦轮速度为-5000
        CON_DELAY_5MS(TAKE_BALL_KEY,200);
        Rx_event=TAKE_BALL;
    }
}

/*机构复位状态函数*/
u8 PushMotor_Init_Flag=0;//推射初始化完成标记
void recover_state_task_begin()
{
    PushMotor_Init_Flag=0;//推射标记未初始化    //dma_printf("机构复位\n");
    
    PushMotor.Aim_Place=300000;//给推射2006一个最大位置，让它能够碰撞限位
    PushMotor.Aim_Speed=1000;//设置移动速度为1000
    PushMotor.Max_Current=5000;//设置最大电流为5000
}
void recover_state_task_run()
{
    if(*PushMotor.Now_Current>4500&&PushMotor_Init_Flag==0)//推射电机未初始化而且与限位发生碰撞，电流增大
    {
        PushMotor_Init_Flag=1;//标志推射电机初始化完成
        PushMotor.Aim_Place=0;//设置推射电机初始位置为100000
        PushMotor.Zero_Place=*PushMotor.Now_Place-6000;//设置推射电机零点为当前位置+6000
        PushMotor.Max_Current=10000;//设置推射电机最大电流为10000
        PushMotor.Aim_Speed=10000;//设置目标速度为9000
    } 

    if(PushMotor_Init_Flag)//所以电机初始化完成
    {
        CON_DELAY_5MS(Rx_event!=UKEY,0xFFFF);//等待所以按键松开
        Rx_event=FINISH;
    }
}

/*********************************************************************************************************************************************/
#endif

STATE_MISS    idle_state , prep_file_state , file_state , take_ball_state , dribble_state , recover_state;//空闲状态，准备发射状态，发射状态，接球状态，运球状态，复位状态
STATE_Handle  Sta_Handler;//状态机句柄

/*=<<动作状态任务>>=*/
void ActionStart_Task()
{
    TickType_t lasttick;
    lasttick = xTaskGetTickCount();
    
    StateCreate(&idle_state,idle_state_task_begin,idle_state_task_run,NULL,5,                                       //创建空闲状态：开始函数idle_state_task_begin，任务函数idle_state_task_run，无结束函数，下一状态数量5
                NEXT_STATE_MISS_COM(PREP_FILE,&prep_file_state),                                                    //下一状态1:切换事件PREP_FILE,状态prep_file_state
                NEXT_STATE_MISS_COM(FILE,&prep_file_state),                                                         //下一状态2:切换事件FILE,状态prep_file_state
                NEXT_STATE_MISS_COM(TAKE_BALL,&take_ball_state),                                                    //下一状态3:切换事件TAKE_BALL,状态take_ball_state
                NEXT_STATE_MISS_COM(PREP_DRIBBLE,&dribble_state),                                                   //下一状态4:切换事件PREP_DRIBBLE,状态dribble_state
                NEXT_STATE_MISS_COM(RECOVER,&recover_state));                                                       //下一状态5:切换事件FINISH,状态recover_state
                
    StateCreate(&prep_file_state,prep_file_state_task_begin,prep_file_state_task_run,NULL,4,                        //创建准备发射状态：开始函数prep_file_state_task_switch，任务函数prep_file_state_task_run，无结束函数，下一状态数量4
                NEXT_STATE_MISS_COM(FILE,&file_state),                                                              //下一状态1:切换事件FILE,状态file_state
                NEXT_STATE_MISS_COM(TAKE_BALL,&take_ball_state),                                                    //下一状态2:切换事件TAKE_BALL,状态take_ball_state
                NEXT_STATE_MISS_COM(PREP_DRIBBLE,&dribble_state),                                                   //下一状态3:切换事件PREP_DRIBBLE,状态dribble_state
                NEXT_STATE_MISS_COM(RECOVER,&idle_state));                                                          //下一状态4:切换事件FINISH,状态idle_state
                
    StateCreate(&file_state,NULL,file_state_task_run,NULL,1,                                                        //创建发射状态：无开始函数，任务函数file_state_task，无结束函数，下一状态数量1
                NEXT_STATE_MISS_COM(UKEY,&take_ball_state));                                                        //下一状态1:切换事件UKEY,状态idle_state
                
    StateCreate(&take_ball_state,take_ball_state_task_begin,take_ball_state_task_run,take_ball_state_task_end,5,    //创建接球状态：开始函数take_ball_state_task_begin，任务函数take_ball_state_task_run，结束函数take_ball_state_task_end，下一状态数量5
                NEXT_STATE_MISS_COM(PREP_FILE,&prep_file_state),                                                    //下一状态1:切换事件PREP_FILE,状态prep_file_state
                NEXT_STATE_MISS_COM(FILE,&prep_file_state),                                                         //下一状态2:切换事件FILE,状态prep_file_state
                NEXT_STATE_MISS_COM(FINISH,&idle_state),                                                            //下一状态3:切换事件TAKE_BALL,状态idle_state
                NEXT_STATE_MISS_COM(PREP_DRIBBLE,&dribble_state),                                                   //下一状态4:切换事件PREP_DRIBBLE,状态dribble_state
                NEXT_STATE_MISS_COM(RECOVER,&idle_state));                                                          //下一状态5:切换事件FINISH,状态idle_state
                
    StateCreate(&dribble_state,dribble_state_task_begin,dribble_state_task_run,NULL,5,                              //创建运球状态：开始函数dribble_state_task_begin，任务函数dribble_state_task_run，无结束函数，下一状态数量5
                NEXT_STATE_MISS_COM(PREP_FILE,&prep_file_state),                                                    //下一状态1:切换事件PREP_FILE,状态prep_file_state
                NEXT_STATE_MISS_COM(FILE,&prep_file_state),                                                         //下一状态2:切换事件FILE,状态prep_file_state
                NEXT_STATE_MISS_COM(TAKE_BALL,&take_ball_state),                                                    //下一状态3:切换事件TAKE_BALL,状态take_ball_state
                NEXT_STATE_MISS_COM(PREP_DRIBBLE,&idle_state),                                                      //下一状态4:切换事件PREP_DRIBBLE,状态idle_state
                NEXT_STATE_MISS_COM(RECOVER,&idle_state));                                                          //下一状态5:切换事件FINISH,状态idle_state
                
    StateCreate(&recover_state,recover_state_task_begin,recover_state_task_run,NULL,1,                              //创建复位状态：开始函数recover_state_task_begin，任务函数recover_state_task_run，无结束函数，下一状态数量，1
                NEXT_STATE_MISS_COM(FINISH,&take_ball_state));                                                      //下一状态1:切换事件FINISH,状态take_ball_state
                
    State_Handle_Init(Sta_Handler,recover_state);  //初始化状态机句柄，设置首个状态为idle_state
    
    
    while(1)
    {
        StateRun(&Rx_event,0xFF,&Sta_Handler);//状态机运行：当前状态事件Rx_event，状态事件掩码0xFF，状态机句柄Sta_Handler
        vTaskDelayUntil(&lasttick, 10);//每10ms运行一次
    }
}




/*=<<电机驱动任务1>>=*/
void MotorDrive1_Task() 
{
    TickType_t lasttick;
    lasttick = xTaskGetTickCount();
    int wheel1_speed;
    int wheel2_speed;
    int wheel3_speed;
    
    while(1)
    {
        //因为在高速时电机减速的反向电动势较大，会使VESC冒红，所以在速度超过35000时的减速要进行缓慢减速
        if(wheel1_speed>35000 && wheel_vesc_speed1<wheel1_speed)wheel1_speed-=200;
        else wheel1_speed=wheel_vesc_speed1;
        if(wheel2_speed>35000 && wheel_vesc_speed2<wheel2_speed)wheel2_speed-=200;
        else wheel2_speed=wheel_vesc_speed2;
        if(wheel3_speed>35000 && wheel_vesc_speed3<wheel3_speed)wheel3_speed-=200;
        else wheel3_speed=wheel_vesc_speed3;
        
        VESC_Tx(1,CAN_PACKET_SET_RPM,-wheel1_speed);
        VESC_Tx(2,CAN_PACKET_SET_RPM,-wheel2_speed);
        VESC_Tx(3,CAN_PACKET_SET_RPM,-wheel3_speed);
        
        vTaskDelayUntil(&lasttick, 4);
    }
}



/*=<<电机驱动任务2>>=*/
void MotorDrive2_Task()
{
    int U1;
    struct PID pids1,pidp1;
    
    TickType_t lasttick;
    lasttick = xTaskGetTickCount();
    
    pid_init(&pids1,10.0,0.1,4);
    pid_init(&pidp1,0.8,0,0.1);
    
    
    while(1)
    {
//        double Pid_PlaceSerialSpeed(struct PID*pid_place,double aim_place,double now_place,
//		struct PID*pid_speed,double max_speed,double now_speed,double max_output,float Ka);
			
        U1=Pid_PlaceSerialSpeed(&pidp1, PushMotor.Aim_Place+PushMotor.Zero_Place,
		*PushMotor.Now_Place,&pids1,PushMotor.Aim_Speed,*PushMotor.Now_Speed,PushMotor.Max_Current,10000);
        
        RM_Tx(0x200,0,U1,0,0);
        
        vTaskDelayUntil(&lasttick, 4);
    }
}



/*=<<测试任务>>=*/
void Test_Task()
{ 
    char buf[16];
    char qt_buf[8]={[0] = '[' , [5] = ']'};
    
    while(1)
    {
        if(dma_rx(7,buf,13))//接收vofa+的信息
        {
            if(buf[0]=='w'&&buf[1]=='h'&&buf[2]=='e'&&buf[3]=='e'&&buf[4]=='l'&& WHEEL_VESC_LOCK==0)
            {
                if(buf[5]=='1')wheel_vesc_speed1=atoi(buf+5+2);
                if(buf[5]=='2')wheel_vesc_speed2=atoi(buf+5+2);
                if(buf[5]=='3')wheel_vesc_speed3=atoi(buf+5+2);
            }
        }
        
        qt_buf[1]=wheel_vesc_err;
        qt_buf[2]=wheel_vesc_err>>8;
        
        qt_buf[3]=wheel_vesc_speed;
        qt_buf[4]=wheel_vesc_speed>>8;
        dma_tx(5,qt_buf,6);
        
        PDout(14)=!PDout(14);
            
        vTaskDelay(100);
    }
}



/*=<<自动任务》=*/
void Auto_Task()
{
    u8 stop_time;
    long long num;
    
    TickType_t lasttick;
    lasttick = xTaskGetTickCount();
    
    
    
    while(1)
    {
        
        if(half_auto)
        {
            if(stop_time<3)   
            {
                num=DistanceLinearityCalulation();//根据离篮筐距离以及数据表计算发射速度
                
                //num的0~31bit为基准速度，32~63bit为差速
                wheel_vesc_speed=num&0xFFFFFFFF;//解算出轮速度
                wheel_vesc_err=(num>>32);//解算出差速
            }
            
            if(classis_speed>0.05f)stop_time=0;
            else if(stop_time<10)stop_time++;
        }
        
        
        if(sys_timeout>=100)nvic_system_reset();//超过1000ms没有接收到任何CAN消息，说明上层已经断电，直接复位单片机
        sys_timeout++;
        
        dma_printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f\n",wheel_vesc_speed1,wheel_vesc_speed2,wheel_vesc_speed3,
                                                        -VESC_RPM(VESC_BUF[0]),-VESC_RPM(VESC_BUF[1]),-VESC_RPM(VESC_BUF[2]),
                                                        (int16_t)VESC_CUR(VESC_BUF[0]),(int16_t)VESC_CUR(VESC_BUF[1]),(int16_t)VESC_CUR(VESC_BUF[2]),
                                                        (-VESC_RPM(VESC_BUF[0])-VESC_RPM(VESC_BUF[1])-VESC_RPM(VESC_BUF[2]))/3,distance,classis_speed);
        
        vTaskDelayUntil(&lasttick, 10);
    }
}


/*============================================================================================================================================*/





#define TASK_NUM 7//任务数量
//任务信息
Task_Mess R1_Task[TASK_NUM]={
{(TaskFunction_t)Test_Task,&Test_Task_Handler,256,1,"Test_Task",NULL},//测试任务
{(TaskFunction_t)Hand_Task,&Hand_Task_Handler,256,2,"Hand_Task",NULL},//手动任务
{(TaskFunction_t)ActionStart_Task,&ActionStart_Task_Handler,256,3,"ActionStart_Task",NULL},//运动状态任务
{(TaskFunction_t)Auto_Task,&Auto_Task_Handler,256,4,"Auto_Task_Task",NULL},//自动任务
{(TaskFunction_t)MotorDrive1_Task,&MotorDrive1_Task_Handler,256,5,"MotorDrive1_Task",NULL},//电机驱动任务1
{(TaskFunction_t)Communiction_Task,&Communiction_Task_Handler,128,6,"Communiction_Task",NULL},//通讯任务
{(TaskFunction_t)MotorDrive2_Task,&MotorDrive2_Task_Handler,256,7,"MotorDrive2_Task",NULL},//电机驱动任务2
};


void CAN1_IRQ(uint32_t id,u8*data)//CAN1中断接收回调函数
{
    vesc_can_rx(id,data);//VESC数据接收
    sys_timeout=0;
}

void CAN2_IRQ(uint32_t id,u8*data)//CAN2中断接收回调函数
{
    RM_Rx(id,data);//大疆电机数据接收
    sys_timeout=0;
}




int main()
{
    KEY key;
    
//    delay_init();
//    key_init(&key,KEY_MODE_DOWN);
//    //初始化GPIO口
//    GPIO_init('D',13,OUT_PP);PDout(13)=0;
//    GPIO_init('D',14,OUT_PP);PDout(14)=1;
//    GPIO_init('D',15,OUT_PP);PDout(15)=0;
//    GPIO_init('C',3,IPU);
//    GPIO_init('E',3,IPD);
    dma_uart_init(1,115200);
    dma_uart_init(7,115200);//调试串口
//    dma_uart_init(usart3,115200);//蓝牙串口
//    dma_uart_init(usart4,115200);//串口屏
//    dma_uart_init(usart5,19200);//底盘串口
    
    Tim_Init(3,100,1200,Up);//初始化蜂鸣器定时器
    Pwm_Init('B',0,1,High,TMR3,3);//初始化蜂鸣器pwm
    BEEP(0);//关闭蜂鸣器
    
    Tim_Init(4,2000,2400,Up);//初始化定时器4，重装载值2000，预分频值2400，向上计数
    Pwm_Init('B',6,1,High,TMR4,1);//初始化pwm
    Pwm_Init('B',7,1,High,TMR4,2);
    
    RM_Init(2,CAN2_RM_BUF,5);//如果四个电机要初始化4次吗？应该不用，因为现在传进去的是指针，然后里面也一直是指针，然后在接收处理函数，通过
	//指针地址加加，达到数据存到不同的指针下面。
    VESC_Init(1,VESC_BUF,5);
    
    CAN1_RX0_IRQ=CAN1_IRQ;//装载CAN1回调函数
    CAN2_RX1_IRQ=CAN2_IRQ;//装载CAN2回调函数
    
    
    while(1)
    {
//        PushMotor.Zero_Place=*PushMotor.Now_Place;//获取推射2006启动位置为0点
//        if(key_scan(&key,!PCin(3)) || (CAN2_RM_BUF[1].pos!=-1))//按下PC3按键或者接收到推射电机数据
//        {
//            PushMotor.Zero_Place=*PushMotor.Now_Place;//获取推射2006启动位置为0点
//            break;//跳出循环，进入系统
//        }
//		
		dma_printf("aaa\r\n");
        
        delay_ms(5);
    }
    
    
    BEEP(1);
    delay_ms(500);
    BEEP(0);
    PDout(13)=0;
    PDout(14)=0;
    PDout(15)=0;
    
//    FreeRtos_Mode(R1_Task,TASK_NUM);//进入系统
    while(1);
}
