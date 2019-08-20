#ifndef _stm32_flash_H
#define _stm32_flash_H
#include "sys.h"
#include "delay.h"
 
#define flash_adr 0x0807f800	 //STM32���洢����Ԫ�����һҳ�׵�ַ  ���ڴ�����оƬһҳ��2K�ֽ�

void STM32_Flash_Init(void);
void STM32_Flash_Write(u32 write_addr,u16 *pdat,u16 len);
void STM32_Flash_Read(u32 read_addr,u16 *pdat,u16 len);
u16 STM32_Flash_HalfRead(u32 read_addr);


#endif
