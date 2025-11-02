#include<sys.h>
#include<my_library.h>
#include<math.h>
#include<string.h>
#include<stdarg.h>
#include<stdio.h>


/*矩阵相加*/
//mat1：被加矩阵
//mat2：加矩阵
//row_num：矩阵行数
//list_num：矩阵列数
//mat_return：返回结果矩阵
void mat_add(double*mat1,double*mat2,u8 row_num,u8 list_num,double*mat_return)
{
    u8 i;
    for(i=0;i<row_num*list_num;i++)
    {
        *(mat_return+i)=*(mat1+i)+*(mat2+i);
    }
}


/*矩阵相减*/
//mat1：被减矩阵
//mat2：减矩阵
//row_num：矩阵行数
//list_num：矩阵列数
//mat_return：返回结果矩阵
void mat_sub(double*mat1,double*mat2,u8 row_num,u8 list_num,double*mat_return)
{
    u8 i;
    for(i=0;i<row_num*list_num;i++)
    {
        *(mat_return+i)=*(mat1+i)-*(mat2+i);
    }
}


/*矩阵相乘*/
//mat1：被乘矩阵
//mat2：乘矩阵
//mat1_row_num：被乘矩阵行数
//mat1_list_num：被乘矩阵列数
//mat2_row_num：乘矩阵行数
//mat2_list_num：乘矩阵列数
//mat_return：返回结果矩阵
u8 mat_mul(double*mat1,double*mat2,u8 mat1_row_num,u8 mat1_list_num,u8 mat2_row_num,u8 mat2_list_num,double*mat_return)
{
    u8 i,j,k;
    if(mat1_list_num!=mat2_row_num)return 0;//被乘矩阵列数 不等于 乘矩阵行数，不符合矩阵乘法要求，返回0
    for(i=0;i<mat1_row_num;i++)
    {
        for(j=0;j<mat2_list_num;j++)
        {
            *(mat_return+i*mat2_list_num+j)=0;
            for(k=0;k<mat1_list_num;k++)*(mat_return+i*mat2_list_num+j)+=*(mat1+i*mat1_list_num+k)*(*(mat2+j+k*mat2_list_num));
        }
    }
    return 1;//完成矩阵乘法运算，返回1
}

/*矩阵转置*/
//mat：被转置矩阵
//row_num：矩阵行数
//list_num：矩阵列数
//mat_return：返回结果矩阵
void mat_roll(double*mat,u8 row_num,u8 list_num,double*mat_return)
{
    u8 i,j;
    for(i=0;i<list_num;i++)
    {
        for(j=0;j<row_num;j++)*(mat_return+i*row_num+j)=*(mat+j*list_num+i);
    }
}



/*数组元素移动(8位)*/
//array：数组首地址
//add_start：移动区域首元素
//add_end：移动区域尾元素
//移动距离
//注意：必须add_start<add_end；|move_len|<add_end-add_start；move_len!=0
u8 array_move_8(u8*array,unsigned int add_start,unsigned int add_end,int move_len)
{
    int i;
    u8 buf[128];
    if(add_start>add_end||move_len>(int)(add_end-add_start)||-move_len>(int)(add_end-add_start)||move_len==0)return 0;//不符合条件，直接返回0
    for(i=0;i<=add_end-add_start;i++)buf[i]=array[i+add_start];
    if(move_len>0)for(i=0;i<=add_end-add_start;i++)array[add_start+(i+move_len<=add_end-add_start ? i+move_len : i+move_len-(add_end-add_start)-1)]=buf[i];
    if(move_len<0)for(i=0;i<=add_end-add_start;i++)array[add_start+(i+move_len>=0 ? i+move_len : i+move_len+(add_end-add_start)+1)]=buf[i];
    return 1;//完成运算，返回1
}


/*数组元素移动(16位)*/
//array：数组首地址
//add_start：移动区域首元素
//add_end：移动区域尾元素
//移动距离
//注意：必须add_start<add_end；|move_len|<add_end-add_start；move_len!=0
u8 array_move_16(u16*array,unsigned int add_start,unsigned int add_end,int move_len)
{
    int i;
    u16 buf[128];
    if(add_start>add_end||move_len>(int)(add_end-add_start)||-move_len>(int)(add_end-add_start)||move_len==0)return 0;//不符合条件，直接返回0
    for(i=0;i<=add_end-add_start;i++)buf[i]=array[i+add_start];
    if(move_len>0)for(i=0;i<=add_end-add_start;i++)array[add_start+(i+move_len<=add_end-add_start ? i+move_len : i+move_len-(add_end-add_start)-1)]=buf[i];
    if(move_len<0)for(i=0;i<=add_end-add_start;i++)array[add_start+(i+move_len>=0 ? i+move_len : i+move_len+(add_end-add_start)+1)]=buf[i];
    return 1;//完成运算，返回1
}

