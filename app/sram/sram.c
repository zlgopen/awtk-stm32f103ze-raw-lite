#include "sram.h"


//��ʼ���ⲿSRAM
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTiming;
	GPIO_InitTypeDef GPIO_InitStructure;

	//PD,PE-FSMC_DATA 	  PF,PG-FSMC_A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE  
	                      | RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;	  //FSMC_NE3 PG10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;	  //FSMC_NBL0-1 PE0	PE1
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5;	  //FSMC_NOE,FSMC_NWE PD4 PD5
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8  
								  | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
	                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								   | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  
								  | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  
								  | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5);
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	
	FSMC_NORSRAMTiming.FSMC_AddressSetupTime = 0x00; //��ַ����ʱ��Ϊ1��HCLK
	FSMC_NORSRAMTiming.FSMC_AddressHoldTime = 0x00;	//��ַ����ʱ��ģʽAδʹ��
	FSMC_NORSRAMTiming.FSMC_DataSetupTime = 0x03;  //���ݱ���ʱ��Ϊ3��HCLK
	FSMC_NORSRAMTiming.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTiming.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ʹ��ģʽA

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;	 //ʹ��Bank1-3��
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//�洢��дʹ��
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; //��дʹ����ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTiming;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);		
}

//FSMC_SRAMд����
//pbuf��д�������
//writeaddr��д������ݵ���ʼ��ַ
//len��д�����ݵĳ���
void FSMC_SRAM_WriteBuf(u8 *pbuf,u32 writeaddr,u32 len)
{
	while(len--)
	{
		*(u8*)(SRAM_ADDR+writeaddr)=*pbuf;
		writeaddr++;
		pbuf++;
	}		
}

//FSMC_SRAM������
//pbuf�������������
//readaddr����ȡ���ݵ���ʼ��ַ
//len�������ݵĳ���
void FSMC_SRAM_ReadBuf(u8 *pbuf,u32 readaddr,u32 len)
{
	while(len--)
	{
		*pbuf=*(u8*)(SRAM_ADDR+readaddr);
		readaddr++;
		pbuf++;
	}		
}

//SRAMд���ԣ�дһ���ֽ�
//dat��д�������
//wirte_addr��д��ĵ�ַ
void FSMC_SRAM_Test_Write(u8 dat,u32 wirte_addr)
{
	FSMC_SRAM_WriteBuf(&dat,wirte_addr,1);			
}

//SRAM�����ԣ���һ���ֽ�
//read_addr����ȡ�ĵ�ַ
u8 FSMC_SRAM_Test_Read(u32 read_addr)
{
	u8 dat;
	FSMC_SRAM_ReadBuf(&dat,read_addr,1);
	return dat;			
}

