#include "stm32f10x.h"                  // Device header
#include "pid.h"
#include "math.h"

void PID_Init(PID_TypeDef* pid, float kp, float ki, float kd, float max, float min)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output_max = max;
    pid->output_min = min;
}

void CurveObject_Init(CurveObjectType *curve, float startSpeed, float targetSpeed, 
float stepSpeed, float speedMin, float speedMax,float flexible)
{
    curve->startSpeed = startSpeed;
    curve->currentSpeed = startSpeed;
    curve->targetSpeed = targetSpeed;
    curve->stepSpeed = stepSpeed;     //加速度
    curve->speedMin = speedMin;
    curve->speedMax = speedMax;
    curve->aTimes = 0;
    curve->maxTimes = 0;

    curve->flexible = flexible;
}



float PID_Calculate(PID_TypeDef* pid, float target, float current)
{

    pid->error = target - current;

    pid->derivative= pid->error - pid->prev_error;
    // 积分项
    pid->integral += pid->error;
    
    // 积分限幅
    if (pid->integral > pid->output_max) 
        pid->integral = pid->output_max;
    else if (pid->integral < pid->output_min) 
        pid->integral = pid->output_min;
    
    // PID计算

    pid->output = pid->kp * pid->error + pid->ki * pid->integral + pid->kd * pid->derivative;
	
    // 输出限幅
    if (pid->output > pid->output_max)
        pid->output = pid->output_max;
    else if (pid->output < pid->output_min)
        pid->output = pid->output_min;
    
    pid->prev_error = pid->error;
    
    return pid->output;
}


// 重置规划状态
void ResetCurvePlanning(CurveObjectType *curve)
{
    curve->maxTimes = 0;
    curve->aTimes = 0;
    curve->startSpeed = curve->currentSpeed; // 从当前速度重新开始
}

// 修改目标速度并重置规划
void SetTargetSpeedWithReset(CurveObjectType *curve, float newTargetSpeed)
{
    curve->targetSpeed = newTargetSpeed;
    ResetCurvePlanning(curve);
}

// 原有的规划函数保持不变
void CalCurveSPTA(CurveObjectType *curve)
{
    float temp = 0;
    
    // 限幅
    curve->currentSpeed = float_limit(curve->currentSpeed, curve->speedMin, curve->speedMax);
    
    // 计算maxTimes
    if ((float_abs(curve->currentSpeed - curve->startSpeed) <= curve->stepSpeed) && (curve->maxTimes == 0)) {
        if (curve->startSpeed < curve->speedMin) {
            curve->startSpeed = curve->speedMin;
        }
        
        temp = float_abs(curve->targetSpeed - curve->startSpeed);
        temp = temp / curve->stepSpeed;
        curve->maxTimes = (uint32_t)(temp) + 1;
        curve->aTimes = 0;
    }
    
    // 规划中
    if (curve->aTimes < curve->maxTimes) {
        float tau = (float)curve->aTimes / (float)curve->maxTimes;
        float power = curve->flexible * (8.0f * tau - 4.0f);   //这个参数可以调整
        float sigma = 1.0f / (1.0f + expf(-power));
		
		
	    float newSpeed = curve->startSpeed + (curve->targetSpeed - curve->startSpeed) * sigma;
		
		//超调抑制
		float remaining = fabsf(curve->targetSpeed - curve->currentSpeed);
        if (remaining < curve->stepSpeed * 2.0f) {
            // 在接近目标时使用线性插值，避免超调
            float blend = remaining / (curve->stepSpeed * 2.0f);
            newSpeed = blend * newSpeed + (1.0f - blend) * curve->targetSpeed;
        }
		

        curve->currentSpeed = curve->startSpeed + (curve->targetSpeed - curve->startSpeed) * sigma;
        //这个是s型规划之后的速度值
        curve->aTimes++;
    } else { // 重置速度规划
        curve->currentSpeed = curve->targetSpeed;
        curve->maxTimes = 0;
        curve->aTimes = 0;
    }
}




