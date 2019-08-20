#include "iwdg.h"
void iwdg_init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����
	IWDG_SetPrescaler(prer);   //���ÿ��Ź���Ƶϵ�� 0-7
   	IWDG_SetReload(rlr);		//���ÿ��Ź���װ�س�ֵ
	IWDG_ReloadCounter();	//���� IWDG ��װ�ؼĴ�����ֵ��װ�� IWDG ������
	IWDG_Enable();			//ʹ�� IWDG
}
void iwdg_feed()  
{
	IWDG_ReloadCounter();	
}
