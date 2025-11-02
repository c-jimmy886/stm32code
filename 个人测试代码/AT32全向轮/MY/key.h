#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY_MODE_SERIES             (                                                             0) //连续触发模式
#define KEY_MODE_DOWN               (                                                             1) //按下触发模式
#define KEY_MODE_UP                 (                                                             2) //弹起触发模式
#define KEY_MODE_LONG(T,SERIES)     (                            ((T&0xFFF)<<8)|((SERIES&0xF)<<4)|3) //长按触发模式（T:长按时间，SERIES:是否连续触发）
#define KEY_MODE_MANY(MAX,T)        ((((long long)MAX&0x7F)<<32)|((T&0xFFF)<<8)|                  4) //多按触发模式（MAX:最大按下次数，T:最大间隔时间）
#define KEY_MODE_REPEAT(MAX,T,WAIT) ((((long long)MAX&0x7F)<<32)|((T&0xFFF)<<8)|((WAIT&0xF)  <<4)|5) //快速复按模式（MAX:最大复按次数，T:最大间隔时间，WAIT:是否等待间隔结束再返回0）


typedef u8 key_bool;


typedef struct
{
    long long KEY_MODE;//按键模式
    struct 
    {
        int trigger     :1;//触发标志
        int trigger_num :7;//触发次数
        int time        :24;//时间记录
    }KEY_FLAG;//按键状态
}KEY;



void key_init(KEY*key,long long KEY_MODE);//按键初始化
u8 key_scan(KEY*key,key_bool event);//按键扫描


#endif
