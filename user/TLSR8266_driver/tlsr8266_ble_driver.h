#ifndef _TLSR8266_BLE_DRIVER_H_
#define _TLSR8266_BLE_DRIVER_H_


#include "board_config.h"

#include "pipe.h"


#define __BLE_RESET_PORT(a)	if(a)		GPIO_SetBits(GPIOC, GPIO_Pin_3);\
								else		GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#define __BLE_ENABLE_PORT(a)	if(a)		GPIO_SetBits(GPIOA, GPIO_Pin_1);\
								else		GPIO_ResetBits(GPIOA, GPIO_Pin_1)


#define __BLE_READ_RADIO_PORT()		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

#define __BLE_READ_CONNECT_PORT()		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)



#define BLE_LOW		0
#define BLE_HIGH	1

#define BLE_SUCCESS		0
#define BLE_ERROR		1

#define TLSR8266_OFF		1
#define TLSR8266_ON			0

#define BLE_Buff		Usart3_buff



#define ble_printk printf

//BLE 模组发送指令成功后返回的字符串
//#define TLSR8266_RETURN_STRING_SUCCESS		"\r\nOK\r\n"
//#define TLSR8266_RETURN_STRING_SIZE			6
#define TLSR8266_RETURN_STRING_SUCCESS		"OK\r\n"
#define TLSR8266_RETURN_STRING_SIZE			4


extern Pipe_t TLSR8266_pipe;
extern uint8_t TLSR8266_pipe_enable;


void TLSR8266_Init(void);


#endif
