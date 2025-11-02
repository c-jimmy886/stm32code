#include<sys.h>
#include<pid.h>
#include<math.h>
#include<usart.h>

//pid初始化
void pid_init(struct PID*pid,float Kp,float Ki,float Kd)
{
    pid->Kp=Kp;//设置比例参数
    pid->Ki=Ki;//设置积分参数
    pid->Kd=Kd;//设置微分参数
    pid->MB=0;
    pid->MQ=0;
    pid->err=0;
    pid->last_err=0;
    pid->P=0;
    pid->I=0;
    pid->D=0;
}

//pid运算
//pid:pid结构体地址；mb:目标量；mq:目前量
//积分最大值
//积分最小值
double Pid(struct PID*pid,double mb,double mq,double umax,double umin)
{
    float pid_U=0.00;//pid输出
    
    pid->MB=mb;//获得目标量
    pid->MQ=mq;//获得目前量
    
    pid->err=pid->MB-pid->MQ;//计算误差
    pid->P=pid->err;//当前比例
    pid->D=pid->err-pid->last_err;//当前微分
    pid->last_err=pid->err;//记录当前误差
    if((pid->Kp * pid->P + pid->Ki * pid->I>umax&&pid->err<0)||(pid->Kp * pid->P + pid->Ki * pid->I<umin&&pid->err>0)||(pid->Kp * pid->P + pid->Ki * pid->I<umax&&pid->Kp * pid->P + pid->Ki * pid->I>umin))//（抗积分饱和）
    {
        pid->I+=pid->err;//计算积分
    }
    pid_U=pid->Kp * pid->P + pid->Ki * pid->I + pid->Kd * pid->D;//pid运算
    
    if(pid_U>umax)pid_U=umax;
    if(pid_U<umin)pid_U=umin;
    
    return pid_U;//返回pid值
}



//速度环（pwm输出）
//pid:pid结构体地址；mb:目标速度；mq:目前速度；pwm_max;pwm最大值
uint16_t pid_speed_pwm(struct PID*pid,double mb,double mq,uint16_t pwm_max)
{
    
    float U=Pid(pid,mb,mq,pwm_max,0);//pid运算
    float pwm=pwm_max*U/mb;
    if(pwm>pwm_max)pwm=(float)pwm_max;
    return (uint16_t)pwm;//得到返回相应pwm
}

//速度环（电流输出）
//pid:pid结构体地址；mb:目标速度；mq:目前速度；current_max;电流最大值
double pid_speed_current(struct PID*pid,double mb,double mq,uint16_t current_max)
{
    float U=Pid(pid,mb,mq,current_max,-current_max);//pid运算
    float current=U;
    if(current>current_max)current=(float)current_max;
    if(current<-current_max)current=-(float)current_max;
    return (int)current;//得到返回相应电流值
}



//位置环
//pid:pid结构体地址；mb:目标位置；mq:目前位置
//max_input:输出最大值
//使用位置环尽量不要使用积分
double pid_place(struct PID*pid,double mb,double mq,double max_input)
{
    double U=Pid(pid,mb,mq,max_input,-max_input);//pid运算
    if(U>max_input)U=max_input;
    if(U<-max_input)U=-max_input;
    return U;//得到返回相应pwm(返回值为负数，说明电机应反转，使用时要做好处理)
}


//位置_速度环（能以某一速度达到目标位置）
//pid_place:位置环pid结构体地址；aim_place:目标位置；now_place:当前位置
//pid_speed:速度环pid结构体地址；aim_speed:最大速度；now_speed:当前速度
//change_place:切环位置
//max_output:输出最大值
double pid_place_speed(struct PID*pid_place,double aim_place,double now_place,struct PID*pid_speed,double aim_speed,double now_speed,double change_place,double max_output)
{
    float U;//pid运算值
    float imax=0;//积分最大值
    if(fabs(aim_place-now_place)<change_place)U=Pid(pid_place,aim_place,now_place,0,0);//位置环pid运算
    else U=Pid(pid_speed,aim_speed*(aim_place-now_place>0 ? 1 : -1),now_speed,imax,-imax);//速度环运算
    
    if(U>max_output)U=max_output;
    if(U<-max_output)U=-max_output;
    return U;//返回运算结果
}
#include<dma_usart.h>

