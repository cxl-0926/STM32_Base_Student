#include "usart1.h"


u32 USART1_Flag;
u8 USART1_buf[255];
u8 USART1_len;

void USART1_Config(void)
{
	/*打开USART1和GPIOA时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);
	/*初始化GPIOA*/
	
	GPIO_InitTypeDef GPIOA_InitStruct;
	GPIOA_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOA_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIOA_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStruct);
	
	GPIOA_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOA_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOA_InitStruct);
	
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate=115200;
	USART1_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART1_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART1_InitStruct.USART_Parity=USART_Parity_No;
	USART1_InitStruct.USART_StopBits=USART_StopBits_1;
	USART1_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART1_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); 
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
}

void USART1_SendByte(u8 dat)
{
	USART_SendData(USART1,dat);
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void USART1_SendBuf(u8 *buf, u8 len)
{
    for (u8 i = 0; i < len; i++)
    {
        USART1_SendByte(buf[i]);  // 逐个字节发送
    }
}
/*重定向函数..将printf()函数指向USART1*/
int fputc(int c,FILE * stream)
{
	USART1_SendByte(c);
	return c;
}

void USART1_IRQHandler(void)
{
	u8 temp=0;
	if(!(USART_GetITStatus(USART1,USART_IT_RXNE)==RESET))
	{
		temp=USART_ReceiveData(USART1);
		USART1_buf[USART1_len++]=temp;
	}
	if(!(USART_GetITStatus(USART1,USART_IT_IDLE)==RESET))
	{
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
		USART1_Flag=1;
	}
}

