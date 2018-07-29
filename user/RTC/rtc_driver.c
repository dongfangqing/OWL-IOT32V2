/*********************************iBoxV300*********************************                                      
 
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
#include "stm32f10x.h" 
#include "rtc_driver.h"
#include "usart_driver.h"


/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;

/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置RTC秒中断的主中断优先级为1，次优先级为0
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_Alarm_EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}



//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

tm timer;


/*
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset Backup Domain */
//	BKP_DeInit();	//每次都初始化BKP的话，会将备份寄存器清空，不会保存值，所以，初始化一次之后，把它注释掉

	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);

#ifdef USART1_DEBUG	
	printf("Wait till LSE is ready\r\n");
#endif
	
	/* Wait till LSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}
#ifdef USART1_DEBUG	
	printf(" Out Wait till LSE is ready\r\n");
#endif
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Alarm */  
	RTC_ITConfig(RTC_IT_ALR, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}
/************************************************
**函数功能：判断输入的年份是否为闰年
**返回	值：0-----不是闰年	1-----是闰年
*************************************************/
u8 Is_Leap_Year(u16 year)
{			  
			   
	if(year%4 == 0) //必须能被4整除
	{ 
		if(year%100 == 0) 
		{ 
			if(year%400 == 0)
				return 1;//如果以00结尾,还要能被400整除 	   
			else 
				return 0;   
		}
		else 
			return 1;   
	}
	else 
		return 0;	
}	


/*********************************************************
**函数功能：设置RTC时间,把输入的时钟转换为秒钟
**			以1970年1月1日为基准,1970~2099年为合法年份
**返回	值:0,成功;其他:错误代码.
**********************************************************/
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{		   
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)
		return 1;	   
	
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))
			seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	
	smon-=1;
	
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE); 
	PWR_BackupAccessCmd(ENABLE);
	printf("SETUP Wait\r\n");
	//设置时钟
    RTC_WaitForLastTask(); 
	RTC_SetCounter(seccount);
    RTC_WaitForLastTask();
	printf("SETUP succeed\r\n");
	return 0;	    
}


/***************************************************
**函数功能：输入公历日期得到星期(只允许1901-2099年)，
**			获得现在是星期几
**输入参数：公历年月日 
**返回	值：星期号	
****************************************************/
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}


/*****************************************************
**函数功能：获取当前时间
**返回	值：0,成功;其他:错误代码.
******************************************************/
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	   
     
    timecount = RTC_GetCounter();			 

	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		timer.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(timer.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		timer.w_month=temp1+1;//得到月份
		timer.w_date=temp+1;  //得到日期 
	}
	temp = timecount%86400;     //得到秒钟数   	   
	timer.hour = temp/3600;     //小时
	timer.min = (temp%3600)/60; //分钟	
	timer.sec = (temp%3600)%60; //秒钟
	timer.week = RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//获取星期   
	return 0;
}


/*
 * 函数名：RTC_CheckAndConfig
 * 描述  ：检查并配置RTC
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_CheckAndConfig(tm *tm)
{
	/*在启动时检查备份寄存器BKP_DR1，如果内容不是0xA5A5,
	则需重新配置时间并询问用户调整时间*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
#ifdef USART1_DEBUG	
		printf("\r\n\r\n RTC not yet configured....");
#endif
		/* RTC Configuration */
		RTC_Configuration();

#ifdef USART1_DEBUG	
		printf("\r\n\r\n RTC configured....");
#endif
		
		RTC_Set(tm->w_year,tm->w_month,tm->w_date,tm->hour,tm->min,tm->sec);

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		/*启动无需设置新时钟*/
		/*检查是否掉电重启*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
#ifdef USART1_DEBUG	
			printf("\r\n\r\n Power On Reset occurred....");
#endif
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)	/*检查是否Reset复位*/
		{
#ifdef USART1_DEBUG	
			printf("\r\n\r\n External Reset occurred....");
#endif
		}

#ifdef USART1_DEBUG	
		printf("\r\n No need to configure RTC....");
#endif	
	
		/*等待寄存器同步*/
		RTC_WaitForSynchro();

		/*允许RTC秒中断*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		RTC_WaitForSynchro();
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		/*等待上次RTC寄存器写操作完成*/
		RTC_WaitForLastTask();
	}
	/* Clear reset flags */
	RCC_ClearFlag();
}


/************************************************************
**函数功能	:识别当前是上午、中午还是下午
*************************************************************/
u8 Get_AMPM(void)
{
	if(RTC_Get() == 0)
	{
#ifdef USART1_DEBUG	
		printf("Get RTC finish\r\n");
		printf("%d年 %d月 %d日 %d:%d:%d Week: %d \r\n",timer.w_year,timer.w_month,
					timer.w_date,timer.hour,timer.min,timer.sec,timer.week);
#endif
	}

	if((timer.hour >= 5)&&(timer.hour < 12))			//上午
		return Froenoon;
	else if((timer.hour >= 12)&&(timer.hour < 17))	//中午
		return Noon;
	else if(((timer.hour >= 17)&&((timer.hour <= 23)&&(timer.min <= 59)))||(/*(timer.hour >=0)&&*/(timer.hour < 5)))//下午
		return Afternoon;

	return 0;
}


