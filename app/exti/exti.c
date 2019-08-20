#include "exti.h"
#include "button.h"
#include "led.h"
void exti_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //�˿ڸ���ʱ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;		 //�ж���·0
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  //ʹ��
	EXTI_Init(&EXTI_InitStructure);	 //��ʼ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;		 //�ж���·2
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  //ʹ��
	EXTI_Init(&EXTI_InitStructure);	 //��ʼ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;		 //�ж���·3
	EXTI_Init(&EXTI_InitStructure);	 //��ʼ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;		 //�ж���·4
	EXTI_Init(&EXTI_InitStructure);	 //��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;  //�ⲿ�ж���0ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;  //�ⲿ�ж���2ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;  //�ⲿ�ж���3ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;  //�ⲿ�ж���4ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 /*   //���ʹ�ô������ѣ������Ҫע�ͣ���Ϊ��������Ҳʹ�����ж�0����
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)	
	{	
		if(K_UP==1)
		{
			delay_ms(10);
			if(K_UP==1)
			{
				led0=~led0;	
			}
			while(K_UP);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);		
}*/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)	
	{	
		if(K_LEFT==0)
		{
			delay_ms(10);
			if(K_LEFT==0)
			{
				led1=~led1;	
			}
			while(!K_LEFT);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);			
}
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)	
	{	
		if(K_DOWN==0)
		{
			delay_ms(10);
			if(K_DOWN==0)
			{
				led2=~led2;	
			}
			while(!K_DOWN);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);			
}
void EXTI4_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)	
	{	
		if(K_RIGHT==0)
		{
			delay_ms(10);
			if(K_RIGHT==0)
			{
				led3=~led3;	
			}
			while(!K_RIGHT);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);				
}
