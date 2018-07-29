#include "display.h"
#include "OLED_Device.h"
#include "dht11_driver.h"
#include "adc_driver.h"
#include "rtc_driver.h"




void Display_Date(uint16_t w_year,uint8_t w_month,uint8_t w_date)
{
	uint8_t buf[] = "2000-00-00";
	uint8_t year;
	
	year = w_year%100;
	buf[2] = year/10 + '0';
	buf[3] = year%10 + '0';
	
	buf[5] = w_month/10 +'0';
	buf[6] = w_month%10 + '0';
	
	buf[8] = w_date/10 + '0';
	buf[9] = w_date%10 + '0';
	
	LCD_P8x16Str(20,2,buf);
}

void Display_Time(uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t buf[] = "00:00:00";
	
	buf[0] = hour/10 + '0';
	buf[1] = hour%10 + '0';
	
	buf[3] = min/10 +'0';
	buf[4] = min%10 + '0';
	
	buf[6] = sec/10 + '0';
	buf[7] = sec%10 + '0';
	
	LCD_P8x16Str(25,4,buf);
}
#if 0
void Display_Temperature(uint8_t temp_int,uint8_t temp_deci)
#else
void Display_Temperature(uint8_t temp_int)
#endif
{
	uint8_t buf[] = "T:00C";
	
	buf[2] = temp_int/10 + '0';
	buf[3] = temp_int%10 + '0';
	LCD_P8x16Str(5,6,buf);
}

#if 0
void Display_humidity(uint8_t humi_int,uint8_t humi_deci)
#else
void Display_humidity(uint8_t humi_int)
#endif
{
	uint8_t buf[] = "H:00%RH";
	
	buf[2] = humi_int/10 + '0';
	buf[3] = humi_int%10 + '0';
	LCD_P8x16Str(50,6,buf);
}

void Display_light(uint8_t Val)
{
}


