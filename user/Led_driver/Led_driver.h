#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "stm32f10x.h"
#include "board_config.h"



#ifdef DEVICE_LED

void LED_GPIO_InitConfig(void);
void LED_All_ON(void);
void LED_All_OFF(void);
void Red_LED(u16 pwm);
void Green_LED(u16 pwm);
void Blue_LED(u16 pwm);
					
					
void BreadtheLight(void);
					
#endif

#endif
