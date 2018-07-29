/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "key_driver.h"
#include "Data_Structure.h"
#include "usart_driver.h"
#include "RTL8710_driver.h"
#include "Bluetooth.h"
#include "TimeOut.h"
#include "tlsr8266_ble_driver.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	
}

void EXTI0_IRQHandler(void)
{
	
}

void EXTI2_IRQHandler(void)
{
	
}

void EXTI4_IRQHandler(void)
{

}

//extern IIC_Type_Def IIC_Structure;
void EXTI9_5_IRQHandler(void)
{
}

void EXTI15_10_IRQHandler(void)
{
	
}




/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART1_IRQHandler(void)
{
	
}

uint8_t ch = 0;
void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//transmission mode
	{
		USART_ClearITPendingBit (USART2, USART_IT_RXNE);
		
		ch = USART2->DR;
//		usart1_putc(ch);
		if(wifi_pipe_enable)
			pipe_write(&WIFI_RTL8710_pipe,ch);
	} 
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/


int time_out_flag = 0;
void TIM2_IRQHandler(void)
{
//	printf("TIM2_IRQHandler\r\n");
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
		ADC_Scan();
		Key_Scan();
		Timeout_Val_add(&Bluetooth_TimeOut);
		
		time_out_flag++;
		if((time_out_flag%100) == 0)
		{
			put_msg_Fifo(MSG_200MS);
		}
		
		if(IsDataPack_Check() == BLUETOOTH_SUCCESS)
		{
			put_msg_Fifo(MSG_DATA_PACK);
		}
		
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
	}
}


#if 0
void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_IDLE) != RESET)
	{
//		USART_ReceiveData(USART3);
//		usart1_sendstr("IDLE\r\n");
		Bluetooth_ReSetBuff();
		TimeOut_Enable(&Bluetooth_TimeOut);
	}
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		uint8_t ch = USART3->DR;
		
		USART_ClearITPendingBit (USART3, USART_IT_RXNE);
		Timeout_Clear_CountVal(&Bluetooth_TimeOut);
		if(TimeOut_IsEnable(&Bluetooth_TimeOut) == TIMEOUT_ENABLE)
		{
			if(Bluetooth_pipe_enable)
				pipe_write(&Bluetooth_pipe,ch);
		}
		
//		usart1_putc(ch);
//		printf("%x ",ch);
	}
}
#else

void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_IDLE) != RESET)
	{
	}
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		uint8_t ch = USART3->DR;
		
		USART_ClearITPendingBit (USART3, USART_IT_RXNE);
		
		if(TLSR8266_pipe_enable)
				pipe_write(&TLSR8266_pipe,ch);
		
		usart1_putc(ch);
//		printf("%x ",ch);
	}
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
