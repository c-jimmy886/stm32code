#ifndef __AK_MOTOR_H
#define __AK_MOTOR_H	 
#include "sys.h"



#define AK_P_MIN    -12.5f
#define AK_P_MAX    12.5
#define AK_V_MIN    -30.0f
#define AK_V_MAX    30.0f
#define AK_T_MIN    -18.0f
#define AK_T_MAX    18.0f
#define AK_Kp_MIN   0
#define AK_Kp_MAX   500.0f
#define AK_Kd_MIN   0
#define AK_Kd_MAX   5.0f
#define AK_Test_Pos 0.0f


#define AK_Enable   0xFC
#define AK_Disable  0xFD
#define AK_SetZero  0xFE

#define AK_MODE_MIT   0x00 //运控模式
#define AK_MODE_SERVO 0x01 //伺服模式


//串口返回数据类型
#define AK_USART_RX_MOS_TEMP     1<<0  //MOS温度  (2byte)
#define AK_USART_RX_MOTOR_TEMP   1<<1  //电机温度 (2byte)
#define AK_USART_RX_OUT_CURRENT  1<<2  //输出电流 (4byte)
#define AK_USART_RX_IN_CURRENT   1<<3  //输入电流 (4byte)
#define AK_USART_RX_ID_CURRENT   1<<4  //id电流   (4byte)
#define AK_USART_RX_IQ_CURRENT   1<<5  //iq电流   (4byte)
#define AK_USART_RX_DUTY         1<<6  //占空比   (2byte)
#define AK_USART_RX_RPM          1<<7  //转速     (4byte)
#define AK_USART_RX_IN_VOLTAGE   1<<8  //输入电压 (2byte)
#define AK_USART_RX_ERROR        1<<15 //错误标志 (1byte)
#define AK_USART_RX_POS          1<<16 //位置     (4byte)
#define AK_USART_RX_ID           1<<17 //电机ID   (1byte)


typedef struct
{
    uint32_t id;//电机id;
    float pos;//位置
    float vel;//速度
    float cur;//转矩
    u8 temp;//MOS温度
    
    int round;//圈数
    double place;//多圈位置
    float last_pos;//上一个位置
}AK_RX_DATA;//AK电机数据接收结构体


typedef struct
{
    float mos_temp;//MOS温度
    float motor_temp;//电机温度
    float out_current;//输出电流
    float in_current;//输入电流
    float id_current;//id电流
    float iq_current;//iq电流
    float duty;//占空比
    float vel;//速度
    float in_voltage;//输入电压
    u8 start;//电机状态
    float pos;//位置
    u8 id;//电机ID
    float vd_voltage;//Vd电压
    float vq_voltage;//Vq电压
}AK_USART_RX_DATA;//AK电机串口接收数据结构体




//错误标志
typedef enum 
{
    FAULT_CODE_NONE = 0,
    FAULT_CODE_OVER_VOLTAGE,// 过压
    FAULT_CODE_UNDER_VOLTAGE,// 欠压
    FAULT_CODE_DRV,// 驱动故障
    FAULT_CODE_ABS_OVER_CURRENT,// 电机过流
    FAULT_CODE_OVER_TEMP_FET,// MOS 过温
    FAULT_CODE_OVER_TEMP_MOTOR,//电机过温
    FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE,// 驱动过压
    FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE,// 驱动欠压
    FAULT_CODE_MCU_UNDER_VOLTAGE,// MCU 欠压
    FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET,// 欠压
    FAULT_CODE_ENCODER_SPI,// SPI 编码器故障
    FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE,//编码器超限
    FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE,//编码器超限
    FAULT_CODE_FLASH_CORRUPTION,// FLASH 故障
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1,// 电流采样通道 1 故障
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2,// 电流采样通道 2 故障
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3,// 电流采样通道 1 故障
    FAULT_CODE_UNBALANCED_CURRENTS,// 电流不平衡
}mc_fault_code;


