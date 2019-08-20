#include "wkup.h"
#include "led.h"
#include "gui.h"
void wkup_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//�򿪶˿�A�͸���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);  //Ҫ����������䣬��PA0������

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

	EXTI_InitStructure.EXTI_Line=EXTI_Line0;		 //�ж���·0
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  //ʹ��
	EXTI_Init(&EXTI_InitStructure);	 //��ʼ��

	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;  //�ⲿ�ж���0ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if(Check_Wkup()==0)	   //���ǿ����������ģʽ
	{
		GUI_Show12ASCII(10,150,"Enter Standby Mode...",GREEN,BLACK);
		Sys_Standyby();  
	}

}

void Sys_Standyby()	  //����ģʽ
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE); //�򿪵�Դʱ��
	PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ��ѹܽ�WAKE_UP	
	PWR_EnterSTANDBYMode(); //�������ģʽ
}
void Sys_Enter_WkupStandby()
{
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	Sys_Standyby();	
}
u8 Check_Wkup()	//����1������ ����0������
{
	u8 t=0;
	while(1)
	{
		if(WKUP_KD)
		{
			t++;
			delay_ms(20);
			if(t>=100)
			{
				led0=0;	   //��
				return 1;  //����3���ʾ����		
			}
		}
		else
		{
			led0=1;	  //��
			return 0;  //����3���ʾ�������
		}	
	}	
}
//���ʹ�ô������ѣ���exti.c�ļ��ڵ��ж�0��������ע�ͣ��������ᱨ��
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)	
	{	
		delay_ms(10);
		if(Check_Wkup())   //�Ƿ�ػ���
		{
			Sys_Enter_WkupStandby();
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);		
}

