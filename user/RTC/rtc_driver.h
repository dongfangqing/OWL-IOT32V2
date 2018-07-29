#ifndef _RTC_DRIVER_H_
#define _RTC_DRIVER_H_

#include "stm32f10x.h"

void RTC_NVIC_Config(void);


typedef enum
{
	Froenoon = 1,	//上午
	Noon,		//中午
	Afternoon	//下午
}AM_PM_DEF;


typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//公历日月年周
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}tm;


extern __IO uint32_t TimeDisplay;
extern tm timer;

void RTC_Configuration(void);
u8 Is_Leap_Year(u16 year);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Get(void);
void RTC_CheckAndConfig(tm *tm);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);

void Set_RTC_SetAlarm(u32 s);

void Set_RTC_Alarm(u8 hour,u8 min);

void RTC_init(void);




u8 Get_AMPM(void);

//u16 Get_Date_Distance(tm Date1,tm Date2);


uint8_t Judge_CalendarDate(tm *CalendarDate_val);
int8_t Judge_Timestamp(tm timer_1,tm timer_2);

#endif
