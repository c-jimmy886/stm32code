#include<my_library.h>
#include<arm.h>
#include<math.h>


//三轴机械臂数据初始化
//arm_data：数据结构体
//L1：第一节臂长度
//L2：第一节臂长度
//L3：第一节臂长度
//A1_Max：轴1最大角度
//A1_Min：轴1最小角度
//A2_Max：轴2最大角度
//A2_Min：轴2最小角度
//A3_Max：轴3最大角度
//A3_Min：轴3最小角度
void ARM_3Axle_Init(ARM_3Axle_Data*arm_data,float L1,float L2,float L3,float A1_Max,float A1_Min,float A2_Max,float A2_Min,float A3_Max,float A3_Min)
{
    arm_data->L1=L1;
    arm_data->L2=L2;
    arm_data->L3=L3;
    
    arm_data->A1_Max=A1_Max;
    arm_data->A1_Min=A1_Min;
    arm_data->A2_Max=A2_Max;
    arm_data->A2_Min=A2_Min;
    arm_data->A3_Max=A3_Max;
    arm_data->A3_Min=A3_Min;
}



//三轴机械臂解算1（节点相互影响）
//arm_data：数据结构体
//X：末端横坐标
//Y：末端纵坐标
//pitch：末端俯仰角
u8 ARM_3Axle_1(ARM_3Axle_Data*data,float X,float Y,float pitch)
{
    float A1,A2,A3;//三轴角度
    float PI=3.1415926;//圆周率
//  float node1_x=0,node1_y=0;
    float node2_x,node2_y;//轴坐标
    float node3_x,node3_y;
    
    data->X=X;//得到机械臂末端坐标
    data->Y=Y;
    data->pitch=pitch;//得到机械臂末端俯仰角
    //计算轴3坐标
    node3_x=data->X - data->L3 * cos(AngToRad(data->pitch));//正交分解
    node3_y=data->Y - data->L3 * sin(AngToRad(data->pitch));//正交分解
    
    if(pow(node3_x,2)+pow(node3_y,2)>pow(data->L1+data->L2,2))return 0;//坐标超出机械臂长度，数据错误，返回0
    //计算轴1角度
    A1=  PI/2;
    A1-= acos( (pow(node3_x,2) + pow(node3_y,2) + pow(data->L1,2) - pow(data->L2,2)) / (2 * data->L1 * sqrt(pow(node3_x,2) + pow(node3_y,2))));//余弦定理
    A1-= atan(node3_y/node3_x);
    A1= -RadToAng(A1);
    if(A1 > data->A1_Max || A1 < data->A1_Min)return 0;//角度A1不在范围内，返回0
    //计算轴2角度
    A2=  acos((pow(node3_x,2) + pow(node3_y,2) - pow(data->L1,2) - pow(data->L2,2)) / (2 * data->L1 * data->L2)) * (node3_x - node2_x) / fabs(node3_x - node2_x);//余弦定理
    A2= -RadToAng(A2);
    if(A2 > data->A2_Max || A2 < data->A2_Min)return 0;//角度A2不在范围内，返回0
    //计算轴2坐标
    node2_x=sin(AngToRad(-A1)) * data->L1;//正交分解
    node2_y=cos(AngToRad(-A1)) * data->L1;//正交分解
    //计算轴3角度
    if(node3_y-node2_y!=0)A3=  data->pitch - (RadToAng(acos((node3_x - node2_x) / data->L2)) * (node3_y-node2_y) / fabs(node3_y-node2_y));
    else A3=data->pitch;
    if(A3 > data->A3_Max || A3 < data->A3_Min)return 0;//角度A3不在范围内，返回0
    
    data->A1=A1;//得到角度A1（逆时针为正）
    data->A2=A2;//得到角度A2（逆时针为正）
    data->A3=A3;//得到角度A3（逆时针为正）
    return 1;//解算成功，返回1
}

/***************************************************************************************************************************************************************
                                                                                  |
                                                                                |
                                                                          OOOO|   A3
                                                                     *****OOOO*****————————
                                                                  *****   OOOO   *****    pitch
                                                        L2     *****                *****
                                            |               *****         node3         *****   L3
                                              |   A2     *****                            *****
                                                |     *****                                  *****
                                                  |*****                                        *****C （X,Y）
                                                    OOO
                                                   OOOOO   node2
                                                    OOO 
                                                   *****         |
                                                     *****       |
                                                       *****  A1 |
                                                         *****   |
                                                     L1    ***** |
                                                             *****
                                                               *****
                                                                OOOO   node1
                                                               OOOOOO
                                              ==========================================

******************************************************************************************************************************************************************/



u8 ARM_3Axle_2(ARM_3Axle_Data*data,float X,float Y,float pitch)
{
    float A1,A2,A3;//三轴角度
    float PI=3.1415926;//圆周率
//  float node1_x=0,node1_y=0;
    float node2_x,node2_y;//轴坐标
    float node3_x,node3_y;
    
    data->X=X;//得到机械臂末端坐标
    data->Y=Y;
    data->pitch=pitch;//得到机械臂末端俯仰角
    //计算轴3坐标
    node3_x=data->X - data->L3 * cos(AngToRad(data->pitch));//正交分解
    node3_y=data->Y - data->L3 * sin(AngToRad(data->pitch));//正交分解
    
    if(pow(node3_x,2)+pow(node3_y,2)>pow(data->L1+data->L2,2))return 0;//坐标超出机械臂长度，数据错误，返回0
    
    //计算轴1角度
    A1=  PI/2;
    A1-= acos( (pow(node3_x,2) + pow(node3_y,2) + pow(data->L1,2) - pow(data->L2,2)) / (2 * data->L1 * sqrt(pow(node3_x,2) + pow(node3_y,2))));//余弦定理
    A1-= atan(node3_y/node3_x);
    A1= -RadToAng(A1);
    if(A1 > data->A1_Max || A1 < data->A1_Min)return 0;//角度A1不在范围内，返回0
    //计算轴2角度
    node2_x=sin(AngToRad(-A1)) * data->L1;//正交分解
    node2_y=cos(AngToRad(-A1)) * data->L1;//正交分解
    //计算轴2坐标
    A2= acos((pow(node3_x,2) + pow(node3_y,2) - pow(data->L1,2) - pow(data->L2,2)) / (2 * data->L1 * data->L2)) * (node3_x - node2_x) / fabs(node3_x - node2_x);//余弦定理
    A2= -RadToAng(A2);
    A2+=A1;
    if(A2 > data->A2_Max || A2 < data->A2_Min)return 0;//角度A2不在范围内，返回0
    //计算轴3角度
    if(node3_y-node2_y!=0)A3=  data->pitch - (RadToAng(acos((node3_x - node2_x) / data->L2)) * (node3_y-node2_y) / fabs(node3_y-node2_y));
    else A3=data->pitch;
    if(A3 > data->A3_Max || A3 < data->A3_Min)return 0;//角度A3不在范围内，返回0
    
    data->A1=A1;//得到角度A1（逆时针为正）
    data->A2=A2;//得到角度A2（逆时针为正）
    data->A3=A3;//得到角度A3（逆时针为正）
    return 1;//解算成功，返回1
}

