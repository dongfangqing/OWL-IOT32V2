#include "Run_task.h"
#include "Data_Structure.h"
#include "ST_string.h"
#include "Led_driver.h"
#include "beep_driver.h"
#include "TIM_driver.h"
#include "usart_driver.h"
#include "Bluetooth.h"
#include "TimeOut.h"
#include "data_pack.h"

typedef struct __arg_LedTypeDef{
	uint8_t red_led;
	uint8_t green_led;
	uint8_t blue_led;
}LedTypeDef;

static void Run_Task(void)
{
	uint8_t msg = NO_MSG;
	LedTypeDef led;
	
	#ifdef USART1_DEBUG
	printf("Run_Task\r\n");
	#endif
	
	while(1)
	{
		msg = get_msg_Fifo();
		
		switch(msg)
		{
			case MSG_IOKEY_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_IOKEY_SHORT\r\n");
				#endif
				break;
			case MSG_ADKEY_1_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_1_SHORT\r\n");
				#endif
				break;
			case MSG_ADKEY_2_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_2_SHORT\r\n");
				#endif
				break;
			case MSG_ADKEY_3_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_3_SHORT\r\n");
				#endif
				break;
			case MSG_ADKEY_4_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_4_SHORT\r\n");
				#endif
				break;
			case MSG_ADKEY_5_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_5_SHORT\r\n");
				#endif
				break;
			case MSG_DATA_PACK:
				#ifdef USART1_DEBUG
				printf("MSG_DATA_PACK\r\n");
				#endif
				{
					uint8_t buff[10];
					uint8_t i = 0;
					for(i = 0;i < 10;i++)
						pipe_read(&Bluetooth_pipe, &buff[i]);
					printf("buff:\r\n");
					for(i = 0;i < 10;i++)
						printf("%x ",buff[i]);
					if(Untie_DataPack(buff,&led.red_led,&led.green_led,&led.blue_led) == CMD_RGBLED)
						put_msg_Fifo(MSG_CONTROL_RGBLED);
				}
				break;
			case MSG_CONTROL_RGBLED:
				#ifdef USART1_DEBUG
				printf("MSG_CONTROL_RGBLED\r\n");
				#endif
				Red_LED(led.red_led);
				Green_LED(led.green_led);
				Blue_LED(led.blue_led);
				break;
			case MSG_100MS:
				break;
			case MSG_200MS:
				#ifdef USART1_DEBUG
//				printf("MSG_200MS\r\n");
				
				#endif
				break;
			case MSG_HALF_SECOND:
				break;
			case MSG_800MS:
				break;
			default:
				break;
		}
	}
}


void Run_Start(void)
{
	#ifdef USART1_DEBUG
	printf("Run_Start\r\n");
	#endif
	
	SeqQueue_Init();
	Clear_msg_Fifo();
	
	Run_Task();
}

