#include "tim.h"
u32 TIM6_CNT;
u32 TIM7_CNT;
u32 Breather_flag;
u32 temp;

/*基本定时器 TIM6 TIM7*/
void Base_TIM_Config(void)
{
	//打开TIM6 TIM7时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM7,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);
	// 清除更新中断标志位
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	TIM_ClearFlag(TIM7,TIM_FLAG_Update);
	//开启定时器中断
	TIM_ITConfig(TIM6,TIM_FLAG_Update,ENABLE);
	TIM_ITConfig(TIM7,TIM_FLAG_Update,ENABLE);
	
	//使能定时器
	TIM_Cmd(TIM6,ENABLE);
	TIM_Cmd(TIM7,ENABLE);
}

void NVIC_TIM_Config(void)
{
	/*配置NVIC*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
}

void Base_TIM_Init(void)
{
	NVIC_TIM_Config();
	Base_TIM_Config();
}

/*通用定时器 TIM4*/
void Universal_TIM_Config(void)
{
	//打开TIM4 TIM5时钟 PB8
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	/*配置GPIOB*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	/*配置TIM4时基单元*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	/*配置TIM4为输出*/
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//先赋默认值，在进行覆盖
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	
	/*配置TIM4的通道*/
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
	/*使能通道1预加载*/
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	/* 配置TIM4更新中断(TIM_IT_Update) */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
	/*通道 3 捕获比较中断（TIM_IT_CC3）使能*/
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE); 
	/* 配置NVIC（中断优先级） */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	//使能定时器
	TIM_Cmd(TIM4,ENABLE);
}

void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
		TIM6_CNT++;
	}
}
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM7,TIM_FLAG_Update);
		TIM7_CNT++;
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(Breather_flag==0)
            temp++;
        if(Breather_flag==1)
            temp--;
		
        if(temp >=1000)
            Breather_flag=1;
        if(temp <=0)
            Breather_flag=0;
		TIM_SetCompare3(TIM4,temp);
		LED2(1);LED3(1);LED4(1);
	}
	if(TIM_GetITStatus(TIM4,TIM_IT_CC3)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		LED2(0);LED3(0);LED4(0);
	}
}
