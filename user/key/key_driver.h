#ifndef _KEY_DRIVER_H_
#define _KEY_DRIVER_H_

#include "board_config.h"
#include "Run_task.h"

#ifdef DEVICE_KEY


#define KEY_DOWN		0
#define KEY_UP			1



#define ADKEY_VAL1		0
#define ADKEY_VAL2		491
#define ADKEY_VAL3		966
#define ADKEY_VAL4		1280
#define	ADKEY_VAL5		1660
#define ADKEY_MAX		1800
#define ADKEY_MIN		0


#define ADKEY_V_0		(ADKEY_VAL2-ADKEY_VAL1)/2 + ADKEY_VAL1
#define ADKEY_V_1		(ADKEY_VAL3-ADKEY_VAL2)/2 + ADKEY_VAL2
#define ADKEY_V_2		(ADKEY_VAL4-ADKEY_VAL3)/2 + ADKEY_VAL3
#define ADKEY_V_3		(ADKEY_VAL5-ADKEY_VAL4)/2 + ADKEY_VAL4
#define ADKEY_V_4		(ADKEY_MAX-ADKEY_VAL5)/2 + ADKEY_VAL5



#define	NO_KEY			0xff


//#define KEY_BASE_CNT  5
//#define KEY_LONG_CNT  (75)
//#define KEY_HOLD_CNT  15

#define KEY_BASE_CNT  35
#define KEY_LONG_CNT  (200)
#define KEY_HOLD_CNT  50



////////////////IO Key//////////////////////
#define IOKEY_SHORT		\
						MSG_IOKEY_SHORT,\
						NO_MSG,\
						NO_MSG,
							
							

//长按
#define IOKEY_LONG		\
						MSG_IOKEY_LONG,\
						NO_MSG,\
						NO_MSG,



//连按
#define IOKEY_HOLD		\
						MSG_IOKEY_HOLD,\
						NO_MSG,\
						NO_MSG,

//长按抬起
#define IOKEY_LONG_UP	\
						MSG_IOKEY_LONG_UP,\
						NO_MSG,\
						NO_MSG,


////////////////AD Key//////////////////////
#define ADKEY_SHORT		\
						MSG_ADKEY_1_SHORT,\
						MSG_ADKEY_2_SHORT,\
						MSG_ADKEY_3_SHORT,\
						MSG_ADKEY_4_SHORT,\
						MSG_ADKEY_5_SHORT
							
							

//长按
#define ADKEY_LONG		\
						MSG_ADKEY_1_LONG,\
						MSG_ADKEY_2_LONG,\
						MSG_ADKEY_3_LONG,\
						MSG_ADKEY_4_LONG,\
						MSG_ADKEY_5_LONG



//连按
#define ADKEY_HOLD		\
						MSG_ADKEY_1_HOLD,\
						MSG_ADKEY_2_HOLD,\
						MSG_ADKEY_3_HOLD,\
						MSG_ADKEY_4_HOLD,\
						MSG_ADKEY_5_HOLD

//长按抬起
#define ADKEY_LONG_UP	\
						MSG_ADKEY_1_LONG_UP,\
						MSG_ADKEY_2_LONG_UP,\
						MSG_ADKEY_3_LONG_UP,\
						MSG_ADKEY_4_LONG_UP,\
						MSG_ADKEY_5_LONG_UP



typedef enum
{
	MODE_ADKEY = 1,
	MODE_IOKEY,
}_KEY_MODE;


void Key_GPIOConfig(void);
void ADC_Scan(void);
void Key_Scan(void);

#endif

#endif

