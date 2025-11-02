#ifndef __MY_LIBRARY_H
#define __MY_LIBRARY_H	 
#include "sys.h"

//模拟电容滤波数据结构体
typedef struct
{
    double Kc;
    double OUT;
}CAP_FILTER;


//S型速度规划数据结构体
typedef struct
{
    double last_speed;
    double aim_speed;
    double out_speed;
    uint32_t t;
}S_SPEED_PLAN_DATA;




void mat_add(double*mat1,double*mat2,u8 row_num,u8 list_num,double*mat_return);//矩阵相加
void mat_sub(double*mat1,double*mat2,u8 row_num,u8 list_num,double*mat_return);//矩阵相减
u8 mat_mul(double*mat1,double*mat2,u8 mat1_row_num,u8 mat1_list_num,u8 mat2_row_num,u8 mat2_list_num,double*mat_return);//矩阵相乘
void mat_roll(double*mat,u8 row_num,u8 list_num,double*mat_return);//矩阵转置
u8 array_move_8(u8*array,unsigned int add_start,unsigned int add_end,int move_len);//数组元素移动(8位)
u8 array_move_16(u16*array,unsigned int add_start,unsigned int add_end,int move_len);//数组元素移动(16位)
u8 array_move_32(u32*array,unsigned int add_start,unsigned int add_end,int move_len);//数组元素移动(32位)
long long bit_move(long long data,u8 bit_start,u8 bit_end,int bit_move);//位移动
float equation_1_2(float a,float b,float c,float*x1,float*x2);//解一元二次方程
u8 parabola_angle(float g,float v,float X,float Y, float*x1,float*x2);//抛物线计算（仰角）
float parabola_vel(float g,float angle,float X,float Y);//抛物线计算（初速度）
unsigned int FloatToHex(float num);//浮点数转16进制
unsigned int FloatToHex_union(float num);//浮点数转16进制(共用体方式)
float AngToRad(float Angle);//角度转弧度
float RadToAng(float Radian);//弧度转角度
long long my_abs(long long num);//整数绝对值
double Cap_Filter(CAP_FILTER*CapFilter,double IN);//模拟电容滤波
unsigned long long BitToHex(char*BIT);//二进制转十六进制
double S_Speed_Plan(S_SPEED_PLAN_DATA*SPEED_PLAN_DATA,uint32_t Tmax,double aim_speed);//S型速度规划
 


#endif
