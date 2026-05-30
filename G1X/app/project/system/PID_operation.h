#ifndef	__PID_OPERATION_H__
#define __PID_OPERATION_H__
#include "at32f415.h"

typedef struct PID {
		float  Kp;         //  Proportional Const  P系数
		float  Ki;           //  Integral Const      I系数
		float  Kd;         //  Derivative Const    D系数
		
		volatile float  PrevError ;          //  Error[-2]  
		volatile float  LastError;          //  Error[-1]  
		volatile float  Error;              //  Error[0 ]  
		volatile float  DError;            //pid->Error - pid->LastError	
		volatile float  SumError;           //  Sums of Errors  
		
		volatile float  output;
		
		volatile float  Integralmax;      //积分项的最大值
		volatile float  outputmax;        //输出项的最大值
} PID;


extern PID direct_pid;

void PID_Init(PID *pid, float Kp , float Ki , float Kd , float Limit_value);
void PID_Clear(PID *pid);
void PID_Clear_I(PID *pid);
float PID_Position_Calc(PID *pid, float Target_val, float Actual_val);
#endif
