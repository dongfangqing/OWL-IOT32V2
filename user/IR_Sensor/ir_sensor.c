#include "ir_sensor.h"



void IR_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}


uint8_t Get_IrDensor_status(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
}






