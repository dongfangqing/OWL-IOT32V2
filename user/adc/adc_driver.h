#ifndef _ADC_DRIVER_H_
#define _ADC_DRIVER_H_

#include "board_config.h"

#ifdef DEVICE_ADC

void ADC1_Init(void);
uint16_t Get_Adc(uint8_t channel);
float Get_AdcMath(u16 ch);




void ADC1_Disable(void);

#endif

#endif

