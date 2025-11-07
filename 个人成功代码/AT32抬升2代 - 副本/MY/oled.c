#include<sys.h>
#include<iic.h>
#include<oled.h>
#include<delay.h>
#include<usart.h>


void oled_cmd(uint8_t cmd)//写命令
{
    
    iic_start();
    iic_send(0x78);//寄存器地址
    iic_wait_ack(0);
    iic_send(0x00);
    iic_wait_ack(0);
    iic_send(cmd);
    iic_wait_ack(0);
    iic_stop();
    

}

void oled_data(uint8_t Data)//写数据
{
    iic_start();
    iic_send(0x78);//寄存器地址
    iic_wait_ack(0);
    iic_send(0x40);
    iic_wait_ack(0);
    iic_send(Data);
    iic_wait_ack(0);
    iic_stop();

}

void oled_cetcursor(uint8_t Y, uint8_t X)//设置坐标，Y:0~7；X:0~127
{
    oled_cmd(0xB0 | Y);//设置Y位置
    oled_cmd(0x10 | ((X & 0x70) >> 4));//设置X位置高4位
    oled_cmd(0x00 | (X & 0x0F));//设置X位置低4位
}

void oled_clear(void)//清屏
{  
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        oled_cetcursor(j, 0);
        for(i = 0; i < 128; i++)
        {
            oled_data(0x00);
        }
    }
}



long long OLED_DATA[128];//存储屏幕信息；128个元素对应128列，每列8字节
long long OLED_DATA_last[128];//存储上次屏幕信息；128个元素对应128列，每列8字节


