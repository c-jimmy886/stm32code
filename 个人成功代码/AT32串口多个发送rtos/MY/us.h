#ifndef __US_H
#define __US_H	 
#include "sys.h"



//系统变量
#define US_Pid          "sys_pid"           //当前页面ID                （只读）     [0~225]
#define US_Baud         "sys_baud"          //当前波特率                （只读）     [设备支持的波特率：4800，9600，14400，19200，38400，56000，57600，115200，256000，460800，921600]
#define US_Light        "sys_light"         //亮度                      （可读写）   [0~100]
#define US_Sleep        "sys_sleep"         //是否休眠                  （可读写）   [0不休眠，1休眠]
#define US_SlpCle       "sys_slp_cls"       //休眠等级                  （可读写）   [0为普通休眠（熄灭背光），1为深度休眠]
#define US_SlpNu        "sys_slp_nu"        //无串口数据自动睡眠时间     （可读写)    [单位:秒 0~255，0表示不睡眠]
#define US_SlpNt        "sys_slp_nt"        //无触摸操作自动睡眠时间     （可读写）   [单位:秒 0~255，0表示不睡眠]
#define US_WupBt        "sys_wup_bt"        //睡眠模式下触摸唤醒开关     （可读写）   [0睡眠后触摸不会唤醒，1睡眠后触摸唤醒]
#define US_WupBu        "sys_wup_bu"        //睡眠模式下串口数据唤醒开关 （可读写)    [0睡眠后串口不会唤醒，1睡眠后串口唤醒]
#define US_Bkcmd        "sys_bkcmd"         //设置指令执行结果的返回     （只读）     [0x00不返回结果，0x01只返回成功的结果，0x02只返回失败的结果，0x03成功或者失败都返回结果]
#define US_Year         "rtc_year"          //使用RTC时获取时间年       （可读写）    [0~99，使用时需要加上偏移2000]
#define US_Month        "rtc_month"         //使用RTC时获取时间月       （可读写）    [1-12]
#define US_Day          "rtc_day"           //使用RTC时获取时间日       （可读写）    [1-28/29/30/31]
#define US_Hour         "rtc_hour"          //使用RTC时获取时间时       （可读写）    [00:00-23:00]
#define US_Minute       "rtc_minute"        //使用RTC时获取时间分       （可读写）    [00-59]
#define US_Second       "rtc_second"        //使用RTC时获取时间秒       （可读写）    [00-59]
#define US_Week         "rtc_week"          //使用RTC时获取时间周       （只读)       [周一到周六为：1-6，周日：0]
#define US_RunTime      "sys_run_time"      //系统运行时间              （只读）      [0~n，单位秒系统运行时，该变量从0累加]
#define US_NtTime       "sys_nt_time"       //系统无触摸时间            （只读）      [0~n，单位秒无触摸时，该变量从0累加；有触摸时，自动置零]
#define US_NuTime       "sys_nu_time"       //系统无串口数据时间        （只读）      [0~n，单位秒无接收到串口数据时，该变量从0累加；有接收到串口数据时，自动置零]
#define US_ModebusAddr  "sys_modbus_addr"   //主串口modbus从机地址      （可读写）    [0~255]
#define US_Uid          "sys_uid"           //串口屏唯一ID              （只读）      [4字节的int类型，0~0xFFFFFFFF]
#define US_Stopbits     "sys_stopbits"      //主串口通信 停止位         （可读写）     [1: 1位;2: 2位;3: 1.5位]
#define US_Parity       "sys_parity"        //主串口通信 校验位         （可读写）     [0: 无;1: 奇校验;2: 偶校验]
#define US_DisAngle     "sys_dis_angle"     //显示角度                  （可读写）    [0（0度），1（90度），2（180度），3（270度）]
#define US_MemPer1      "sys_mem_per1"      //系统运行内存使用情况       （只读）      [0-100%]
#define US_MemPer2      "sys_mem_per2"      //系统GUI内存使用情况       （只读）       [0-100%]
#define US_DirItem_type "sys_dir_item_type" //SD卡文件条目类型，        （只读）       [0文件，1文件夹]
                                            //只能配合readdir()函数使用 




typedef union US_get_data//串口屏接收的数据
{
    int Int;
    char Char[128];
    float Float;
}US_Get;


void US_Init(u8 USARTx,u32 bound);//串口屏初始化
u8 US_Rx(u8*buf,u8 len);//串口屏数据接收
int ByteToInt(u8*data,u8 mode);//字节形转整形
u16 US_addt_int(u8 page_id,u8 widget_id,u8 ch,u16 len,int*data);//曲线数据透传(Int型)
u16 US_addt_char(u8 page_id,u8 widget_id,u8 ch,u16 len,char*data);//曲线数据透传(Char型)
u16 US_addt_short(u8 page_id,u8 widget_id,u8 ch,u16 len,short*data);//曲线数据透传(Short型)
u16 US_addt_float(u8 page_id,u8 widget_id,u8 ch,u16 len,float*data);//曲线数据透传(Float型)
void US_page(char*page_name_id);//切换页面
void US_reset(void);//复位屏幕
void US_click(char*widget_name);//模拟点击
void US_event(char*widget_name,u8 val);//事件触发
void US_wset(char*name_natue,char*value);//控件属性设置
u8 US_wget(char*name_natue,US_Get*reset_value);//控件属性读取
void US_sset(char*sys_name,char*value);//系统变量设置
u8 US_sget(char*sys_name,int*reset_value);//系统变量读取

#endif
