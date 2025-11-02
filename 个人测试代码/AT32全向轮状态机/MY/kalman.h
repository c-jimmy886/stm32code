#ifndef __KALMAN_H
#define __KALMAN_H	 
#include "sys.h"

#define Kalman_1 struct kalman_1

struct kalman_1
{
    float A;//状态转移矩阵
    float B;//控制输入矩阵
    float H;//观察矩阵
    
    float Q;//过程噪声协方差
    float R;//观测噪声协方差

    float X;//初始状态
    float P;//初始状态协方差

    float K;//卡尔曼增益
};

void kalman_init(struct kalman_1*kalman,float A,float B,float H,float Q,float R,float X,float P);
float kalman_oper(struct kalman_1*kalman,float Z,float U);



#endif
