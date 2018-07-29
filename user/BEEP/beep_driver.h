#ifndef _BEEP_DRIVER_H_
#define _BEEP_DRIVER_H_


#include "stm32f10x.h" 
#include "board_config.h"

#ifdef DEVICE_BEEP


void Beep_InitConfig(void);

#if 1
void Beep_Contrl(u8 status);
#else
void Beep_Contrl(u16 pwm);
#endif

#endif

#endif


