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
#include "TIM_driver.h"




#ifdef DEVICE_TIM2
/***********************************************************************
**函数功能：定时器2初始化配置，产生1s中断一次的计时
**计	算：计数器的时钟频率CK_CNT = f(CK_PSC)/(PCS[15:0]+1)
**			计数器的计时频率CK_CNT = 72MHz/(7199+1) = 10KHz
**			单次计数时间 	T(CNT) = 1/CK_CNT = 100us
**			定时器溢出时间	Tout = ((CNT[15:0] + 1)*[PSC[15:0]+1])/Tclk
**			定时器溢出时间	Tout = ((9999 + 1)*(7199 + 1))/72MHz = 1s
**			定时器溢出时间	Tout = ((19 + 1)*(7199 + 1))/72MHz = 2ms
**********************************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 19;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);	
}


/*************************************************************
**函数功能：定时器2中断控制器初始化配置
**************************************************************/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
#endif

#ifdef DEVICE_TIM3
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* ----------------------------------------------------------------------- 
    TIM2 Channe2 duty cycle = (TIM2_CCR2/ TIM2_ARR+1)* 100% = PWM占空比
  ----------------------------------------------------------------------- */
	// Time base configuration 	 
	TIM_TimeBaseStructure.TIM_Period = 255;       //当定时器从0计数到255，即为256次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    //设置预分频：2000预分频，即为36KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channe2 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	#ifdef DEVICE_LED
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 //使能通道2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	#endif
	
	#ifdef DEVICE_BEEP
//	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道4
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	#endif

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	// TIM4 enable counter 
	TIM_Cmd(TIM3, ENABLE);                   //使能定时器3

//	TIM3->BDTR|=0x8000;  //使能定时器3输出

	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
}
#endif

void TIM_InitConfig(void)
{
#ifdef DEVICE_TIM2
	TIM2_Configuration();
	TIM2_NVIC_Configuration();
#endif
#ifdef DEVICE_TIM3
	TIM3_Configuration();
#endif
}




