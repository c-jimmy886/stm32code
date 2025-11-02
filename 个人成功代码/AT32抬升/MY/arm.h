#ifndef __ARM_H
#define __ARM_H	 
#include "sys.h"



//三轴机械臂数据结构体
typedef struct
{
    float L1;//臂1长度
    float L2;//臂2长度
    float L3;//臂3长度
    
    float A1;//轴1角度
    float A2;//轴2角度
    float A3;//轴3角度
    
    float X;//末端坐标X
    float Y;//末端坐标Y
    float pitch;//末端俯仰角
    
    float A1_Max;//轴1角度最大值
    float A1_Min;//轴1角度最小值
    float A2_Max;//轴2角度最大值
    float A2_Min;//轴2角度最小值
    float A3_Max;//轴3角度最大值
    float A3_Min;//轴3角度最小值
}ARM_3Axle_Data;
//轴角度以逆时针旋转为正，与y轴夹角为0时角度为0
//



void ARM_3Axle_Init(ARM_3Axle_Data*arm_data,float L1,float L2,float L3,float A1_Max,float A1_Min,float A2_Max,float A2_Min,float A3_Max,float A3_Min);//三轴机械臂数据初始化
u8 ARM_3Axle_1(ARM_3Axle_Data*data,float X,float Y,float pitch);//三轴机械臂解算
u8 ARM_3Axle_2(ARM_3Axle_Data*data,float X,float Y,float pitch);


#endif
