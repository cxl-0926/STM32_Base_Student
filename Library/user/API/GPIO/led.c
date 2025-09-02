#include "led.h"

u32 SysTick_cnt;
u8 LED_Flag;

void LED_Config(void)
{
	/*��GPIOBʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);
	/*��ʼ��LED����*/
	GPIO_InitTypeDef LED_InitStruct;
	LED_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	LED_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	LED_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&LED_InitStruct);
	
	LED_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOE,&LED_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
}

void LED_Water_Blocking(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//���㣨���͵�ƽ��
	Delay_ms(500);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//��һ(���ߵ�ƽ)
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//���㣨���͵�ƽ��
	Delay_ms(500);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);//��һ(���ߵ�ƽ)
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);//���㣨���͵�ƽ)
	Delay_ms(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);//��һ(���ߵ�ƽ)
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_1);//���㣨���͵�ƽ��
	Delay_ms(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);//��һ(���ߵ�ƽ)
}

void LED_Water_Non_Blocking(void)
{
	
	if(SysTick_cnt>=500)
	{
		LED_Flag++;
		SysTick_cnt=0;
		switch(LED_Flag)
		{
			case 1:LED1(1);LED2(0);LED3(0);LED4(0);break;
			case 2:LED1(0);LED2(1);LED3(0);LED4(0);break;
			case 3:LED1(0);LED2(0);LED3(1);LED4(0);break;
			case 4:LED1(0);LED2(0);LED3(0);LED4(1);break;
		}
		if(LED_Flag>=4)
			LED_Flag=0;
	}
}
void SysTick_Handler(void)
{
	SysTick_cnt++;
}

