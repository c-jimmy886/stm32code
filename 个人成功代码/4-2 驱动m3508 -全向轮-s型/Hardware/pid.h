#ifndef __PID_H
#define __PID_H

typedef struct {
    float kp;
    float ki;
    float kd;
    float integral;
    float prev_error;
    float output_max;
    float output_min;
	
	//速度pid数值
	float error;
	float derivative;
	float output;
	
} PID_TypeDef;


typedef struct
{
	float startSpeed;         //开始调速时的初始速度
	float currentSpeed;       //当前速度
	float targetSpeed;        //目标速度
	float stepSpeed;          //加速度
	float speedMax;           //最大速度
	float speedMin;           //最小速度
	uint32_t aTimes;          //调速时间
	uint32_t maxTimes;        //调速跨度
//	SpeedCurveType curveMode; //曲线类型
	float flexible;           //S型曲线拉伸度
	
	
	
}CurveObjectType;


void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float max, float min);
float PID_Calculate(PID_TypeDef* pid, float target, float current);
void CurveObject_Init(CurveObjectType *curve, float startSpeed, float targetSpeed, 
float stepSpeed, float speedMin, float speedMax,float flexible);
void CalCurveSPTA(CurveObjectType *curve);


void ResetCurvePlanning(CurveObjectType *curve);
void SetTargetSpeedWithReset(CurveObjectType *curve, float newTargetSpeed);
void CalCurveSPTA(CurveObjectType *curve);


#endif

