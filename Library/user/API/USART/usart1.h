#ifndef __USART1_H
#define __USART1_H

#include "main.h"

void USART1_Config(void);
void USART1_SendByte(u8 dat);
void USART1_SendBuf(u8 *buf, u8 len);

extern u32 USART1_Flag;
extern u8 USART1_buf[255];
extern u8 USART1_len;

#endif
