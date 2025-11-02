#include<chassis.h>
#include<math.h>


//全向轮移动
//OmnirangeWheel：全向轮数据结构体
//speed：移动速度
//angle：移动方向角度
//shift_angle：偏移角度
void omnirange_wheel_move(OMNIRANGE_WHEEL*OmnirangeWheel,double speed,double angle,float shift_angle)
{
    OmnirangeWheel->wheel1_speed=-speed*sin((angle-shift_angle)/180.0f*3.1415926f);
    OmnirangeWheel->wheel2_speed=speed*cos((angle-shift_angle)/180.0f*3.1415926f);
    OmnirangeWheel->wheel3_speed=speed*sin((angle-shift_angle)/180.0f*3.1415926f);
    OmnirangeWheel->wheel4_speed=-speed*cos((angle-shift_angle)/180.0f*3.1415926f);
}

//全向轮原地旋转
//OmnirangeWheel：全向轮数据结构体
//speed：旋转速度
//direction：旋转方向（1:逆时针，0:顺时针）
void omnirange_wheel_revolve(OMNIRANGE_WHEEL*OmnirangeWheel,double speed,u8 direction)
{
    if(direction)
    {
        OmnirangeWheel->wheel1_speed=-speed;
        OmnirangeWheel->wheel2_speed=-speed;
        OmnirangeWheel->wheel3_speed=-speed;
        OmnirangeWheel->wheel4_speed=-speed;
    }
    else
    {
        OmnirangeWheel->wheel1_speed=speed;
        OmnirangeWheel->wheel2_speed=speed;
        OmnirangeWheel->wheel3_speed=speed;
        OmnirangeWheel->wheel4_speed=speed;
    }
}






void OWCL_Count(OMNIRANGE_WHEEL_CHASSIS_LOCATION*OWCL_DATA,double shift_angle,double Kc,double wheel1_pos,double wheel2_pos,double wheel3_pos,double wheel4_pos)
{
    //计算所有轮子连续两次位置差
    double wheel1_pos_err=wheel1_pos-OWCL_DATA->wheel1_last_pos;
    double wheel2_pos_err=wheel2_pos-OWCL_DATA->wheel2_last_pos;
    double wheel3_pos_err=wheel3_pos-OWCL_DATA->wheel3_last_pos;
    double wheel4_pos_err=wheel4_pos-OWCL_DATA->wheel4_last_pos;
    
    OWCL_DATA->angle=-(wheel1_pos+wheel2_pos+wheel3_pos+wheel4_pos)/4.0f/Kc*360.0f;//将4个轮子的位置的和的平均值除底盘周长乘360°，得到底盘旋转角度
    
    //根据偏移的角度求出底盘在一小段时间内X轴的位移
    OWCL_DATA->place_x+=(wheel2_pos_err-wheel4_pos_err)/2*cos((OWCL_DATA->angle-shift_angle)/180.0*3.1425926f) + (wheel1_pos_err-wheel3_pos_err)/2*sin((OWCL_DATA->angle-shift_angle)/180.0*3.1425926f);
    //根据偏移的角度求出底盘在一小段时间内Y轴的位移
    OWCL_DATA->place_y+=(wheel2_pos_err-wheel4_pos_err)/2*sin((OWCL_DATA->angle-shift_angle)/180.0*3.1425926f) - (wheel1_pos_err-wheel3_pos_err)/2*cos((OWCL_DATA->angle-shift_angle)/180.0*3.1425926f);
    
    //记录4个轮子单前位置为上一个位置
    OWCL_DATA->wheel1_last_pos=wheel1_pos;
    OWCL_DATA->wheel2_last_pos=wheel2_pos;
    OWCL_DATA->wheel3_last_pos=wheel3_pos;
    OWCL_DATA->wheel4_last_pos=wheel4_pos;
}


/*================================================================================================================================*/
/*
                                                                  Y
                                                                  ^
                                                                  |
                                                                  |
                                                                  |
                                                                  |
                                                                  |


                                                           ****************
                                                         *        2         *
                                                           ****************


                                                 *                                   *
                                               *   *                               *   *
                                               *   *                               *   *
                                               *   *                               *   *
                                               * 3 *                               * 1 *          ----------->X   0°
                                               *   *                               *   *
                                               *   *                               *   *
                                               *   *                               *   *
                                               *   *                               *   *
                                                 *                                   *


                                                           **************** 
                                                         *         4        *
                                                           **************** 
*/











