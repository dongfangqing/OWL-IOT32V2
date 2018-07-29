#ifndef __DHT11_DRIVER_H
#define	__DHT11_DRIVER_H

#include "board_config.h"
#include "SysTick.h"

#ifdef DEVICE_DHT11

#define HIGH  1
#define LOW   0


//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DHT11_DATA_OUT(a)	if(a) GPIO_SetBits(GPIOA,GPIO_Pin_8);\
					else GPIO_ResetBits(GPIOA,GPIO_Pin_8)
 //读取引脚的电平
#define  DHT11_DATA_IN()	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

#endif

#endif







