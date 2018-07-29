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
#include "spi_driver.h"
#include "usart_driver.h"
#include "SysTick.h"

/**********************************************************
**************************引脚定义*************************
**SPIFlash device
**SPI_NSS:			PA4
**SPI_CLK:			PA5
**SPI_MISO:			PA6
**SPI_MOSI:			PA7
***********************************************************/
#ifdef DEVICE_SPI


#ifdef DEVICE_SPIFLASH
/******************************************************
**函数功能：SPI初始化
*******************************************************/
void SPIflash_SPI_InitConfig(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//NSS   CLK    MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	SPI_NSS_HIGH();
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 			
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  			
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	// Enable SPI1  
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_RW(uint8_t dat)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	// 通过 SPI2发送一字节数据 
	SPI_I2S_SendData(SPI1, dat);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}
#endif

#ifdef DEVICE_OLED
/**********************************************************
**************************引脚定义*************************
**SCLK(SCL)----->GPIOC4
**MOSI(SDA)----->GPIOC5
***********************************************************/
void OLED_SPI_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
}

#define OLED_SCLK_Clr()		GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define OLED_SCLK_Set()		GPIO_SetBits(GPIOC, GPIO_Pin_4)

#define OLED_SDIN_Clr()		GPIO_ResetBits(GPIOC, GPIO_Pin_5)	
#define OLED_SDIN_Set()		GPIO_SetBits(GPIOC, GPIO_Pin_5)


void OLED_SPI_WriterByte(uint8_t dat)
{	
	u8 i;			  

	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		delay_us(10);
		if(dat&0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		delay_us(10);
		OLED_SCLK_Set();
		delay_us(10);
		dat<<=1;   
	}
}
#endif


#ifdef DEVICE_RFID
/**********************************************************
**************************引脚定义*************************
**RFID RC522 device
**SPI_NSS:			PA12		SDA
**SPI_CLK:			PA11		SCK
**SPI_MISO:			PC2			
**SPI_MOSI:			PB8
***********************************************************/

#define RF_CLK_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define RF_CLK_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define RF_MOSI_Clr()		GPIO_ResetBits(GPIOB, GPIO_Pin_8)	
#define RF_MOSI_Set()		GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define RF_NSS_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_12)	
#define RF_NSS_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define RF_MISO_READ()		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)

void RFID_SPI_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	RF_CLK_Clr();
	RF_CLK_Set();
	RF_NSS_Set();
}

#if 0
//CPOL: Low
//CPHA: 1Edge
uint8_t RFID_SPI_WriterByte(uint8_t dat)
{	
	uint8_t i;
	uint8_t rx_data = 0;
	
	for(i = 0;i < 8;i++)
	{
		RF_CLK_Set();
		rx_data <<= 1;
		if(RF_MISO_READ())
		{
			rx_data |= 1;
		}
		
		RF_CLK_Clr();
		if(dat&0x80)
		{
			RF_MOSI_Set();
		}
		else	RF_MOSI_Clr();
		dat <<= 1;
	}
	
	return rx_data;
}
#else
//CPOL: Hig
//CPHA: 2Edge
uint8_t RFID_SPI_WriterByte(uint8_t dat)
{	
	uint8_t i;
	uint8_t rx_data = 0;
//	RF_CLK_Set();
	for(i = 0;i < 8;i++)
	{
		RF_CLK_Clr();
		if(dat&0x80)
		{
			RF_MOSI_Set();
		}
		else	RF_MOSI_Clr();
		dat <<= 1;
		
		RF_CLK_Set();
		rx_data <<= 1;
		if(RF_MISO_READ())
		{
			rx_data |= 1;
		}
	}
	
	return rx_data;
}

#endif

#endif

/******************************************************
**函数功能：SPI初始化
*******************************************************/
void SPI_InitConfig(void)
{
#ifdef DEVICE_SPIFLASH
	SPIflash_SPI_InitConfig();
#endif
	
#ifdef DEVICE_OLED
	OLED_SPI_InitConfig();
#endif

#ifdef DEVICE_RFID
	RFID_SPI_InitConfig();
#endif
}

#endif

