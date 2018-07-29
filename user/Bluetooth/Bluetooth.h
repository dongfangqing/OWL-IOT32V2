#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_


#include "stm32f10x.h"
#include "board_config.h"
#include "TimeOut.h"

#include "pipe.h"

#define BLE_DEBUG

#define GPIO_OUT_HIGH	1
#define GPIO_OUT_LOW	0


#define BM_S02_EN(a)		if (a)	\
							GPIO_SetBits(GPIOA,GPIO_Pin_1);\
							else		\
							GPIO_ResetBits(GPIOA,GPIO_Pin_1)
						
#define BM_S02_Check_Connect_State()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

#define BM_S02_RestoreFactory(a)	if (a)	\
									GPIO_SetBits(GPIOC,GPIO_Pin_3);\
									else		\
									GPIO_ResetBits(GPIOC,GPIO_Pin_3)

									
	
									
#define BLUETOOTH_SUCCESS		0
#define BLUETOOTH_ERROR			1

#define BLUETOOTH_RETURN_STRING_SUCCESS		"TTM:OK\r\n"
#define BLUETOOTH_RETURN_STRING_SIZE			8


#define Bluetooth_Buff		Usart3_buff
									
									
extern Pipe_t Bluetooth_pipe;
extern uint8_t Bluetooth_pipe_enable;
extern TimeOut Bluetooth_TimeOut;
									
void Bluetooth_ReSetBuff(void);				
static uint8_t Bluetooth_RetString_Compare(unsigned char* buf);
uint8_t Bluetooth_RF_BM_S02_change_name(uint32_t id);	
uint8_t Bluetooth_RF_BM_S02_getMAC(void);
uint8_t Bluetooth_RF_BM_S02_change_BPS(void);
uint8_t Bluetooth_RF_BM_S02_change_Connect_time(void);
void Bluetooth_RF_BM_S02_Connect_Mode(void);
void Bluetooth_RF_BM_S02_Sleep_Mode(void);
uint8_t Bluetooth_RF_BM_S02_Check_Connect_State(void);
void Bluetooth_RF_BM_S02_RestoreFactory(int rank);

void Bluetooth_Init(void);

uint8_t IsDataPack_Check(void);

#endif
