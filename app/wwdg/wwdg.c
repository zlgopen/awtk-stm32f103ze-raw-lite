#include "wwdg.h"
#include "led.h"
u8 WWDG_CNT=0x7f;
//tr��T[0-6]���Ź�����ֵ��windowvalue�Ǵ���ֵ��prer�Ƿ�Ƶϵ����ϵ��ΪWWDG_Prescaler_1 ��2��4��8
void wwdg_init(u8 tr,u8 windowvalue,u32 prer)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //�򿪴��ڿ��Ź�ʱ��
	WWDG_CNT=tr&WWDG_CNT;	 //tr��T[0-6]ֵ	    ��ʼ��WWDG_CNT
	WWDG_SetWindowValue(windowvalue); //���ô��ڿ��Ź��ϴ���ֵ	 ���0x7f����С����Ϊ0x40
	WWDG_SetPrescaler(prer);					//���� WWDG Ԥ��Ƶֵ
	WWDG_Enable(WWDG_CNT);		   //ʹ�� WWDG ��װ�������ֵ

	WWDG_ClearFlag();		  //������ڻ����жϱ�־λ
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;  //���ڿ��Ź��ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	WWDG_EnableIT();	 //ʹ�� WWDG ���ڻ����жϣ� EWI��
}
void WWDG_IRQHandler(void)
{
	//WWDG_Enable(WWDG_CNT);	  //����װ�ؼ�����ֵ
	WWDG_SetCounter(WWDG_CNT);
	WWDG_ClearFlag();		  //������ڻ����жϱ�־λ
	led1=~led1;
}
