#ifndef _RTL8710_DRIVER_H_
#define _RTL8710_DRIVER_H_

#include "board_config.h"

#include "pipe.h"

typedef enum{
	WIFI_MODE_AP,
	WIFI_MODE_STA,
	WIFI_MODE_APSTA
}WIFIWork_Mode;


typedef enum{
	WIFI_START_SUCCEED = 0,//wifi 启动成功标志
	WIFI_SETUP_MODE,//配置模式
	WIFI_TRANSFER_MODE,//透传模式
}WIFI_WORK_STATUS;

typedef enum{
	OPEN_OS_WAKELOCK = 0,//打开操作系统锁(系统禁止睡眠)
	UNLOCK_OS_WAKELOCK,//解除操作系统睡眠锁(系统允许睡眠)
	GET_OS_SLEEP_STATUS,//获取操作系统睡眠锁状态
}__ATSP_PWR_SLEEP;

typedef enum{
	AT_PRINT_ENABLE = 0,//使能串口打印AT指令
	AT_PRINT_DISABLE,//禁用串口打印AT指令
}__ATSE_AT_PRINT;


#define WIFI_SUCCESS		0
#define WIFI_ERROR			1


#define WIFI_MEM_BUFF			Usart2_buff
#define WIFI_MEM_BUFF_SIZE		USART2_BUFF_SIZE

typedef struct __tag_WIFI_CONNECT_INFO{
	uint8_t ap_ssdi[40];	//AP的SSID
	uint8_t ap_password[40];	//AP的密码
	uint8_t ipaddr[16];//本机的iP地址
	uint8_t gatewayip[16];//网关
	uint8_t mac_addr[18];//本机的MAC地址
}WIFI_CONNECT_INFO;

extern uint8_t wifi_pipe_enable;
extern Pipe_t WIFI_RTL8710_pipe;


void WIFI_RTL8710_Port_GPIO_Init(void);
void WIFI_RTL8710_Enable(void);
void WIFI_RTL8710_Disable(void);
void WIFI_RTL8710_ReSet(void);

void WIFI_RTL8710_Startup(void);
void WIFI_RTL8710_Init(void);

#endif