/*数组元素移动(32位)*/
//array：数组首地址
//add_start：移动区域首元素
//add_end：移动区域尾元素
//移动距离
//注意：必须add_start<add_end；|move_len|<add_end-add_start；move_len!=0
u8 array_move_32(u32*array,unsigned int add_start,unsigned int add_end,int move_len)
{
    int i;
    u32 buf[128];
    if(add_start>add_end||move_len>(int)(add_end-add_start)||-move_len>(int)(add_end-add_start)||move_len==0)return 0;//不符合条件，直接返回0
    for(i=0;i<=add_end-add_start;i++)buf[i]=array[i+add_start];
    if(move_len>0)for(i=0;i<=add_end-add_start;i++)array[add_start+(i+move_len<=add_end-add_start ? i+move_len : i+move_len-(add_end-add_start)-1)]=buf[i];
    if(move_len<0)for(i=0;i<=add_end-add_start;i++)array[add_start+(i+move_len>=0 ? i+move_len : i+move_len+(add_end-add_start)+1)]=buf[i];
    return 1;//完成运算，返回1
}


/*位移动*/
//data：输入数据
//bit_start：移动区域首位
//bit_end：移动区域末位
//移动距离
long long bit_move(long long data,u8 bit_start,u8 bit_end,int bit_move)
{
    long long buf=data;//移动区域缓存
    long long move_buf;//移动部分缓存
    long long clear=0;//清除区
    if((int)(bit_end-bit_start)<0||bit_move>(int)(bit_end-bit_start)||-bit_move>(int)(bit_end-bit_start)||bit_move==0)return 0;//不符合条件，直接返回0
    clear=(long long)(pow(2,bit_end-bit_start+1)-1);//建立清除区大小（大小为移动区域大小）
    clear<<=bit_start;//将移动区域转移到待清除区（移动区域）
    clear=~clear;//激活清除区
    buf>>=bit_start;//将移动区移到最低位，方便处理
    buf&=(long long)(pow(2,bit_end-bit_start+1)-1);//分离出移动区
    if(bit_move<0)//向低位移动
    {
        move_buf=buf&(long long)(pow(2,-bit_move)-1);//将要移动的部分放到移动部分缓存
        buf>>=(-bit_move);//将剩下部分移到最低位
        move_buf<<=(bit_end+bit_move+1);//将移动部分移动到目标位置
        buf<<=bit_start;//将剩下部分移动到目标位置
        data&=clear;//清除移动区域之前数据
        data|=buf;//将移动后的数据放回移动区域，完成运算
        data|=move_buf;
    }
    if(bit_move>0)
    {
        move_buf=buf&(long long)(pow(2,bit_end-bit_start+1-bit_move)-1);//将要移动的部分放到移动部分缓存
        buf>>=(bit_end-bit_start+1-bit_move);//将剩下部分移到最低位
        move_buf<<=(bit_start+bit_move);//将移动部分移动到目标位置
        buf<<=bit_start;//将剩下部分移动到目标位置
        data&=clear;//清除移动区域之前数据
        data|=buf;//将移动后的数据放回移动区域，完成运算
        data|=move_buf;
    }
    return data;//返回运算结果
}


//解一元二次方程
float equation_1_2(float a,float b,float c,float*x1,float*x2)
{
    float delta=b*b-4*a*c;
    *x1=(-b+sqrt(delta))/2/a;
    *x2 =(-b-sqrt(delta))/2/a;
    return delta;
}


//抛物线计算（仰角）
//g：重力加速度
//v：初速度
//X，Y：经过点位坐标
//x1，x2：两个运算结果
u8 parabola_angle(float g,float v,float X,float Y, float*x1,float*x2)
{
    float X1,X2;
    float delta;
    float a=pow(g,2)*pow(X,4)/4/pow(v,4);
    float b=g*Y*pow(X,2)/pow(v,2)-pow(X,2);
    float c=pow(Y,2)+pow(X,2);
    delta=equation_1_2(a,b,c,&X1,&X2);//解方程
    if(delta<0)return 0;
    
    X1=sqrt(1/(X1));//得到余弦值
    X2=sqrt(1/(X2));
    
    X1=acos(X1);//得到弧度
    X2=acos(X2);
    
    *x1=X1*180.0f/3.1415926f;//得到角度
    *x2=X2*180.0f/3.1415926f;
    
    return 1;//无实数解，返回1
}


