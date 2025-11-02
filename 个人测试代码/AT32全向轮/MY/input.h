#ifndef __INPUT_H
#define __INPUT_H	 
#include "sys.h"

#define on_choose 385595     //选择性接收并显示串口发送的数据
#define off_choose -385595   //选择性接收不显示串口发送的数据

int Gets(int show,char*str); //接收并比较串口发送数据函数
int Gets_clear(int x,int y);//清除未接收的数据函数
int Gets_num(int x);//接收整数函数
double Gets_snum(int x);//接收浮点数函数  
void Getchar(void);//缓冲区函数
int Get_find(void);//检测串口是否接收到数据函数
int Scanf(char*str,const int*p,...);//接收并比较串口发送的数据是否符合类型，并且返回格式化整数或字符（支持 %d %c）
#endif
