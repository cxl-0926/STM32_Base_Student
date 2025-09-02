#include "main.h" 


int main()
{
	SysTick_Config(72000);//1ms计数一次
	LED_Config();
	Universal_TIM_Config();
	USART1_Config();
	printf("ah\r\n");
	while(1)
	{
		if(USART1_Flag)
		{
//			USART1_Flag=0;
	
//			memset(USART1_buf,0,255);
//			USART1_len=0;
		}
	}
}