//oled初始化
//SCL_GPIO，SCL_PIN：时钟IO口
//SDA_GPIO，SDA_PIN：信号IO口
void oled_init(char SCL_GPIO,int SCL_PIN,char SDA_GPIO,int SDA_PIN)
{
    u8 i;
    delay_init(168);
    delay_ms(20);
    for(i=0;i<128;i++){OLED_DATA[i]=0;OLED_DATA_last[i]=0;}
    iic_init(SCL_GPIO,SCL_PIN,SDA_GPIO,SDA_PIN);//初始化iic
    oled_cmd(0xAE);	//关闭显示
    
    oled_cmd(0xD5);	//设置显示时钟分频比/振荡器频率
    oled_cmd(0x80);
    
//    oled_cmd(0xA8);	//设置多路复用率
//    oled_cmd(0x31);
    
    oled_cmd(0xD3);	//设置显示偏移
    oled_cmd(0x00);

    oled_cmd(0x40);	//设置显示开始行
    oled_cmd(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
    oled_cmd(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

    oled_cmd(0xDA);	//设置COM引脚硬件配置
    oled_cmd(0x12);

    oled_cmd(0x81);	//设置对比度控制
    oled_cmd(0xff);

    oled_cmd(0xD9);	//设置预充电周期
    oled_cmd(0xff);

    oled_cmd(0xDB);	//设置VCOMH取消选择级别
    oled_cmd(0x30);

    oled_cmd(0xA4);	//设置整个显示打开/关闭

    oled_cmd(0xA6);	//设置正常/倒转显示

    oled_cmd(0x8D);	//设置充电泵
    oled_cmd(0x14);

    oled_cmd(0xAF);	//开启显示
    oled_clear();//清屏
    
}



//操控OLED(8*128)
//X:0~127；Y:0~(63-8）
//data:控制（8*1）像素点,低位在前
void OLED(u8 X,u8 Y,u8 data)
{
    long long oled_data=data;//将8位数据扩容位64位
    long long oled_clear=0xff;//设置8位清除区
    //限定范围防止数据溢出
    if(X>127)X=127;
    if(Y>63-8)Y=63-8;
    oled_clear<<=Y;//将清除区移动到目标坐标
    oled_data<<=Y;//将数据移动到目标坐标
    oled_clear=~oled_clear;//启动清除区
    OLED_DATA[X]&=oled_clear;//将目标位置的旧数据清除
    OLED_DATA[X]|=oled_data;//将新的数据放入目标位置
}


//导入显示数据
//(X,Y)起点坐标   X：0~127，Y：0~63
//x,y点阵大小     x：0~127，y：0~7
//导入的数据
void oled_input(u8 X,u8 Y,u8 x,u8 y,u8*data)
{
    u8 i,j;
    for(i=0;i<x;i++)
    {
        for(j=0;j<y/8;j++)
        {
            OLED(i+X,j*8+Y,*(data+i+x*j));//导入数据至存储区
        }
    }
}


//更新指定区域（row1~row2行，page1~page2列）oled数据
//row1,row2<=7;page1,page2<=127
//row1<=row2;page1<=page2
//auto_renew=1开启自动识别更新
void oled_renew(u8 row1,u8 row2,u8 page1,u8 page2,u8 auto_renew)
{
    u8 i,j;
    //限定范围防止数据溢出
    if(page1>127)page1=127;
    if(page2>127)page2=127;
    if(row1>7)row1=7;
    if(row2>7)row2=7;
    for(i=page1;i<=page2;i++)//更新数据
    {
        for(j=row1;j<=row2;j++)
        {
            if((OLED_DATA[i]>>8*j)!=(OLED_DATA_last[i]>>8*j)||auto_renew==0)//上次区块数据与本次要更新的数据不符
            {
                oled_cetcursor(j,i);//对区块进行更新
                oled_data(OLED_DATA[i]>>8*j);
            }
        }
    }
    if(auto_renew)for(i=0;i<128;i++)OLED_DATA_last[i]=OLED_DATA[i];//复制屏幕信息
}


void oled_lighteness(u8 lighteness)//调节亮度，0~0xff
{
    oled_cmd(0x81);	//设置对比度控制
	oled_cmd(lighteness);
	oled_cmd(0xD9);	//设置预充电周期
	oled_cmd(lighteness);
}


//屏幕移动
//move:移动方向
//X:移动距离
void oled_move(u8 move,u8 X)
{
    int i;
    long long oled_buf[128];//oled屏幕数据缓存
    for(i=0;i<128;i++)oled_buf[i]=OLED_DATA[i];//复制oled数据至缓存区
    if(move==OLED_UP)//向上移动
    {
        if(X<1)X=1;//限制移动位数1~63
        if(X>63)X=63;
        for(i=0;i<128;i++)
        {
            OLED_DATA[i]>>=X;//将数据向左移X(即oled屏幕数据向上移X)
            OLED_DATA[i]&=(0x7fffffffffffffff>>(X-1));//将X以上的位置0
            oled_buf[i]<<=(64-X);//将X以下的位移到高位（即oled上面屏幕数据移到下面）
            OLED_DATA[i]|=oled_buf[i];//整合到oled数据
        }
    }
    if(move==OLED_DOWN)//向下移动
    {
        if(X<1)X=1;//限制移动位数1~63
        if(X>63)X=63;
        for(i=0;i<128;i++)
        {
            OLED_DATA[i]<<=X;//将数据向右移X(即oled屏幕数据向下移X)
            oled_buf[i]>>=(64-X);//将X以下的位置0
            oled_buf[i]&=(0xffffffffffffffff>>(64-X));//将X以上的位移到低位
            OLED_DATA[i]|=oled_buf[i];//整合到oled数据
        }
    }
    if(move==OLED_LEFT)//向左移动
    {
        if(X<1)X=1;//限制移动位数1~127
        if(X>127)X=127;
        for(i=0;i<128;i++)
        {
            if(i<128-X)OLED_DATA[i]=oled_buf[i+X];//将右边的数据向左移X(即oled屏幕数据向左移X)
            else OLED_DATA[i]=oled_buf[i+X-128];//将左边的数据向右移X（即oled左边屏幕数据移到右边）
        }
    }
    if(move==OLED_RIGHT)//向右移动
    {
        if(X<1)X=1;//限制移动位数1~127
        if(X>127)X=127;
        for(i=127;i>=0;i--)
        {
            if(i<X)OLED_DATA[i]=oled_buf[128-X+i];//将左边的数据向右移X(即oled屏幕数据向右移X)
            else OLED_DATA[i]=oled_buf[i-X];//将右边的数据向左移X（即oled右边屏幕数据移到左边）
        }
    }
}

//oled屏幕放大（将X：0~63；Y：0~31的图像放大至全屏）
void oled_boost()
{
    u8 i,j;
    long long oled_buf[128];//oled屏幕数据缓存
    for(i=0;i<128;i+=2)
    {
        oled_buf[i]=OLED_DATA[i];//将数据复制到缓存区
        oled_buf[i+1]=OLED_DATA[i+1];
        OLED_DATA[i]=0;//清除原先数据
        OLED_DATA[i+1]=0;
        for(j=0;j<64;j+=2)
        {
            if(((oled_buf[i/2]>>(j/2))&0x01)==1)//某个像素点为亮，则将对应该像素的四个像素点点亮
            {
                OLED_DATA[i]|=((long long)0x01<<j);
                OLED_DATA[i+1]|=((long long)0x01<<j);
                OLED_DATA[i]|=((long long)0x01<<(j+1));
                OLED_DATA[i+1]|=((long long)0x01<<(j+1));
            }
        }
    }
}

//oled屏幕缩小（将图像缩小至X：0~63；Y：0~31）
//tv：阈值（1~4）
void oled_lessen(u8 tv)
{
    u8 i,j,k;
    long long oled_buf[128];//oled屏幕数据缓存
    if(tv<1)tv=1;//限制阈值范围
    if(tv>4)tv=4;
    for(i=0;i<128;i+=2)
    {
        oled_buf[i]=OLED_DATA[i];//将数据复制到缓存区
        oled_buf[i+1]=OLED_DATA[i+1];
        OLED_DATA[i]=0;//清除原先数据
        OLED_DATA[i+1]=0;
        for(j=0;j<64;j+=2)//检测相邻的四个像素点
        {
            k=0;
            if(((oled_buf[i]>>j)&0x01)==1)k++;
            if(((oled_buf[i]>>(j+1))&0x01)==1)k++;
            if(((oled_buf[i+1]>>j)&0x01)==1)k++;
            if(((oled_buf[i+1]>>(j+1))&0x01)==1)k++;
            
            if(k>=tv)OLED_DATA[i/2]|=(0x01<<(j/2));//若这四个像素点亮的点数大于等于阈值，则点亮对应的像素点
        }
    }
}
