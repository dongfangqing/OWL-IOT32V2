#include "data_pack.h"
#include "ST_string.h"
#include "led_driver.h"
#include "usart_driver.h"



uint8_t Untie_DataPack(uint8_t buff[],uint8_t *red_led,uint8_t* green_led,uint8_t* blue_led)
{
	uint16_t lenth = 0;
	uint16_t i = 0;
	uint8_t bcc_data = 0;
#if 1	
	if((buff[0] == 0xff)&&(buff[1] == 0xff))
	{		
		lenth = 0xffff&((buff[2]<<8)|buff[3]);
		
		bcc_data = buff[2];
		for(i = 0;i < lenth;i++)
		{
			bcc_data = bcc_data^buff[i+3];
		}
//		usart1_putc(bcc_data);
		if(bcc_data != buff[lenth+3])
		{
			printf("crc error!\n");
			return 0;
		}
		else
		{
			
			if(buff[4] == CMD_RGBLED)
			{
				*red_led = buff[5];
				*green_led = buff[6];
				*blue_led = buff[7];
			}
		}
		
		return buff[4];
	}
	printf("data error\r\n");
#endif
	return 0;
}



