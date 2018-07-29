#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include "stm32f10x.h"

void Display_Date(uint16_t w_year,uint8_t w_month,uint8_t w_date);

void Display_Time(uint8_t hour,uint8_t min,uint8_t sec);

#if 0
void Display_Temperature(uint8_t temp_int,uint8_t temp_deci);
#else
void Display_Temperature(uint8_t temp_int);
#endif

#if 0
void Display_humidity(uint8_t humi_int,uint8_t humi_deci);
#else
void Display_humidity(uint8_t humi_int);
#endif




#endif


