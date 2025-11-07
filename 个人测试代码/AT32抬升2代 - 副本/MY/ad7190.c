#include<init.h>
#include<ad7190.h>
#include<delay.h>



spi_type*AD7190_SPI;//AD7190使用的SPI
//SPI1：SCL:PB3  ； MIS0:PB4  ； MOSI:PB5
//SPI2：SCL:PB13 ； MIS0:PB14 ； MOSI:PB15
//SPI3：SCL:PC10 ； MIS0:PC11 ； MOSI:PC12

u8 AD7190_SPI_CS_CPIO;//AD7190使用的SPI片选口
u8 AD7190_SPI_CS_PIN;
u8 AD7190_RDY_GPIO;
u8 AD7190_RDY_PIN;


/*AD7190读写1字节*/
u8 AD7190_RW_1Byte(u8 data)
{
    u8 ret;
    Pout(AD7190_SPI_CS_CPIO,AD7190_SPI_CS_PIN)=0;//片选拉低
    ret=Spi_RW(AD7190_SPI,data);
    Pout(AD7190_SPI_CS_CPIO,AD7190_SPI_CS_PIN)=1;//片选拉高
    return ret;
}


/*AD7190读写3字节*/
int AD7190_RW_3Byte(int data)
{
    int buf=0;
    Pout(AD7190_SPI_CS_CPIO,AD7190_SPI_CS_PIN)=0;//片选拉低
    buf+=Spi_RW(AD7190_SPI,(data >> 16) & 0xFF);
    buf<<=8;
    buf+=Spi_RW(AD7190_SPI,(data >> 8) & 0xFF);
    buf<<=8;
    buf+=Spi_RW(AD7190_SPI,(data >> 0) & 0xFF);
    Pout(AD7190_SPI_CS_CPIO,AD7190_SPI_CS_PIN)=1;//片选拉高
    return buf;
}


/*复位AD7190*/
void AD7190_Reset(void)
{
    u8 registerWord[7];
    u8 i=0;

    registerWord[0] = 0xFF;
    registerWord[1] = 0xFF;
    registerWord[2] = 0xFF;
    registerWord[3] = 0xFF;
    registerWord[4] = 0xFF;
    registerWord[5] = 0xFF;
    registerWord[6] = 0xFF;
    for(i = 0; i < 7; i++)AD7190_RW_1Byte(registerWord[i]);//连续发送40个1
    delay_ms(100);//等待100ms
}


/*读状态寄存器*/
u8 StaReg()
{
    ComReg(AD7190_REG_STAT,1,0);//选择读取状态寄存器 
    return AD7190_RW_1Byte(0xff);//读取并返回状态值
}


/*读ID寄存器*/
u8 IdReg()
{
    ComReg(AD7190_REG_ID,1,0);//选择读取ID寄存器
    return AD7190_RW_1Byte(0xff);//读取并返回状ID
}



/*操作通讯寄存器*/
//AD7190_REG：选择寄存器（0~7）
//AD7190_wirte_read：选择读写模式（0:写模式；1:读模式）
//series_read：是否开启连续读写
void ComReg(u8 AD7190_REG,u8 AD7190_wirte_read,u8 series_read)
{
    u8 Cmd=0x00;//操作指令
    AD7190_REG<<=3;//选择寄存器
    AD7190_wirte_read<<=6;//选择读写模式
    series_read<<=2;//是否开启连续读写
    Cmd|=(AD7190_REG|AD7190_wirte_read|series_read);//得到指令
    AD7190_RW_1Byte(Cmd);//发送指令
}


/*操作模式寄存器*/
//AD7190_WRITH_READ：选择读寄存器还是写寄存器（0:写；1:读）
//AD7190_DM：选择AD7190工作模式（0~7）
//DAT_STA：使能发送数据时附加状态（0~1）
//CLK：选择时钟源（0:外部晶振 ； 1:外部时钟 ； 2:4.92MHz内部时钟,MCLK2引脚为三态 ； 3:4.92MHz内部时钟,内部时钟可从MCLK2引获得）（0~3）
//SINC3：选择sinc3滤波器（默认选择sinc4）（0~1）
//ENPAR：使能奇偶校验（0~1）
//Single：单周期转换使能（0~1）
//REJ60：使能60HZ陷波（0~1）
//FS9_0：滤波器速率设置（0~1023）
int ModReg(u8 AD7190_WRITH_READ,u8 AD7190_DM,u8 DAT_STA,u8 CLK,u8 SINC3,u8 ENPAR,u8 Single,u8 REJ60,u16 FS9_0)
{
    int data=0;
    
    data+=((int)AD7190_DM & 0x7) << 21;
    data+=(int)DAT_STA<<20;
    data+=((int)CLK & 0x3) << 18;
    data+=(int)SINC3<<15;
    data+=(int)ENPAR<<13;
    data+=(int)Single<<11;
    data+=(int)REJ60<<10;
    data+=(int)FS9_0;
    ComReg(AD7190_REG_MODE,AD7190_WRITH_READ,0);//选择模式寄存器
    return AD7190_RW_3Byte(data);//发送数据，返回寄存器值
}


