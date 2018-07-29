#ifndef _SPIFLASH_DRIVER_H_
#define _SPIFLASH_DRIVER_H_


#include "board_config.h"



#ifdef DEVICE_SPIFLASH

#define SPI1_MASTER_MODE



/*********************************************************************************************
*****************************************Master Mode******************************************
**********************************************************************************************/
void SPI1_InitConfig(void);
uint8_t SPI_RW(uint8_t byte);
uint16_t SPI_RWHalfWord(uint16_t HalfWord);


/*********************************************************************************************
**************************************SPI_Flash define****************************************
**********************************************************************************************/

#define  sFLASH_ID              0XEF4017 		//W25Q64


uint8_t SPI_FLASH_ReadByte(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
uint32_t SPI_FLASH_ReadDeviceID(void);
uint32_t SPI_FLASH_ReadID(void);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);



#endif

#endif
