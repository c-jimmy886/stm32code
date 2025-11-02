#ifndef __HWT901_H
#define __HWT901_H	 
#include "sys.h"



/**HWT901输出数据**/
#define HWT901_Output_Time               0x50   //时间
#define HWT901_Output_Acceleration       0x51   //加速度
#define HWT901_Output_Palstance          0x52   //角速度
#define HWT901_Output_Angle              0x53   //角度
#define HWT901_Output_MagneticField      0x54   //磁场
#define HWT901_Output_PortStatus         0x55   //端口状态
#define HWT901_Output_Pressure_Height    0x56   //气压、高度
#define HWT901_Output_Longitude_Latitude 0x57   //经纬度
#define HWT901_Output_GroundVelocity     0x58   //地速
#define HWT901_Output_Quaternion         0x59   //四元数
#define HWT901_Output_GPSPrecision       0x5A   //GPS精度





/**HWT901指令**/
#define HWT901_CMD_SAVE0       0x000000  //保持当前设置
#define HWT901_CMD_SAVE1       0x000100  //恢复默认配置

#define HWT901_CMD_CALSW0      0x010000  //退出校准模式
#define HWT901_CMD_CALSW1      0x010100  //进入加速度计校准
#define HWT901_CMD_CALSW2      0x010200  //进入磁场校准
#define HWT901_CMD_CALSW3      0x010300  //高度置0

#define HWT901_CMD_DIRECTION0  0x230000  //设置为水平安装
#define HWT901_CMD_DIRECTION1  0x230100  //设置为垂直安装

#define HWT901_CMD_SLEEP       0x220100  //进入/退出休眠

#define HWT901_CMD_LOCK0       0x6988B5  //解锁
#define HWT901_CMD_LOCK1       0x6977A5  //锁定

#define HWT901_CMD_ALG0        0x240000  //设置成9轴算法
#define HWT901_CMD_ALG1        0x240100  //设置成6轴算法

#define HWT901_CMD_GYRO0       0x630000  //选择自动校准
#define HWT901_CMD_GYRO1       0x630100  //去掉自动校准

#define HWT901_CMD_RATE1       0x030100  //回传速率0.1Hz
#define HWT901_CMD_RATE2       0x030200  //回传速率0.5Hz
#define HWT901_CMD_RATE3       0x030300  //回传速率1Hz
#define HWT901_CMD_RATE4       0x030400  //回传速率2Hz
#define HWT901_CMD_RATE5       0x030500  //回传速率5Hz
#define HWT901_CMD_RATE6       0x030600  //回传速率10Hz(默认)
#define HWT901_CMD_RATE7       0x030700  //回传速率20Hz
#define HWT901_CMD_RATE8       0x030800  //回传速率50Hz
#define HWT901_CMD_RATE9       0x030900  //回传速率100Hz
#define HWT901_CMD_RATEa       0x030A00  //回传速率125Hz
#define HWT901_CMD_RATEb       0x030B00  //回传速率200Hz
#define HWT901_CMD_RATEc       0x030C00  //单次输出

#define HWT901_CMD_BAUD0       0x040000  //波特率2400
#define HWT901_CMD_BAUD1       0x040100  //波特率4800
#define HWT901_CMD_BAUD2       0x040200  //波特率9600
#define HWT901_CMD_BAUD3       0x040300  //波特率19200
#define HWT901_CMD_BAUD4       0x040400  //波特率38400
#define HWT901_CMD_BAUD5       0x040500  //波特率57600
#define HWT901_CMD_BAUD6       0x040600  //波特率115200(默认)
#define HWT901_CMD_BAUD7       0x040700  //波特率230400
#define HWT901_CMD_BAUD8       0x040800  //波特率460800

#define HWT901_CMD_AXOFFSET(x) 0x050000|(uint16_t)x  //设置X轴加速度零偏(x：X轴加速度零偏)
#define HWT901_CMD_AYOFFSET(y) 0x060000|(uint16_t)y  //设置Y轴加速度零偏(y：Y轴加速度零偏)
#define HWT901_CMD_AZOFFSET(z) 0x070000|(uint16_t)z  //设置Z轴加速度零偏(z：Z轴加速度零偏)

#define HWT901_CMD_GXOFFSET(x) 0x080000|(uint16_t)x  //设置X轴角速度零偏(x：X轴角速度零偏)
#define HWT901_CMD_GYOFFSET(y) 0x090000|(uint16_t)y  //设置Y轴角速度零偏(y：Y轴角速度零偏)
#define HWT901_CMD_GZOFFSET(z) 0x0A0000|(uint16_t)z  //设置Z轴角速度零偏(z：Z轴角速度零偏)

#define HWT901_CMD_HXOFFSET(x) 0x0B0000|(uint16_t)x  //设置X轴磁场零偏(x：X轴磁场零偏)
#define HWT901_CMD_HYOFFSET(y) 0x0C0000|(uint16_t)y  //设置Y轴磁场零偏(y：Y轴磁场零偏)
#define HWT901_CMD_HZOFFSET(z) 0x0D0000|(uint16_t)z  //设置Z轴磁场零偏(z：Z轴磁场零偏)


void HWT901_Cmd(uint32_t HWT901_CMD);//HWT901写指令
void HWT901_Init(u8 USARTx,u32 bound);//HWT901初始化
void HWT901_Output_Mode(u8 HWT901_Output);//设置HWT901输出类型
u8 HWT901_Rx(u8*buf);//HWT901数据接收
u8 HWT901_XYZ_Rx(float*X,float*Y,float*Z);//读取计算HWT901三轴数据


#endif