//串级pid（位置_速度）
//pid_place:位置环pid结构体地址；aim_place:目标位置；now_place:当前位置
//pid_speed:速度环pid结构体地址；max_speed:最大速度；now_speed:当前速度
//max_output:输出最大值
//Ka:启动加速因子
double Pid_PlaceSerialSpeed(struct PID*pid_place,double aim_place,double now_place,struct PID*pid_speed,double max_speed,double now_speed,double max_output,float Ka)
{
    double U1,U2;
    U1=Pid(pid_place,aim_place,now_place,max_speed,-max_speed);
    if(U1== max_speed&&now_speed+Ka< max_speed)U1=now_speed+Ka;
    if(U1==-max_speed&&now_speed-Ka>-max_speed)U1=now_speed-Ka;
    U2=Pid(pid_speed,U1,now_speed,max_output,-max_output);
    return U2;
}



//同步环
//pid：pid结构体
//mb：目标量
//mq：目前量
//average：目前量均值
//max_input：最大输出
//min_input：最小输出
//返回偏移量
double pid_same(struct PID*pid,double mb,double mq,double average,double max_input,double min_input)
{
    double U;
    U=Pid(pid,-fabs(mb-average),-fabs(mb-mq),max_input,min_input);
    if(U>max_input)return max_input;
    else if(U<min_input)return min_input;
    else return U;
}


//取样求平均
u8 average(u16 sample_num,float output,float*input,struct sample*SAMPLE)
{
    if(SAMPLE->Sample_init!=1)//未初始化
    {
        //进行初始化
        SAMPLE->Sample_num=0;
        SAMPLE->Sample_add=0;
        SAMPLE->I_input=0;
        SAMPLE->Sample_init=1;//初始化完成标记为已初始化
    }
    if(SAMPLE->Sample_num==0)SAMPLE->Sample_num=sample_num;//获得取样次数
    SAMPLE->Sample_add++;//当前取样次数
    SAMPLE->I_input+=output;//取样累加
    if(SAMPLE->Sample_add==SAMPLE->Sample_num)//达到最大取样次数
    {
        *input=SAMPLE->I_input / SAMPLE->Sample_num;//计算平均值
        //清零
        SAMPLE->I_input=0;
        SAMPLE->Sample_num=0;
        SAMPLE->Sample_add=0;
        return 1;//获得平均值，则返回1
    }
    else return 0;//反之返回0
}




