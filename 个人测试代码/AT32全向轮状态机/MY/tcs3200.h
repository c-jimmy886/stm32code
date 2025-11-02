#ifndef __TCS3200_H
#define __TCS3200_H	 
#include "sys.h"


#define tcs_out_mode_0 0      //000%输出，000KHz
#define tcs_out_mode_2 2      //002%输出，012KHz
#define tcs_out_mode_20 20    //020%输出，120KHz
#define tcs_out_mode_100 100  //100%输出，600KHz

#define tcs_RGB_red 'R'   //红光滤波
#define tcs_RGB_green 'G' //绿光滤波
#define tcs_RGB_blue 'B'  //蓝光滤波
#define tcs_RGB_null '0'  //无滤波

void tcs_init(char S0_GPIO,u8 S0_PIN,char S1_GPIO,u8 S1_PIN,char S2_GPIO,u8 S2_PIN,char S3_GPIO,u8 S3_PIN,u8 tcs_out_mode);//初始化tcs3200
void tcs_RGB_mode(u8 R_G_B);//RGB滤波选择

#endif
