#include "tim.h"
u32 TIM6_CNT;
u32 TIM7_CNT;
u32 Breather_flag;
u32 temp;

/*������ʱ�� TIM6 TIM7*/
void Base_TIM_Config(void)
{
	//��TIM6 TIM7ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM7,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);
	// ��������жϱ�־λ
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	TIM_ClearFlag(TIM7,TIM_FLAG_Update);
	//������ʱ���ж�
	TIM_ITConfig(TIM6,TIM_FLAG_Update,ENABLE);
	TIM_ITConfig(TIM7,TIM_FLAG_Update,ENABLE);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM6,ENABLE);
	TIM_Cmd(TIM7,ENABLE);
}

void NVIC_TIM_Config(void)
{
	/*����NVIC*/
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

/*ͨ�ö�ʱ�� TIM4*/
void Universal_TIM_Config(void)
{
	//��TIM4 TIM5ʱ�� PB8
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	/*����GPIOB*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	/*����TIM4ʱ����Ԫ*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	/*����TIM4Ϊ���*/
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);	//�ȸ�Ĭ��ֵ���ڽ��и���
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	
	/*����TIM4��ͨ��*/
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
	/*ʹ��ͨ��1Ԥ����*/
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	/* ����TIM4�����ж�(TIM_IT_Update) */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
	/*ͨ�� 3 ����Ƚ��жϣ�TIM_IT_CC3��ʹ��*/
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE); 
	/* ����NVIC���ж����ȼ��� */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	//ʹ�ܶ�ʱ��
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
