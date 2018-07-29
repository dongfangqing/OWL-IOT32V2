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
#include "key_driver.h"
#include "SysTick.h"
#include "Data_Structure.h"
#include "adc_driver.h"

/**********************************************************
**************************引脚定义*************************
**IO KEY:			PA0
**AD KEY:			PC1
***********************************************************/


void Key_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Get_KeyStatus(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}


uint16_t adkey_value = 0;

uint16_t adkey_check_table[] = {ADKEY_V_0, ADKEY_V_1, ADKEY_V_2, ADKEY_V_3, ADKEY_V_4};



uint8_t iokey_table[4][3] = {
	{IOKEY_SHORT},
	{IOKEY_LONG},
	{IOKEY_HOLD},
	{IOKEY_LONG_UP}
};

uint8_t adkey_table[4][5] = {
	{ADKEY_SHORT},
	{ADKEY_LONG},
	{ADKEY_HOLD},
	{ADKEY_LONG_UP}
};



/****************************************************************************/

uint8_t io_key(void)
{
	uint8_t keyTemp;
	
	keyTemp = NO_KEY;
	
	if(Get_KeyStatus() == KEY_DOWN)
	{
		keyTemp = 0;
		return keyTemp;
	}
	
	return NO_KEY;
}

void ADC_Scan(void)
{
	static uint8_t cnt;
	
	if(cnt == 0)
	{
	}
	else if(cnt == 1)
	{
	}
	else if(cnt == 2)
	{
		adkey_value = Get_Adc(11);
//		usart1_putc('H');
	}
	else
	{
		cnt = 0;
	}
	
	cnt++;
	if(cnt >= 3)
		cnt = 0;
}

uint8_t adkey(u16 key_value)
{
	uint8_t i = 0;
	if(key_value > ADKEY_MAX)
	{
		return NO_KEY;
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if(key_value < adkey_check_table[i])
			{
				break;
			}
		}
	}
	return i;
}


uint8_t Key_mode = 0;
uint8_t key_counter;
uint8_t last_key = NO_MSG;
uint8_t msg_key = NO_MSG;
void Key_Scan(void)
{
	uint8_t keyTemp = NO_KEY;
	uint8_t key_style = NO_KEY;
	
	keyTemp = adkey(adkey_value);
	
	if(keyTemp == NO_KEY)
	{
		//Get IO Key Value
		keyTemp = io_key();
		if(keyTemp != NO_KEY)
		{
			Key_mode = MODE_IOKEY;	//IO Key
		}
	}
	else
		Key_mode = MODE_ADKEY;	//AD Key
	
	if(NO_KEY == keyTemp)
	{
		if (key_counter >= KEY_LONG_CNT)
		{
			key_style = 3;
		}
		else if (key_counter >= KEY_BASE_CNT)
		{
			key_style = 0;
		}

		key_counter = 0;
	}
	else if (keyTemp != last_key)
	{ 
		key_counter = 0;
	}
	else
	{
		key_counter++;
		if (KEY_BASE_CNT == key_counter)
		{
		}
		else if (KEY_LONG_CNT == key_counter)
		{
			key_style = 1;
		}
		else if ((KEY_LONG_CNT + KEY_HOLD_CNT) == key_counter)
		{
			key_style = 2;
			key_counter = KEY_LONG_CNT;
		}
	}
	
	if((NO_KEY != last_key) && (key_style != NO_KEY))
	{
		if(Key_mode == MODE_ADKEY)//AD Key
		{
			msg_key = adkey_table[key_style][last_key];
		}
		else if(Key_mode == MODE_IOKEY)//IO Key
		{
			msg_key = iokey_table[key_style][last_key];
		}

		if (msg_key == NO_MSG)
			goto _exit_keyScan;

			if(msg_key == MSG_IOKEY_LONG_UP)
				Clear_msg_Fifo();
			put_msg_Fifo(msg_key);
			msg_key = NO_MSG;
	}
	
_exit_keyScan:
	last_key = keyTemp;
}


