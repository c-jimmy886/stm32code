#ifndef __AD7190_H
#define __AD7190_H
#include "sys.h"


#define AD7190_WRITH            0//写数据
#define AD7190_READ             1//写数据



/* AD7190所有寄存器地址 (RS2, RS1, RS0)*/
#define AD7190_REG_COMM         0 // 通讯寄存器                           (WO, 8-bit)只写寄存器
#define AD7190_REG_STAT         0 // 状态寄存器                           (RO, 8-bit)只读寄存器
#define AD7190_REG_MODE         1 // 模式寄存器                           (RW, 24-bit)
#define AD7190_REG_CONF         2 // 配置寄存器                           (RW, 24-bit)
#define AD7190_REG_DATA         3 // 数据寄存器/数据寄存器加状态寄存器      (RO, 24/32-bit)
#define AD7190_REG_ID           4 // ID寄存器                             (RO, 8-bit)
#define AD7190_REG_GPOCON       5 // GPOCON寄存器                         (RW, 8-bit)
#define AD7190_REG_OFFSET       6 // 失调寄存器                           (RW, 24-bit)
#define AD7190_REG_FULLSCALE    7 // 满量程寄存器                         (RW, 24-bit)


/* AD7190工作模式 */
#define AD7190_DM_CON           0//连续转换模式
#define AD7190_DM_SIN           1//单次转换模式
#define AD7190_DM_IDL           2//空闲模式
#define AD7190_DM_ECO           3//省电模式
#define AD7190_DM_INS_ZER       4//内部零电平校准
#define AD7190_DM_INS_FUL       5//内部满量程校准
#define AD7190_DM_SYS_ZER       6//系统零电平校准
#define AD7190_DM_SYS_FUL       7//系统满量程校准


/* AD7190通道选择 */
#define AD7190_CH0              (1<<0)//通道0（AIN+：AIN1 ； AIN-：AIN2   ； CHD：000 ； 校准寄存器对：0）
#define AD7190_CH1              (1<<1)//通道1（AIN+：AIN3 ； AIN-：AIN4   ； CHD：001 ； 校准寄存器对：1）
#define AD7190_CH2              (1<<2)//通道2（               温度传感器                校准寄存器对：无）
#define AD7190_CH3              (1<<3)//通道3（AIN+：AIN2 ； AIN-：AIN2   ； CHD：011 ； 校准寄存器对：0）
#define AD7190_CH4              (1<<4)//通道4（AIN+：AIN1 ； AIN-：AINCOM ； CHD：000 ； 校准寄存器对：0）
#define AD7190_CH5              (1<<5)//通道5（AIN+：AIN2 ； AIN-：AINCOM ； CHD：000 ； 校准寄存器对：1）
#define AD7190_CH6              (1<<6)//通道6（AIN+：AIN3 ； AIN-：AINCOM ； CHD：000 ； 校准寄存器对：2）
#define AD7190_CH7              (1<<7)//通道7（AIN+：AIN4 ； AIN-：AINCOM ； CHD：000 ； 校准寄存器对：3）


u8 AD7190_RW_1Byte(u8 data);
int AD7190_RW_3Byte(int data);
void AD7190_Reset(void);
u8 StaReg(void);
void ComReg(u8 AD7190_REG,u8 AD7190_wirte_read,u8 series_read);
int ModReg(u8 AD7190_WRITH_READ,u8 AD7190_DM,u8 DAT_STA,u8 CLK,u8 SINC3,u8 ENPAR,u8 Single,u8 REJ60,u16 FS9_0);
int ConReg(u8 AD7190_WRITH_READ,u8 Chop,u8 REFSEL,u8 CH7_0,u8 Burn,u8 REFDET,u8 BUF,u8 U_B_,u8 G2_0);
u8 DatReg(int*data,u8 condition);
u8 StaReg(void);
void AD7190_Init(spi_type*SPIx,u8 SPI_CS_CPIO,u8 SPI_CS_PIN,u8 CHx);
u8 AD7190_Check(void);
u8 AD7190_ADC(double*adc);
u8 AD7190_RDY(void);



#endif