/*操作配置寄存器*/
//AD7190_WRITH_READ：选择读寄存器还是写寄存器（0:写；1:读）
//Chop：斩波使能（0~1）
//REFSEL：选择基准电压（0:REFSEL1(+)和REFSEL1(-)之间施加外部基准电压；1:P1/REFSEL2(+)和P0/REFSEL2(-)之间施加外部基准电压）（0~1）
//CH7_0：通道选择（0~7）
//Burn：使能激励电流（0~1）
//REFDET：使能基准电压检测（0~1）
//BUF：使能模拟输入缓冲器（0~1）
//U_B_：极性选择（0:双极性；1:单极性）（0~1）
//G2_0：增益选择（0：增益1,ADC范围+/-5V ； 1：保留 ； 2：保留 ； 3：增益8,ADC范围+/-625mV ； 4：增益16,ADC范围+/-312.5mV ； 5：增益32,ADC范围+/-156.2mV ； 6：增益64,ADC范围+/-78.125mV ； 7：增益128,ADC范围+/-39.06mV）（0~7）
int ConReg(u8 AD7190_WRITH_READ,u8 Chop,u8 REFSEL,u8 CH7_0,u8 Burn,u8 REFDET,u8 BUF,u8 U_B_,u8 G2_0)
{
    int data=0;
    
    data+=(int)Chop<<23;
    data+=(int)REFSEL<<20;
    data+=(int)(CH7_0&0xFF)<<8;
    data+=(int)Burn<<7;
    data+=(int)REFDET<<6;
    data+=(int)BUF<<4;
    data+=(int)U_B_<<3;
    data+=(int)G2_0&0x7;
    ComReg(AD7190_REG_CONF,AD7190_WRITH_READ,0);//选择配置寄存器
    return AD7190_RW_3Byte(data);//发送数据，返回寄存器值
}


/*读取数据寄存器*/
//buf接收数据缓存
//condition：是否接收状态
u8 DatReg(int*buf,u8 condition)
{
    u8 CHx=0xFF;
    ComReg(AD7190_REG_DATA,AD7190_READ,0);//选择读取数据寄存器
    *buf=AD7190_RW_3Byte(0xffffffff);//接收数据
    if(condition)
    {
        condition=AD7190_RW_1Byte(0xff);//接收状态（如果选择接收）
        if((condition&0x80)==0)CHx=condition&0x07;
    }
    return CHx;//返回状态值
}



//AD7190初始化
//SPIx：选择SPI
//SPI_CS_CPIO，SPI_CS_PIN：SPI片选信号口
void AD7190_Init(spi_type*SPIx,u8 SPI_CS_CPIO,u8 SPI_CS_PIN,u8 CHx)
{
    AD7190_SPI=SPIx;//获取AD7190的SPI
    AD7190_SPI_CS_CPIO=SPI_CS_CPIO;//获取AD7190的SPI片选口
    AD7190_SPI_CS_PIN=SPI_CS_PIN;
    
    if(SPIx==SPI1)AD7190_RDY_GPIO='B',AD7190_RDY_PIN=4;
    if(SPIx==SPI2)AD7190_RDY_GPIO='B',AD7190_RDY_PIN=14;
    if(SPIx==SPI3)AD7190_RDY_GPIO='C',AD7190_RDY_PIN=11;
    
    delay_init();
    GPIO_init(SPI_CS_CPIO,SPI_CS_PIN,OUT_PP);//初始化SPI片选口
    //初始化SPI：SPIx；全双工模式；主SPI；8位数据宽度；时钟空闲电平高；第二个跳变沿数据被采样；内部NSS信号由SSI位控制；波特率预分频值为128；数据传输从MSB位开始,CRC值计算的多项式=7
    Spi_Init(SPIx,SPI_TRANSMIT_FULL_DUPLEX,SPI_MODE_MASTER,SPI_FRAME_8BIT,SPI_CLOCK_POLARITY_HIGH,SPI_CLOCK_PHASE_2EDGE,SPI_MCLK_DIV_128,SPI_FIRST_BIT_MSB);
    AD7190_Reset();//复位AD7190
    
    ModReg(AD7190_WRITH,AD7190_DM_INS_FUL,0,2,0,0,1,1,5);//内部满量程校准
    delay_ms(100);//等待校准完成
    ModReg(AD7190_WRITH,AD7190_DM_INS_ZER,0,2,0,0,1,1,5);//内部零电压校准
    delay_ms(100);//等待校准完成
    
    //写寄存器；默认设置：连续转换模式；不使能数据附加状态；选择连接MCLK1和MCLK2获得的外部时钟；选择sin4滤波器；不使能奇偶校验；不使能单周期转换；使能60HZ陷波；滤波器速率设置为5
    ModReg(AD7190_WRITH,AD7190_DM_CON,1,0,0,0,0,1,5);
    
    //写寄存器；默认设置：斩波使能；选择REFSEL1(+)和REFSEL1(-)之间施加外部基准电压；选择通道CHx；不使能激励电流；不使能基准电压检测；使能模拟输入缓冲器；选择单极性；选择增益1
    ConReg(AD7190_WRITH,1,0,CHx,0,0,1,1,0);
}





#define OPA_RES_R1              6800  // 6.8k 运放输入端电阻
#define OPA_RES_R2              2000  // 2k 运放反馈电阻
#define REFERENCE_VOLTAGE       3297  // 参考电压（放大1000倍）



//检测AD7190
//id：AD7190的ID
u8 AD7190_Check()
{
    u8 id;
    id=IdReg();//读取ID
    if((id&0x0F)==0x04)return id;//ID正确，返回id
    else return 0;//ID错误，返回0
}



//ADC解算
//adc:读取到的ADC
//返回转化通道
u8 AD7190_ADC(double*adc)
{
    u8 CHx;
    int Buf;
    CHx=DatReg(&Buf,1);//只读取数据，并存储在Buf中
    Buf>>=4;//不知为什么，但必须怎么做
    *adc=(double)Buf*REFERENCE_VOLTAGE/OPA_RES_R2*OPA_RES_R1/0xfffff/1000-0.002;//转化成ADC数据(0.002：零偏)
    return CHx;
}




//转换完成中断
//转换完成，返回1
u8 AD7190_RDY()
{
    return !Pin(AD7190_RDY_GPIO,AD7190_RDY_PIN);
}

