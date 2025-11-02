#include "stm32f10x.h"                  // Device header
#include "VESC.h"

/**********************全局变量定义区************************************/

//电机状态等特性的定义
Chassis_MOTO MOTO[4]={0};
int first_state[4]={0,0,0,0};//一级状态（解锁或锁定）
int16_t setangle[4]={0,0,0,0};//记录锁定时的角度

/**********************全局变量定义区************************************/

/**
  * @brief  接收电机数据
  * @param  id：ID号，记得减1
  * @retval 无
  */
void VESC_RX(CanRxMsg *msg)//VESC接收处理函数
{
	uint8_t id   = ((msg->ExtId) & 0xFF)-1;   //ID减1，实际本杰明ID号：1-4；代码中电机数组的编号：0-3.

	static int offset_check[4]={0};

	switch((msg->ExtId)>>8)
	{
		case CAN_PACKET_STATUS://速度处理与储存
					MOTO[id].RPM=msg->Data[3]|(msg->Data[2]<<8)|(msg->Data[1]<<16)|(msg->Data[0]<<24);
					break;
		case CAN_PACKET_STATUS_4://位置处理与储存
					MOTO[id].NowAngle=(((msg->Data[6]<<8)|msg->Data[7])/50.0f);
					if(offset_check[id]==0)
					{
						MOTO[id].LastAngle=MOTO[id].NowAngle;
						MOTO[id].offset_angle=MOTO[id].NowAngle;
						MOTO[id].all_round=0;
						MOTO[id].offset_round=0;
						offset_check[id]=1;
					}
					if(MOTO[id].NowAngle-MOTO[id].LastAngle>=180.0f)
						MOTO[id].round_cnt--;
					if(MOTO[id].NowAngle-MOTO[id].LastAngle<-180.0f)
						MOTO[id].round_cnt++;
					
					MOTO[id].all_angle=(MOTO[id].round_cnt+(MOTO[id].NowAngle-MOTO[id].offset_angle));					
					MOTO[id].all_round=(MOTO[id].round_cnt+(MOTO[id].NowAngle-MOTO[id].offset_angle)/360.0f);
					
					MOTO[id].LastAngle=MOTO[id].NowAngle;
					
					break;
		default: return;
	}
}

void VESC_TX(uint8_t controller_id, VESC_CAN_COMMAND_ID mode,int32_t Temp_PWM1)//发送函数，暂时为CAN1发送
{
		u8 mbox;
		int i=0;
		unsigned short can_id = 0xff;
    CanTxMsg tx_message;
	
    tx_message.IDE =   CAN_ID_EXT;    //拓展帧  CAN_ID_STD CAN_ID_EXT
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.DLC = 0x08;          //帧长度为8
	
		can_id = controller_id | mode<< 8; //2020.06.09
		tx_message.StdId=0;
    tx_message.ExtId = can_id;      //帧ID为传入参数的CAN_ID
	
		if(mode==CAN_PACKET_SET_RPM)
		{
			if(Temp_PWM1 > 40000)
			{
					Temp_PWM1 = 40000;
			}
			else if(Temp_PWM1 < -40000)
			{
					Temp_PWM1 = -40000;
			}
		}
		tx_message.Data[0] = (uint8_t)((Temp_PWM1>>24)&0xff);
		tx_message.Data[1] = (uint8_t)((Temp_PWM1>>16)&0xff);
		tx_message.Data[2] = (uint8_t)((Temp_PWM1>>8)&0xff);
		tx_message.Data[3] = (uint8_t)((Temp_PWM1)&0xff);
		mbox=CAN_Transmit(CAN1,&tx_message);
		while ((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0xfff)) {i++;}
		if(i==0xfff)
		{
			
		}
	
}



/**
  * @brief  VESC多圈位置环
  * @param  id；		电机ID号；【0】-【3】
	          speed:	执行位置环时的平均速度
						pos：		位置/圈，精度：4位数
						moto[]:	对应的VESC电机结构体，id号减1
						cnt:		速度环切位置环的位置点
  * @retval 无
**/
void vesc_pos(int id ,int speed,float pos,Chassis_MOTO MOTO,float cnt)//vesc位置环  如：vesc_pos(1 ,1000,2.0,MOTO[0]);    位置环发送数据为：(200*1000000)) 乘1000000为固定格式
{
	static float i;
	static float ab_angle;
	
	if(first_state[id]==0)//处于解锁态才运动
	{
		if(pos-MOTO.all_round>cnt||pos-MOTO.all_round<-cnt)		//状态1执行，不在同一圈里
		{
			if(pos>=MOTO.all_round)
				VESC_TX(id+1,CAN_PACKET_SET_RPM,speed);//speed原本没有负号
			
			else if(pos<MOTO.all_round)
				VESC_TX(id+1,CAN_PACKET_SET_RPM,-speed);//speed原本有负号
		}
		else  //这是有什么不同？
		{
			i=pos;
			i=i-(int)i;
			ab_angle=i*360;
			setangle[id]=MOTO.offset_angle+ab_angle;
			if(setangle[id]>360)setangle[id]=setangle[id]-360;
			if(setangle[id]<0) setangle[id]=360-setangle[id];
			
			VESC_TX(id+1,CAN_PACKET_SET_POS,((1000000)*((int32_t)setangle[id])));

			first_state[id]=1;//状态更新为锁定态
		}
	}	
	else//处于锁定态
	{
		VESC_TX(id+1,CAN_PACKET_SET_POS,((1000000)*((int32_t)setangle[id])));//锁死电机
	}
}


/**
  * @brief  锁死VESC电机
  * @param  id；电机ID号；【0】-【3】
						moto[]:对应的VESC电机结构体0-3
  * @retval 无
**/
//void vesc_lock(int id,Chassis_MOTO*MOTO)//锁死电机在当前的位置
//{
//	extern int pos_flag;

//	if(first_state[id]==0)
//	{
//		setangle[id]=MOTO->NowAngle;
//		first_state[id]=1;//状态更新为锁定态
//		MOTO->offset_round=MOTO->all_round;
////		DMA_Printf(USART1,"id:%d\r\n",id);
//		if(id==0) 
//		{
//			pos_flag++;
//		}
//	}
//	VESC_TX(id+1,CAN_PACKET_SET_POS,(int32_t)((1000000)*(setangle[id])));//记得把ID号加回来
//}

















