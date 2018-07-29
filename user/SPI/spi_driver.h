#ifndef _SPI_DRIVER_H_
#define _SPI_DRIVER_H_

#include "board_config.h"

#ifdef DEVICE_SPI

void SPI_InitConfig(void);


#ifdef DEVICE_SPIFLASH

#define SPI_NSS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_NSS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)

uint8_t SPI_RW(uint8_t dat);

#endif
	
#ifdef DEVICE_OLED
	void OLED_SPI_WriterByte(u8 dat);
#endif

#ifdef DEVICE_RFID

uint8_t RFID_SPI_WriterByte(uint8_t dat);

#endif

#endif
#endif

