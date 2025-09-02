#ifndef __LED_H
#define __LED_H

#include "main.h" 

#define LED1(X) (X)?(GPIO_ResetBits(GPIOB,GPIO_Pin_8)):(GPIO_SetBits(GPIOB,GPIO_Pin_8))
#define LED2(X) (X)?(GPIO_ResetBits(GPIOB,GPIO_Pin_9)):(GPIO_SetBits(GPIOB,GPIO_Pin_9))
#define LED3(X) (X)?(GPIO_ResetBits(GPIOE,GPIO_Pin_0)):(GPIO_SetBits(GPIOE,GPIO_Pin_0))
#define LED4(X) (X)?(GPIO_ResetBits(GPIOE,GPIO_Pin_1)):(GPIO_SetBits(GPIOE,GPIO_Pin_1))

void LED_Config(void);
void LED_Water_Blocking(void);
void LED_Water_Non_Blocking(void);

extern u32 SysTick_cnt;

#endif
