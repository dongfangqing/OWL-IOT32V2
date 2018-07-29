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
#include "spiflash_driver.h"
#include "spi_driver.h"



#ifdef DEVICE_SPIFLASH

/*********************************************************************************************
*******************************************SPI-Flash******************************************
**********************************************************************************************/
#define SPI_FLASH_PageSize      			256
#define SPI_FLASH_PerWritePageSize      	256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      	0x06 
#define W25X_WriteDisable		      	0x04 
#define W25X_ReadStatusReg		    	0x05 
#define W25X_WriteStatusReg		    	0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      	0x0B 
#define W25X_FastReadDual		      	0x3B 
#define W25X_PageProgram		      	0x02 
#define W25X_BlockErase			      	0xD8 
#define W25X_SectorErase		      	0x20 
#define W25X_ChipErase			      	0xC7 
#define W25X_PowerDown			      	0xB9 
#define W25X_ReleasePowerDown	    	0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   		0x90 
#define W25X_JedecDeviceID		    	0x9F 

#define WIP_Flag                  		0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                		0xFF

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_RW(0xff));
}


/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "Write Enable" instruction */
	SPI_RW(W25X_WriteEnable);

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;

	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "Read Status Register" instruction */
	SPI_RW(W25X_ReadStatusReg);

	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		FLASH_Status = SPI_RW(Dummy_Byte);	 
	}
	while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "Power Down" instruction */
	SPI_RW(W25X_PowerDown);

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();
}   

//唤醒
void SPI_Flash_WAKEUP(void) 
{
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "Power Down" instruction */
	SPI_RW(W25X_ReleasePowerDown);

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();                   //等待TRES1
}


/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "Read from Memory " instruction */
	SPI_RW(W25X_ReadData);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	SPI_RW((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	SPI_RW((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	SPI_RW(ReadAddr & 0xFF);
}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadDeviceID(void)
{
	uint32_t Temp = 0;

	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "RDID " instruction */
	SPI_RW(W25X_DeviceID);
	SPI_RW(Dummy_Byte);
	SPI_RW(Dummy_Byte);
	SPI_RW(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp = SPI_RW(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();

	return Temp;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "RDID " instruction */
	SPI_RW(W25X_JedecDeviceID);

	/* Read a byte from the FLASH */
	Temp0 = SPI_RW(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp1 = SPI_RW(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp2 = SPI_RW(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();

	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();

	/* Send "Read from Memory " instruction */
	SPI_RW(W25X_ReadData);

	/* Send ReadAddr high nibble address byte to read from */
	SPI_RW((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte to read from */
	SPI_RW((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SPI_RW(ReadAddr & 0xFF);

	while (NumByteToRead--) /* while there is data to be read */
	{
	/* Read a byte from the FLASH */
	*pBuffer = SPI_RW(Dummy_Byte);
	/* Point to the next location where the byte read will be saved */
	pBuffer++;
	}

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();
}


/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	/* Enable the write access to the FLASH */
	SPI_FLASH_WriteEnable();

	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();
	/* Send "Write to Memory " instruction */
	SPI_RW(W25X_PageProgram);
	/* Send WriteAddr high nibble address byte to write to */
	SPI_RW((WriteAddr & 0xFF0000) >> 16);
	/* Send WriteAddr medium nibble address byte to write to */
	SPI_RW((WriteAddr & 0xFF00) >> 8);
	/* Send WriteAddr low nibble address byte to write to */
	SPI_RW(WriteAddr & 0xFF);

	if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		//printf("\n\r Err: SPI_FLASH_PageWrite too large!");
	}

	/* while there is data to be written on the FLASH */
	while (NumByteToWrite--)
	{
		/* Send the current byte */
		SPI_RW(*pBuffer);
		/* Point on the next byte to be written */
		pBuffer++;
	}

	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();

	/* Wait the end of Flash writing */
	SPI_FLASH_WaitForWriteEnd();
}


/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			while (NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}

			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
			{
				temp = NumOfSingle - count;

				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count;

				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;

			while (NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}

			if (NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}


/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
	/* Send write enable instruction */
	SPI_FLASH_WriteEnable();

	/* Bulk Erase */
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();
	/* Send Bulk Erase instruction  */
	SPI_RW(W25X_ChipErase);
	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();

	/* Wait the end of Flash writing */
	SPI_FLASH_WaitForWriteEnd();
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	/* Send write enable instruction */
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	/* Sector Erase */
	/* Select the FLASH: Chip Select low */
	SPI_NSS_LOW();
	/* Send Sector Erase instruction */
	SPI_RW(W25X_SectorErase);
	/* Send SectorAddr high nibble address byte */
	SPI_RW((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	SPI_RW((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	SPI_RW(SectorAddr & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	SPI_NSS_HIGH();
	/* Wait the end of Flash writing */
	SPI_FLASH_WaitForWriteEnd();
}



#endif

