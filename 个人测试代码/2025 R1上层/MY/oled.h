#ifndef __OLED_H
#define __OLED_H	 
#include "sys.h"

#define OLED_UP    1
#define OLED_DOWN  2
#define OLED_LEFT  3
#define OLED_RIGHT 4

void oled_cmd(uint8_t cmd);//写命令
void oled_data(uint8_t Data);//写数据
void oled_cetcursor(uint8_t Y, uint8_t X);//设置坐标
void oled_clear(void);//清屏
void oled_init(char SCL_GPIO,int SCL_PIN,char SDA_GPIO,int SDA_PIN);//初始化oled
void oled_input(u8 X,u8 Y,u8 x,u8 y,u8*data);//导入显示数据
void oled_renew(u8 rou1,u8 rou2,u8 page1,u8 page2,u8 auto_renew);//更新指定区域oled数据
void oled_lighteness(u8 lighteness);//调节亮度
void OLED(u8 X,u8 Y,u8 data);//操控OLED
void oled_move(u8 move,u8 X);//屏幕移动
void oled_boost(void);//放大（2倍）
void oled_lessen(u8 tv);//缩小（0.5倍）

#endif
