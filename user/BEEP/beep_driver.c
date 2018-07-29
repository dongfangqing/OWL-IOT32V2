/*********************************OWL-IOT32*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "beep_driver.h"

#ifdef DEVICE_BEEP
/*********************************************************************
******************************蜂鸣器定义*******************************
**BEEP-----PC9
**********************************************************************/
/*******************************************************
**函数功能：蜂鸣器初始化
********************************************************/
#if 1
void Beep_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	
	//蜂鸣器控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, GPIO_Pin_9);
}

void Beep_Contrl(u8 status)
{
	if(status)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
}

#else

void Beep_InitConfig(void)
{
	//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer1完全重映射  TIM1_CH1->PC6 CH2->PC7 CH1->PC8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

static void PWM_Channel4_Out(u16 pwm)
{
	TIM3->CCR4 =  pwm;
}

void Beep_Contrl(u16 pwm)
{
	PWM_Channel4_Out(pwm);
}

#endif

#endif
