#ifndef __CHASSIS_H
#define __CHASSIS_H	 
#include "sys.h"

typedef struct
{
    double wheel1_speed;
    double wheel2_speed;
    double wheel3_speed;
    double wheel4_speed;
}OMNIRANGE_WHEEL;


void omnirange_wheel_move(OMNIRANGE_WHEEL*OmnirangeWheel,double speed,double angle,float shift_angle);
void omnirange_wheel_revolve(OMNIRANGE_WHEEL*OmnirangeWheel,double speed,u8 direction);







typedef struct
{
    double angle;
    double place_x;
    double place_y;
    
    double wheel1_last_pos;
    double wheel2_last_pos;
    double wheel3_last_pos;
    double wheel4_last_pos;
}OMNIRANGE_WHEEL_CHASSIS_LOCATION;

void OWCL_Count(OMNIRANGE_WHEEL_CHASSIS_LOCATION*OWCL_DATA,double shift_angle,double Kc,double wheel1_pos,double wheel2_pos,double wheel3_pos,double wheel4_pos);


#endif
