#ifndef _TIMEOUT_H_
#define _TIMEOUT_H_


#include "stm32f10x.h"

#define TIMEOUT_STATUS_NO	0		//未超时状态

#define TIMEOUT_ENABLE		0
#define TIMEOUT_DISABLE		1

typedef struct __arg_TimeOut
{
	int Count;	//计数值
	int TimeOut;//超时值
	int status;//超时状态  次数
	uint8_t cmd;//使能/失能
}TimeOut;


void TimeOut_Init(TimeOut *timeout,int timeout_val);
void TimeOut_Reset(TimeOut *timeout);
void TimeOut_Enable(TimeOut *timeout);
void TimeOut_Disable(TimeOut *timeout);
uint8_t TimeOut_IsEnable(TimeOut *timeout);
uint8_t Timeout_Val_add(TimeOut *timeout);
void Timeout_Clear_CountVal(TimeOut *timeout);
int Timeout_Get_CountVal(TimeOut *timeout);
int Timeout_Get_timeoutVal(TimeOut *timeout);
int Timeout_Get_Ststus(TimeOut *timeout);

#endif

