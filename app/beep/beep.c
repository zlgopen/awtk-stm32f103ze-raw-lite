#include "beep.h"
void beep_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = beep;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(beep_io, &GPIO_InitStructure);
	beepL;			     	
}
void beep_sound()
{
	beepH;
	delay_us(500);
	beepL;
	delay_us(500);
}
