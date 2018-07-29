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
/***********************************************************
**文件功能：USART串口操作文件
**接口定义：复用端口
**			USART1_Tx-----------GPIOA9
**			USART1_Rx-----------GPIOA10
************************************************************/
#include "usart_driver.h"
#include "ST_string.h"

#ifdef DEVICE_USART

#ifdef USART2_ENABLE
s8 Usart2_buff[USART2_BUFF] = {0};
USARTRecvData Usart2_RecvStat = 
{
	0,
	&Usart2_buff[0],
	&Usart2_buff[0],
};
#endif

#ifdef USART3_ENABLE
s8 Usart3_buff[USART3_BUFF] = {0};
USARTRecvData Usart3_RecvStat = 
{
	0,
	&Usart3_buff[0],
	&Usart3_buff[0],
};
#endif


/***************************************
**USART1
****************************************/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_DeInit(USART1);
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART1->SR &= ~(0x01<<6);
	
	USART_Cmd(USART1, ENABLE);
}


int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



char usart1_putc(char ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);		

	return (ch);
}

int usart1_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart1_putc(buff[i]);
	}

	return len;
}

int usart1_sendstr(unsigned char* buff)
{
	int i = 0;
	
	while(*buff != '\0')
	{
		usart1_putc(*buff++);
		i++;
	}

	return i;
}

char usart1_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!= SET); 
	
	temp = USART_ReceiveData(USART1);

	return temp;
}

int usart1_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart1_getc();
	}

	return len;
}

/************************************************************************/
/***************************************
**USART2
****************************************/
void USART2_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	
		/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART2 Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
//	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART2->SR &= ~(0x01<<6);
	
//	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
//	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

	USART_Cmd(USART2, ENABLE);
}



char usart2_putc(char ch)
{
	USART_SendData(USART2, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);		

	return (ch);
}

char usart2_getc(void)
{
	char temp = 0;
	
	while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET);
	
	temp = USART_ReceiveData(USART2);

	return temp;
}



int usart2_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart2_putc(buff[i]);
	}

	return len;
}

int Usart2_Sendstr(unsigned char* buff)
{
	int ret = 0;
	
	while(*buff)
	{
		usart2_putc(*buff);
		buff++;
		ret++;
	}
	
	return ret;
}

int usart2_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart2_getc();
	}

	return len;
}

/************************************************************************/
/***************************************
**USART3
****************************************/
void USART3_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB10) and RTS(PB14) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	/* Configure USART3 Rx (PB11) and CTS(PB13) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

//	USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
//	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART3->SR &= ~(0x01<<6);

	USART_Cmd(USART3, ENABLE);
}



char usart3_putc(char ch)
{
	USART_SendData(USART3, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);		

	return (ch);
}

char usart3_getc(void)
{
	char temp = 0;
	
	while(	USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!= SET);
	
	temp = USART_ReceiveData(USART3);

	return temp;
}



int usart3_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart3_putc(buff[i]);
	}

	return len;
}

int Usart3_Sendstr(unsigned char* buff)
{
	int ret = 0;
	
	while(*buff)
	{
		usart3_putc(*buff);
		buff++;
		ret++;
	}
	
	return ret;
}

int Usart3_SendData(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart3_putc(buff[i]);
	}

	return len;
}

int usart3_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart3_getc();
	}

	return len;
}

/****************************************************************************/

void Clear_USART_Buff(USART_Buf_Select usart)
{
	if(usart == USART1_BUF)
	{
#ifdef USART1_ENABLE
#endif
	}
	else if(usart == USART2_BUF)
	{
#ifdef USART2_ENABLE
		st_memset((unsigned char *)Usart2_buff,(s8)0,USART2_BUFF);
		Usart2_RecvStat.Recvcount = 0;
		Usart2_RecvStat.Recv_start = &Usart2_buff[0];
		Usart2_RecvStat.Recv_end = &Usart2_buff[0];
#endif
	}
	else if(usart == USART3_BUF)
	{
#ifdef USART3_ENABLE
		st_memset((unsigned char *)Usart3_buff,(s8)0,USART3_BUFF);
		Usart3_RecvStat.Recvcount = 0;
		Usart3_RecvStat.Recv_start = &Usart3_buff[0];
		Usart3_RecvStat.Recv_end = &Usart3_buff[0];
#endif
	}
}

void USART_DMA_Configuration(void)
{
#ifdef USART2_ENABLE
	DMA_InitTypeDef DMA_InitStructure;
	
	
	//USART2 DMA Rx
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart2_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART2_BUFF;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	
	//USART2 DMA Tx
	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart2_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = USART2_BUFF;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
#endif
}

void UASRT_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#ifdef USART1_ENABLE
#if 0
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
#endif
	
#ifdef USART2_ENABLE
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	
#ifdef USART3_ENABLE	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

void USART_InitConfig(void)
{
	UASRT_NVIC_Configuration();
#ifdef USART1_ENABLE
	USART1_Config();
#endif
	
#ifdef USART2_ENABLE
	USART2_Config();
#endif
	
#ifdef USART3_ENABLE
	USART3_Config();
#endif
}

#endif

