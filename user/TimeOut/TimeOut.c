#include "TimeOut.h"


void TimeOut_Init(TimeOut *timeout,int timeout_val)
{
	timeout->Count = 0;
	timeout->TimeOut = timeout_val;
	timeout->status = TIMEOUT_STATUS_NO;
	timeout->cmd = TIMEOUT_DISABLE;
}

void TimeOut_Reset(TimeOut *timeout)
{
	timeout->Count = 0;
	timeout->status = TIMEOUT_STATUS_NO;
}

void TimeOut_Enable(TimeOut *timeout)
{
	TimeOut_Reset(timeout);
	timeout->cmd = TIMEOUT_ENABLE;
}

void TimeOut_Disable(TimeOut *timeout)
{
	TimeOut_Reset(timeout);
	timeout->cmd = TIMEOUT_DISABLE;
}

uint8_t TimeOut_IsEnable(TimeOut *timeout)
{
	return timeout->cmd;
}

uint8_t Timeout_Val_add(TimeOut *timeout)
{
	timeout->Count++;
	
	if(timeout->Count == timeout->TimeOut)
	{
		timeout->Count = 0;
		timeout->status++;
		return timeout->status;
	}
	return TIMEOUT_STATUS_NO;
}

void Timeout_Clear_CountVal(TimeOut *timeout)
{
	timeout->Count = 0;
}

int Timeout_Get_CountVal(TimeOut *timeout)
{
	return timeout->Count;
}

int Timeout_Get_timeoutVal(TimeOut *timeout)
{
	return timeout->TimeOut;
}

int Timeout_Get_Ststus(TimeOut *timeout)
{
	return timeout->status;
}


