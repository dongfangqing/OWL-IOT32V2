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
#include "Led_driver.h"
#include "SysTick.h"
#include "TIM_driver.h"

#if 1
uint16_t LED_indexWave[10] = {0,30,50,70,100,140,180,200,240,255};
#else
uint16_t LED_indexWave[10] = {255,240,200,180,140,100,70,50,30,0};
#endif
/*********************************************************************
******************************LED灯定义*******************************
**LED_R-----PC6
**LED_G-----PC7
**LED_B-----PC8
**********************************************************************/

/**********************************************************
**函数功能：LED端口初始化
***********************************************************/
void LED_GPIO_InitConfig(void)
{
	//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer1完全重映射  TIM1_CH1->PC6 CH2->PC7 CH1->PC8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}




/**********************************************************
**函数功能：全部点亮LED
***********************************************************/
void LED_All_ON(void)
{
	Red_LED(255);
	Green_LED(255);
	Blue_LED(255);
}

/**********************************************************
**函数功能：全部熄灭LED
***********************************************************/
void LED_All_OFF(void)
{
	Red_LED(0);
	Green_LED(0);
	Blue_LED(0);
}



static void PWM_Channel1_Out(u16 pwm)
{
	TIM3->CCR1 =  pwm;
}


static void PWM_Channel2_Out(u16 pwm)
{
	TIM3->CCR2 =  pwm;
}

static void PWM_Channel3_Out(u16 pwm)
{
	TIM3->CCR3 =  pwm;
}



void Red_LED(u16 pwm)
{
	PWM_Channel3_Out(pwm);
}

void Green_LED(u16 pwm)
{
	PWM_Channel2_Out(pwm);
}

void Blue_LED(u16 pwm)
{
	PWM_Channel1_Out(pwm);
}

void BreadtheLight(void)
{
	uint8_t i = 0;
	
	for(i = 0;i < 10;i++)
	{
		PWM_Channel1_Out(LED_indexWave[i]);
		delay_ms(5000);
	}
	PWM_Channel3_Out(0);
	delay_ms(9000);
}