/*************************************************************
**函数功能：设置下一次闹钟时间
**参	数：s------秒（秒）
**************************************************************/
void Set_RTC_SetAlarm(u32 s)
{
	RTC_WaitForLastTask();
	RTC_SetAlarm(RTC_GetCounter() + s);
    RTC_WaitForLastTask();
}


/*************************************************************
**函数功能：指定时间设置下一次闹钟时间
**参	数：hour------小时	min-----分钟
**************************************************************/
void Set_RTC_Alarm(u8 hour,u8 min)
{
	u32 s;
	s8 hour_num;
	s8 min_num;
	
//	if((hour >=0)&&(hour <24)&&(min >= 0)&&(min < 60))
	if((hour <24)&&(min < 60))
	{
		RTC_Get();	//获取当前时间
		if(hour > timer.hour)	//同一天
		{
			hour_num = hour - timer.hour -1;
			min_num = min + (60 - timer.min);

			if(min_num >= 60)
			{
				min_num = min_num - 60;
				hour_num += 1;
			}
		}
		else if((hour == timer.hour)&&(min > timer.min))//同一天
		{
			hour_num = 0;
			min_num = min - timer.min;

		}
		else if((hour == timer.hour)&&(min <= timer.min))//第二天
		{
			hour_num =hour + (24 - timer.hour);
			
			s = hour_num*60 - timer.min + min;
		}
		else	//第二天
		{
			hour_num =hour + (24 - timer.hour);
			s = hour_num*60 - timer.min + min;
		}
		
		

		s = (hour_num*60 + min_num)*60;
//		printf("hour_num = %d\r\nmin_num = %d\r\nS = %d\r\n",hour_num,min_num,s);
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE); 
//		PWR_BackupAccessCmd(ENABLE);
		RTC_WaitForLastTask();
		RTC_SetAlarm(RTC_GetCounter() + s);
		RTC_WaitForLastTask();
	}
}

tm systmtime = 
{
	15,
	33,
	00,
	2016,
	7,
	12,
	2
};

void RTC_init(void)
{
	RTC_NVIC_Config();
	RTC_Alarm_EXTI_Configuration();
	RTC_Configuration();
	
	RTC_CheckAndConfig(&systmtime);
	Set_RTC_SetAlarm(60);
//	RTC_SetAlarm(RTC_GetCounter() + s);
//	Set_RTC_Alarm(3,48);
}



/***************************************************************
**函数功能:输入年月日,判断是否合法
u8 const month_tab1[] = {31,28,31,30,31,30,31,31,30,31,30,31};		   ///<非闰年每月的天数
u8 const month_tab2[] = {31,29,31,30,31,30,31,31,30,31,30,31};		   ///<闰年每月的天数

****************************************************************/
uint8_t Judge_CalendarDate(tm *CalendarDate_val)
{
	if(CalendarDate_val->w_year < 1970)
		CalendarDate_val->w_year = 1970;
	if(CalendarDate_val->w_year > 2099)
		CalendarDate_val->w_year = 2099;
	
	switch(CalendarDate_val->w_month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 31))
				return 0;
			else 
			{
				CalendarDate_val->w_date = 1;
//				return 1;
			}
			break;
		case 2:
			if(Is_Leap_Year(CalendarDate_val->w_year) == 0)	//平年 
			{
				if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 28))
					return 0;
				else 
				{
					CalendarDate_val->w_date = 1;
					return 1;
				}
			}
			else if(Is_Leap_Year(CalendarDate_val->w_year) == 1)	//闰年
			{
				if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 29))
					return 0;
				else 
				{
					CalendarDate_val->w_date = 1;
					return 1;
				}
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if((CalendarDate_val->w_date > 0)&&(CalendarDate_val->w_date <= 30))
				return 0;
			else 
			{
				CalendarDate_val->w_date = 1;
//				return 1;
			}
			break;
	}
	
	return 1;
}

/***************************************************************
**函数功能:输入两个时间,比较大小
** 参	数:
**返回	值:如果timer_1大于timer_2,返回1;如果timer_1小于timer_2,返回-1;如果相等,返回0
****************************************************************/
int8_t Judge_Timestamp(tm timer_1,tm timer_2)
{
	uint32_t sec_1 = 0;
	uint32_t sec_2 = 0;
	
	sec_1 = timer_1.hour*60*60 + timer_1.min*60 + timer_1.sec;
	sec_2 = timer_2.hour*60*60 + timer_2.hour*60 + timer_2.sec;
	
	if(sec_1 > sec_2)
		return 1;
	else if(sec_1 < sec_2)
		return -1;
	else if(sec_1 == sec_2)
		return 0;
	
	return 0;
}





