#include "stm32_flash.h"


//STM32�ڲ�Flash��ʼ��
void STM32_Flash_Init(void)
{
	FLASH_Unlock();	 //STM32 Flash����
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
	
	FLASH_Lock();//����			
}

//STM32Flashд���� һ��д16λ����
//write_addr��д��ĵ�ַ
//pdat��д�������
//len��д��ĳ���
void STM32_Flash_Write(u32 write_addr,u16 *pdat,u16 len)   
{
	u16 i;
	FLASH_Unlock();	 //STM32 Flash����
	FLASH_ErasePage(flash_adr);//Ҫ����ҳ����ʼ��ַ
	for(i=0;i<len;i++)
	{
		FLASH_ProgramHalfWord(write_addr,pdat[i]);   //д����ֺ���
		write_addr+=2;
	}
	FLASH_Lock();//����			
}

//STM32Flash������ 
//read_addr��д��ĵ�ַ
//pdat��д�������
//len��д��ĳ���
void STM32_Flash_Read(u32 read_addr,u16 *pdat,u16 len) 
{ 	
	u16 i;
	FLASH_Unlock();	 //STM32 Flash����
	for(i=0;i<len;i++)
	{
		pdat[i]=STM32_Flash_HalfRead(read_addr);
		read_addr+=2;
	}
	FLASH_Lock();//����		
}

u16 STM32_Flash_HalfRead(u32 read_addr)
{
	return *(u16*)read_addr;	
}