//抛物线计算（初速度）
//g：重力加速度
//v：初速度
//X，Y：经过点位坐标
float parabola_vel(float g,float angle,float X,float Y)
{
    /********************************************************************
    
    v*cos(angle)*t=X
    v*sin(angle)*t-0.5*g*pow(t,2)=Y
    
    t=X/(v*cos(angle))
    v*sin(angle)*(X/(v*cos(angle)))-0.5*g*pow(X/(v*cos(angle)),2)=Y
    
    v=sqrt(g*pow(X,2)/(2*X*sin(angle)*cos(angle)-2*Y*pow(cos(angle),2)))
    
    ********************************************************************/
    
    angle=angle/180*3.1415926f;
    return sqrt(g*pow(X,2)/(2*X*sin(angle)*cos(angle)-2*Y*pow(cos(angle),2)));
}




//浮点数转16进制
unsigned int FloatToHex(float num)
{
    u8 i;
    int S=0,E=0,F=0;
    float ex;
    if(num==0)return 0;
    if(num<0)
    {
        S=1;
        num=-num;
    }
    E=log10(num)/log10(2);
    ex=num/pow(2,E)-1;
    for(i=0;i<23;i++)
    {
        ex*=2;
        if(ex>=1)
        {
            F++;
            ex--;
        }
        F<<=1;
    }
    F>>=1;
    return (S<<31)|((E+127)<<23)|F;
}


//浮点数转16进制(共用体方式)
unsigned int FloatToHex_union(float num)
{
    union float_hex
    {
        float Float;
        int Hex;
    }Float_Hex;
    Float_Hex.Float=num;
    return Float_Hex.Hex;
}


//角度转弧度
//Angle：输入角度
float AngToRad(float Angle)
{
    float PI=3.1415926;
    return Angle*PI/180.0f;
}


//弧度转角度
//Radian：输入弧度
float RadToAng(float Radian)
{
    float PI=3.1415926;
    return Radian*180.0f/PI;
}


long long my_abs(long long num)
{
    return num>=0 ? num : -num; 
}


//模拟电容滤波
//CapFilter：传入参数
//IN：输入数据
double Cap_Filter(CAP_FILTER*CapFilter,double IN)
{
    CapFilter->OUT+=(IN-CapFilter->OUT)/CapFilter->Kc;
    return CapFilter->OUT;
}


//二进制（字符）转十六进制
//BIT：输入二进制（字符）
unsigned long long BitToHex(char*BIT)
{
    int i=0;
    long long Hex=0;
    while(1)
    {
        if(BIT[i]=='1'||BIT[i]=='0')
        {
            Hex<<=1;
            Hex+=BIT[i]-'0';
        }
        else if(BIT[i]!=' ') break;
        
        i++;
    }
    return Hex;
}


//S型速度规划
//SPEED_PLAN_DATA：传入参数
//Tmax：加速周期
//aim_speed：目标速度
double S_Speed_Plan(S_SPEED_PLAN_DATA*SPEED_PLAN_DATA,uint32_t Tmax,double aim_speed)
{
    if(SPEED_PLAN_DATA->aim_speed==aim_speed)//目标速度未改变
    {
        if(SPEED_PLAN_DATA->t<0xFFFFFFFF)SPEED_PLAN_DATA->t++;//运行时间累加
    }
    else //目标速度改变
    {
        SPEED_PLAN_DATA->t=0;//运行时间清0
        SPEED_PLAN_DATA->last_speed=SPEED_PLAN_DATA->out_speed;//将当前速度记录为上一个速度
        SPEED_PLAN_DATA->aim_speed=aim_speed;//记录新的目标速度
    }
    
    SPEED_PLAN_DATA->out_speed=SPEED_PLAN_DATA->last_speed + (aim_speed-SPEED_PLAN_DATA->last_speed) / (1+pow(2.7182818,-5*(2*SPEED_PLAN_DATA->t/(double)Tmax-1)));//速度规划计算
    /*
                      aim_speed - last_speed
        last_speed + ------------------------
                      1 + e^(-5*(2*t/Tmax-1))
    */
    return SPEED_PLAN_DATA->out_speed;
}