//串口数据帧
typedef enum {
COMM_FW_VERSION = 0,
COMM_JUMP_TO_BOOTLOADER, 
COMM_ERASE_NEW_APP, 
COMM_WRITE_NEW_APP_DATA, 
COMM_GET_VALUES, //获取电机运行参数
COMM_SET_DUTY, //电机以占空比模式运行
COMM_SET_CURRENT, //电机以电流环模式运行
COMM_SET_CURRENT_BRAKE, //电机电流刹车模式模式运行
COMM_SET_RPM, //电机以速度环模式运行
COMM_SET_POS, //电机以位置环模式运行
COMM_SET_HANDBRAKE, //电机以手刹电流环模式运行
COMM_SET_DETECT, //电机实时反馈当前位置指令
COMM_ROTOR_POSITION=22,//电机反馈当前位置
COMM_GET_VALUES_SETUP=50,//电机单个或多个参数获取指令
COMM_SET_POS_SPD=91, // 电机以位置速度环模式运行
COMM_SET_POS_MULTI=92, // 设置电机运动为单圈模式
COMM_SET_POS_SINGLE=93, // 设置电机运动为多圈模式 范围±100 圈
COMM_SET_POS_UNLIMITED=94, //保留
COMM_SET_POS_ORIGIN=95, //设置电机原点
} COMM_PACKET_ID;



//AK电机伺服模式控制报文
typedef enum 
{
    AK_PACKET_SET_DUTY = 0, //占空比模式
    AK_PACKET_SET_CURRENT, //电流环模式
    AK_PACKET_SET_CURRENT_BRAKE, // 电流刹车模式
    AK_PACKET_SET_RPM, // 转速模式
    AK_PACKET_SET_POS, // 位置模式
    AK_PACKET_SET_ORIGIN_HERE, //设置原点模式
    AK_PACKET_SET_POS_SPD, //位置速度环模式
} AK_PACKET_ID;



void AK_Motor_MIT_Cmd(u8 id,u8 cmd);//AK电机MIT命令
void AK_MIT_Init(u8 CANx,AK_RX_DATA*AK_Rx_Buf,u8 Buf_Len);//AK电机MIT初始化
void AK_SERVO_Init(u8 CANx,AK_RX_DATA*AK_Rx_Buf,u8 Buf_Len);//AK电机伺服初始化
void AK_Motor_MIT_Tx(u8 id,float _vel,float _pos,float _KP,float _KD,float _torq);//AK电机MIT模式发送
void AK_Duty_Tx(u8 id,float duty);//占空比模式（伺服）
void AK_Current_Tx(u8 id,float current);//电流环模式（伺服）
void AK_CB_Tx(u8 id,float current);//电流刹车模式（伺服）
void AK_RPM_Tx(u8 id,int rpm);//速度环模式（伺服）
void AK_POS_Tx(u8 id,float angle);//位置环模式（伺服）
void AK_Origin_Tx(u8 id,u8 mode);//设置原点模式（伺服）
void AK_POS_SPD_Tx(u8 id,float angle,int rpm,int rpa);//位置速度模式（伺服）
void AK_Motor_MIT_RX(uint32_t id,u8*buf);//AK电机MIT模式接收
void AK_Motor_SERVO_RX(uint32_t id,u8*buf);//AK伺服模式数据接收

void AK_SERVO_USART_Init(u8 USARTx,u8 dma_enable);//AK电机串口1初始化
void AK_SERVO_USART_GetValues(u8 USARTx);//获取一次电机运行参数
void AK_SERVO_USART_GetDetect(u8 USARTx);//设置电机10ms返回一次位置
void AK_SERVO_USART_SetManyRound(u8 USARTx);//设置电机多圈模式
void AK_SERVO_USART_SetOneRound(u8 USARTx);//设置电机单圈模式
void AK_SERVO_USART_SetCypher(u8 USARTx);//设置电机位置最短距离回零
void AK_SERVO_USART_SetOrigin(u8 USARTx);//设置电机原点
void AK_SERVO_USART_GetValuesSetup(u8 USARTx,uint32_t AK_USART_RX);//电机单个或多个参数获取指令
void AK_SERVO_USART_SetDuty(u8 USARTx,float DUTY);//电机以占空比模式运行
void AK_SERVO_USART_SetCurrent(u8 USARTx,float CURRENT);//电机以占空比模式运行
void AK_SERVO_USART_SetCB(u8 USARTx,float CURRENT);//电机电流刹车模式模式运行
void AK_SERVO_USART_SetRPM(u8 USARTx,int RPM);//电机以速度环模式运行
void AK_SERVO_USART_SetPOS(u8 USARTx,int POS);//电机以位置环模式运行
void AK_SERVO_USART_SetHB(u8 USARTx,float CURRENT);//电机电流手刹模式模式运行
void AK_SERVO_USART_SetPOS_SetSPD(u8 USARTx,float POS,float RPM,float RPA);//电机以位置速度环模式运行
u8 AK_USART_RX(u8 USARTx,AK_USART_RX_DATA*AK_USART_RX_BUF);//AK串口接收



#endif
