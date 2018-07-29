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
#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include "stm32f10x.h"

#define USART1_DEBUG

#define ON  0
#define OFF 1

#define DEVICE_RCC
#define DEVICE_SYSTICK
#define DEVICE_TIM2
#define DEVICE_TIM3
#define DEVICE_LED
#define DEVICE_BEEP
#define DEVICE_USART
#define DEVICE_SPI
#define DEVICE_I2C
#define DEVICE_ADC

#define DEVICE_OLED
#define DEVICE_EEPROM
#define DEVICE_DHT11
#define DEVICE_SPIFLASH

#define DEVICE_KEY

#define DEVICE_RFID
#define DEVICE_BMA250E





#endif
