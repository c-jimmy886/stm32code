#include<init.h>
#include<init_simplify.h>



/***简化GPIO初始化***/
void GPIO_init(u8 GPIO,u8 PIN,u8 GPIO_Mode)
{
    switch(GPIO_Mode)
    {
        case OUT_PP:     Gpio_Init(GPIO,PIN,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);break;//推挽输出
        case OUT_OD:     Gpio_Init(GPIO,PIN,GPIO_MODE_OUTPUT,GPIO_OUTPUT_OPEN_DRAIN,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);break;//开漏输出
        case AF_PP:      Gpio_Init(GPIO,PIN,GPIO_MODE_MUX,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);break;//复用推挽输出
        case AF_OD:      Gpio_Init(GPIO,PIN,GPIO_MODE_MUX,GPIO_OUTPUT_OPEN_DRAIN,GPIO_OUTPUT_OPEN_DRAIN,GPIO_DRIVE_STRENGTH_STRONGER);break;//复用开漏输出
        case IPU:        Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_UP,GPIO_DRIVE_STRENGTH_STRONGER);break;//上拉输入
        case IPD:        Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_DOWN,GPIO_DRIVE_STRENGTH_STRONGER);break;//下拉输入
        case IN_FLOATING:Gpio_Init(GPIO,PIN,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);break;//浮空输入
        case AIN:        Gpio_Init(GPIO,PIN,GPIO_MODE_ANALOG,GPIO_OUTPUT_PUSH_PULL,GPIO_PULL_NONE,GPIO_DRIVE_STRENGTH_STRONGER);break;//模拟输入
    }
}





/***简化CAN初始化***/
void CAN_init(u8 CANx,u8 CAN_Mode,u8 CAN_SJW,u8 CAN_BS1,u8 CAN_BS2,u8 CAN_Prescaler)
{
    u8 CAN_FilterNumber=(CANx-1)*14;//CAN1用过滤器0；CAN2用过滤器14
    u8 CAN_FilterFIFOAssignment=CANx-1;//CAN1用FIFO0；CAN2用FIFO1
    
    //CAN初始化-> CAN号：CANx ， CAN模式：CAN_Mode ， 波特率=42M/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler ， 过滤器：CAN_FilterNumber ， 过滤器32位 ， 标准帧 ， ID:0 ， 掩码：0 ， 邮箱：CAN_FilterFIFOAssignment
    Can_Init(CANx,CAN_Mode,CAN_SJW,CAN_BS1,CAN_BS2,CAN_Prescaler,CAN_FilterNumber,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0,0,CAN_FilterFIFOAssignment);
}
