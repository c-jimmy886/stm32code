#include<init.h>
#include<sys.h>
#include<rm_motor.h>
#include<my_library.h>




RM_RX_DATA*RM_Rx_Data;//RM电机接收数据存储

u8 RM_Rx_Len;//RM电机接收数据存储长度
u8 RM_CANx;


/* 大疆电机使用：
1，先定义结束数据数组RM_RX_DATA CAN2_RM_BUF[8]
2, 先初始化RM_init();注意Buf_Len等于需要读取电机得个数，同时在Buf_Len得数组定义也要足够多
3，can接收函数，确定使用哪个can，确定can得回调函数
4，回调函数要在main里面装载
5，在回调函数调用RM_Rx,此时，CAN2_RM_BUF数组得到对应电机的数据
6，可以定义一个新的结构体，将读取到的数据放入结构体
7，调用需要的pid，注意电机的pid和接收数据一定要对其，不要混了
8，pid计算后，调用RM_Tx发送数据

*/


//RM_Rx_Buf：RM电机接收数据存储
//Buf_Len：RM电机接收数据存储长度
void RM_Init(u8 CANx,RM_RX_DATA*RM_Rx_Buf,u8 Buf_Len)//RM电机CAN初始化
{
    if(CANx==1)
    {
        Can_Init(CANx,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS2_2TQ,15,0,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0x00000000,0x00000000,CAN_FILTER_FIFO0);
        //CAN初始化：CAN1，普通模式，波特率120/（1+5+2）/15=1M,，过滤器0，过滤器宽32bit，使用标准id，ID:0x00000000，掩码:0x00000000，过滤器关联邮箱FIFO0
        CAN1_RX0_IRQ=&RM_Rx;//将RM电机接收函数通过函数指针装载到CANx中断服务函数
    }
    if(CANx==2)
    {
        Can_Init(CANx,0,CAN_RSAW_2TQ,CAN_BTS1_5TQ,CAN_BTS2_2TQ,15,13,CAN_FILTER_32BIT,CAN_Id_Mode_StdId,0x00000000,0x00000000,CAN_FILTER_FIFO1);
        //CAN初始化：CAN2，普通模式，波特率120/（1+5+2）/15=1M,，过滤器0，过滤器宽32bit，使用标准id，ID:0x00000000，掩码:0x00000000，过滤器关联邮箱FIFO1
        CAN2_RX1_IRQ=&RM_Rx;
    }
    RM_Rx_Data=RM_Rx_Buf;//得到电机数据缓存区（通过这个将RM_Rx_Buf的地址赋值给RM_Rx_Data,并且是通过指针传参
    RM_Rx_Len=Buf_Len;//得到电机数据缓存区长度
    RM_CANx=CANx;//得到CAN号
}

//发送的can要和接收的can一致，can1对应can1，can2对应can2
void RM_Tx(uint32_t  id,u8 canx,int current1,int current2,int current3,int current4)//发送四路电机电流值
{
    u8 data[8];
    data[0]=(current1&0xff00)>>8;
    data[1]=(current1&0x00ff);
    data[2]=(current2&0xff00)>>8;
    data[3]=(current2&0x00ff);
    data[4]=(current3&0xff00)>>8;
    data[5]=(current3&0x00ff);
    data[6]=(current4&0xff00)>>8;
    data[7]=(current4&0x00ff);
	
    Can_Tx(canx,CAN_ID_STANDARD,id,data,8,0);//启动CAN发送（使用标准ID，ID为RM_ID，数据为八字节data，数据长度为8，不发送换行）
	
}

/*
C620 ID：
    1~4：0x200
    5~8：0x1FF

6020 ID：
    1~4：0x1FF
    5~7：0x2FF
*/

/*
    C620 Tx：-10000~10000
    6020 Tx：-25000~25000
*/


static uint8_t zero_set_flags[10] = {0};

//id：电机ID
//buf：接收数据缓存
void RM_Rx(uint32_t id,u8*buf)
{
    u8 i;

   for(i=0;i<RM_Rx_Len;i++)
   {
		if((RM_Rx_Data+i)->id==id)
		{
			(RM_Rx_Data+i)->pos=(uint16_t)(buf[0]<<8|buf[1]);//得到位置
			(RM_Rx_Data+i)->vel=(uint16_t)(buf[2]<<8|buf[3]);//得到速度
			(RM_Rx_Data+i)->cur=(uint16_t)(buf[4]<<8|buf[5]);//得到电流
			
			//多圈位置统计
			if((RM_Rx_Data+i)->last_pos - (RM_Rx_Data+i)->pos>RM_POS_MAX/2)(RM_Rx_Data+i)->round++;
			if((RM_Rx_Data+i)->last_pos - (RM_Rx_Data+i)->pos<-RM_POS_MAX/2)(RM_Rx_Data+i)->round--;
			(RM_Rx_Data+i)->place = (RM_Rx_Data+i)->round * 8192 + (RM_Rx_Data+i)->pos;
			(RM_Rx_Data+i)->last_pos=(RM_Rx_Data+i)->pos;
			
			if(i<=8&&!zero_set_flags[i])
			{
				zero_set_flags[i] = 1;
			   (RM_Rx_Data+i)->zero_angle=(RM_Rx_Data+i)->pos;
			}

			return;
		}
	
	}

}

/*
C620 ID：
    1~8：0x200+id

6020 ID：
    1~7：0x204+id
*/







//单圈位置最短距离解算
//mb_place：目标位置
//mq_place：目前位置
int RM_One_Round(int mb_place,int mq_place)
{
    int place;//最短距离相对位置
    
    if(my_abs(mb_place-mq_place) <= (RM_POS_MAX+1)-my_abs(mb_place-mq_place))place=mq_place;//正向距离小于反向距离，向正向转
    else place=mb_place + ((RM_POS_MAX+1)-my_abs(mb_place-mq_place))*(mb_place>mq_place ? 1 : -1);//正向距离大于于反向距离，向反向转
    
    return place;//返回最短距离相对位置
}



/*---------------------------------------------------------------*///空载速度pid参数

//pid_init(&pid,10,0.002,20);

/*---------------------------------------------------------------*///空载串级pid参数

//pid_init(&pids,10,0.05,5);
//pid_init(&pidp,0.18,0,0.2);
//delay_ms(2);

//pid_init(&pids,10,0.125,2);
//pid_init(&pidp,0.18,0,0.08); 
//delay_ms(5);

//pid_init(&pids,10,2.0,2.0);
//pid_init(&pidp,0.18,0,0.8);
//delay_ms(5);


//pid_init(&pids,10.0,1.2,1 );(2006)
//pid_init(&pidp,0.12,0,0.4);
//delay_ms(5)
/*---------------------------------------------------------------*///空载位置_速度pid参数

//pid_init(&pidp,1,0,10);
//pid_init(&pids,1.5,0.02,4);
//delay_ms(2);

/*---------------------------------------------------------------*///空载位置_速度参数

//pid_pos_vel(&pid,8,1.1,2,20,8000,DATA.vel,num,DATA.place,17000);
