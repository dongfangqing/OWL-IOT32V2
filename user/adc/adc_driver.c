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
#include "adc_driver.h"


#ifdef DEVICE_ADC

/******************************************************************
**ADC端口:
**采集端口:ADC1  通道10-PC0
**地端口:PB14-----启用时PB14输出低电平
*******************************************************************/

#define ADC1_DR_Address    ((u32)0x40012400+0x4c) //(uint32_t)&ADC1->DR

__IO uint16_t ADC_ConvertedValue[50][2];//用来存放ADC转换结果，也是DMA的目标地址
__IO uint16_t After_filter[2];    //用来存放求平均值之后的结果


/*******************************************
**函数功能：配置ADC1通道10-PC0,11-PC1
********************************************/
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

	
	/* Configure PC.0  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  配置ADC1的工作模式为MDA模式
  * @param  无
  * @retval 无
  */
static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// DMA channel1 configuration 
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;	//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 50*2;	//每通道采集50次,2个通道
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//数据宽度为半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// Enable DMA channel0 
	DMA_Cmd(DMA1_Channel1, ENABLE);

	// ADC1 configuration 	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 	//扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 			//要转换的通道数目2
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道10,11为239.	5个采样周期，序列为1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*****************************************************
**函数功能：ADC1初始化
******************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


//求平均值函数
void filter(u8 num)
{
	int  sum = 0;
	u8  count;    

	for(count=0;count<50;count++)
	{
		sum += ADC_ConvertedValue[count][num];
	}
	After_filter[num]=sum/50;
}

/*
**函数功能：获取X通道AD值（0~1023）
**参数    ：channel---模拟口通道号
**参数取值：	10~11
*/
uint16_t Get_Adc(uint8_t channel)
{
	uint8_t ch = 0;
	
	if(channel == 10)
		ch = 0;
	else if(channel == 11)
		ch = 1;
	else return 0;
	
 	filter(ch);
	return 	After_filter[ch];	  	
}



/***************************************
**函数功能：获取采集电压值
**公	式：V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_AdcMath(u16 ch)
{
	u16 temp = ch;
	float Value;

//	Value = temp * 3.3;
//	Value = Value / (0x0FFF + 1);


	Value = (float)temp * (3.3 / 4096);

	return Value;
}

#endif