//位置规划速度的速度位置pid
//PID_POS_VEL：相关结构体
//Kp：速度环Kp
//Ki：速度环Ki
//Kd：速度环Kd
//Ka：加速度系数（该值为0，则为普通速度环；其他为加速度最大为Ka的位置速度环）
//Kj：加加速度系数（该值为0，则为恒定加速度的位置速度环(速度为指定的最大速度)；其他为加加速度为Kj，最大加速度为Ka的位置速度环）
//speed_max：最大速度
//now_speed：当前速度
//aim_place：目标位置
//now_place：当前位置
//input_max：最大输出值
//fast_on：是否快速启动（1:是，0:否）（快速启动下电机启动会瞬间达到最大速度）
float pid_pos_vel(PID_POS_VEL*BUF,float Kp,float Ki,float Kd,float Ka,float Kj,float speed_max,float now_speed,float aim_place,float now_place,float input_max,u8 fast_on)
{
    float ka=Ka;//加速度
    float pid_U=0.00;//pid输出
    float aim_speed;//目标速度
    
    if(BUF->last_place!=aim_place)BUF->last_aim_place=now_place;//当上一个目标速度和本次目标速度不一样，更新上一个目标速度（作为初始位置）
    BUF->last_place=aim_place;//记录本次目标速度
    
    if(Ka!=0)//加速度系数不为0
    {
        speed_max=fabs(speed_max);//最大速度只能为正数
        if(fabs(now_place-BUF->last_aim_place)<fabs(now_place-aim_place))//当前位置距离初始位置比距离目标位置小
        {
            if(!fast_on)//不使用快速启动
            {
                if(Kj!=0)//加加速度系数不为0
                {
                    ka=pow(Kj*fabs(now_place-BUF->last_aim_place)+0.001f*speed_max,0.5);//根据当前位置计算加速度
                    if(ka>Ka)ka=Ka;//加速度限制
                }
                aim_speed=pow(ka*fabs(now_place-BUF->last_aim_place)+0.001f*speed_max,0.5)*(aim_place-now_place==0 ? 0 : (aim_place-now_place)/fabs(aim_place-now_place));//根据当前加速度和位置计算速度
            }
            else aim_speed=speed_max;//使用快速启动，一开始速度拉满
        }
        else //当前位置距离初始位置比距离目标位置大
        {
            if(Kj!=0)//加加速度系数不为0
            {
                ka=pow(Kj*fabs(aim_place-now_place),0.5);//根据当前位置计算加速度
                if(ka>Ka)ka=Ka;//加速度限制
            }
            aim_speed=pow(ka*fabs(aim_place-now_place),0.5)*(aim_place-now_place==0 ? 0 : (aim_place-now_place)/fabs(aim_place-now_place));//根据当前加速度和位置计算速度
        }
        //速度限幅
        if(aim_speed>speed_max)aim_speed=speed_max;
        if(aim_speed<-speed_max)aim_speed=-speed_max;
    }
    else aim_speed=speed_max;//加速度系数为0，根据最大速度跑位置环
    
    BUF->pid.err=aim_speed-now_speed;//计算误差
    BUF->pid.P=BUF->pid.err;//当前比例
    BUF->pid.D=BUF->pid.err-BUF->pid.last_err;//当前微分
    BUF->pid.last_err=BUF->pid.err;//记录当前误差
    
    if(Kp * BUF->pid.P + Ki * BUF->pid.I>input_max)//pid比例加积分输出超过输出最大值
    {
        if(Kp * BUF->pid.P>input_max)BUF->pid.I=0;//单纯比例就超过最大值，积分清0
        else if(BUF->pid.P<0)BUF->pid.I+=BUF->pid.err;//比例小于0，正常计算积分
        else BUF->pid.I=(input_max-Kp * BUF->pid.P)/Ki;//其他将根据比例改变积分，维持输出不超幅
    }
    else if(Kp * BUF->pid.P + Ki * BUF->pid.I<-input_max)//pid比例加积分输出小于输出最小值
    {
        if(Kp * BUF->pid.P<-input_max)BUF->pid.I=0;//单纯比例就小于最小值，积分清0
        else if(BUF->pid.P>0)BUF->pid.I+=BUF->pid.err;//比例大于0，正常计算积分
        else BUF->pid.I=(-input_max-Kp * BUF->pid.P)/Ki;//其他将根据比例改变积分，维持输出不超幅
    }
    else BUF->pid.I+=BUF->pid.err;////pid比例加积分输出在幅度内，正常计算积分
    
    pid_U=Kp * BUF->pid.P + Ki * BUF->pid.I + Kd * BUF->pid.D;//pid运算
    
    //输出限幅
    if(pid_U>input_max)pid_U=input_max;
    if(pid_U<-input_max)pid_U=-input_max;
    
    return pid_U;
}




/******************************************************串级PID调参******************************************************/

//先调速度环，调出的效果是反应时间尽量快，几乎没有过冲或过冲较小；曲线有较小的波动无关紧要，手握电机感觉抖动不明显就行
//由于速度环已经有Kp,Ki,Kd存在，所以位置环的Kp,Kd一般不用太大，Ki一般为0
//采样时间改变会对原有的参数（Ki,Kd）产生影响；所以当需要改变采样时间时，Ki可以改为：原来的Ki / 原来的采样时间 * 更改后的采样时间，Kd可以改为：原来的Kd * 原来的采样时间 / 更改后的采样时间 ； 这样可以让更改采样时间后的效果和更改采样时间前的效果接近
//最后由于速度环有积分的存在，而且电机不能百分百到达目标位置，所以电机到达目标位置附近时可能会左右摆动，这个只要摆动幅度不大，而且频率不高是可以接受的（如果要减小这种现象只需要把Ki设小）

/***********************************************************************************************************************/


