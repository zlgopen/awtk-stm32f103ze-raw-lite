#include "lcd_driver.h"
u16 tft_id;
/****************************************************************************
*��������TFT_GPIO_Config
*��  �룺��
*��  ������
*��  �ܣ���ʼ��TFT��IO�ڡ�
****************************************************************************/	  

void TFT_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��ʱ��ʹ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE  
	                      | RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	/* FSMC_A10(G12) ��RS��G0��*/
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 
	                              | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_8 
								  | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
	                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								   | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/****************************************************************************
* Function Name  : TFT_FSMC_Config
* Description    : ��ʼ��FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_FSMC_Config(void)
{
	/* ��ʼ������ */
	FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTiming;

	/* ���ö�дʱ�򣬸�FSMC_NORSRAMInitStructure���� */
	/* ��ַ����ʱ�䣬3��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_AddressSetupTime = 0x02;

	/* ��ַ����ʱ�䣬1��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_AddressHoldTime = 0x00;

	/* ���ݽ���ʱ�䣬6��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_DataSetupTime = 0x05;

	/* ���ݱ���ʱ�䣬1��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_DataLatency = 0x00;

	/* ���߻ָ�ʱ������ */
	FSMC_NORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	
	/* ʱ�ӷ�Ƶ���� */
	FSMC_NORSRAMTiming.FSMC_CLKDivision = 0x01;

	/* ����ģʽ������ڵ�ַ/���ݲ�����ʱ��ABCDģʽ�����𲻴� */
	FSMC_NORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_B;

	/*����FSMC_NORSRAMInitStructure������*/
	/* FSMC���ĸ��洢�飨bank��������ʹ�õ�һ����bank1�� */
	/* ͬʱ����ʹ�õ���bank����ĵ� 4��RAM�� */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;

	/* ��������ʹ��SRAMģʽ */
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;

	/* ʹ�õ����ݿ��Ϊ16λ */
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;

	/* ����дʹ�ܴ� */
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;

	/* ѡ����չģʽʹ�ܣ������ö���д�ò�ͬ��ʱ�� */
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	
	/* ���õ�ַ�����ݸ���ʹ�ܲ��� */
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	
	/* ���ö�дʱ�� */
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTiming;
	
	/* ����дʱ�� */
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTiming;

	/* ��FSMC��ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	 
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	/*!< Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	
}

/****************************************************************************
* Function Name  : TFT_WriteCmd
* Description    : LCDд������
* Input          : cmd��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteCmd(uint16_t cmd)
{
	TFT->TFT_CMD = cmd;
}

/****************************************************************************
* Function Name  : TFT_WriteData
* Description    : LCDд������
* Input          : dat��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteData(u16 dat)
{
	TFT->TFT_DATA = dat;
}

/****************************************************************************
* Function Name  : TFT_Init
* Description    : ��ʼ��LCD��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_Init(void)
{
	uint16_t i;

	TFT_GPIO_Config();
	TFT_FSMC_Config();

	TFT_WriteCmd(0xE9); 
	TFT_WriteData(0x20); 
	
	TFT_WriteCmd(0x11); //Exit Sleep 
	for(i=500; i>0; i--);
	
	TFT_WriteCmd(0x3A);
	TFT_WriteData(0x55);  //16Bit colors
	
	TFT_WriteCmd(0xD1); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x65); //���Դ�ֵ����ˮ��
	TFT_WriteData(0x1F); 
	
	TFT_WriteCmd(0xD0); 
	TFT_WriteData(0x07); 
	TFT_WriteData(0x07); 
	TFT_WriteData(0x80); 
	
	TFT_WriteCmd(0x36); 	 //Set_address_mode
	TFT_WriteData(0x48);   	//48
	
	TFT_WriteCmd(0xC1); 
	TFT_WriteData(0x10); 
	TFT_WriteData(0x10); 
	TFT_WriteData(0x02); 
	TFT_WriteData(0x02); 
	
	TFT_WriteCmd(0xC0); //Set Default Gamma 
	TFT_WriteData(0x00);  
	TFT_WriteData(0x35); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x01); 
	TFT_WriteData(0x02); 

	TFT_WriteCmd(0xC4);			
	TFT_WriteData(0x03);

	TFT_WriteCmd(0xC5); //Set frame rate 
	TFT_WriteData(0x01); 
	
	TFT_WriteCmd(0xD2); //power setting 
	TFT_WriteData(0x01); 
	TFT_WriteData(0x22); 

	TFT_WriteCmd(0xE7);			
	TFT_WriteData(0x38);
	
	TFT_WriteCmd(0xF3);			
    TFT_WriteData(0x08); 
	TFT_WriteData(0x12);
	TFT_WriteData(0x12);
	TFT_WriteData(0x08);

	TFT_WriteCmd(0xC8); //Set Gamma 
	TFT_WriteData(0x01); 
	TFT_WriteData(0x52); 
	TFT_WriteData(0x37); 
	TFT_WriteData(0x10); 
	TFT_WriteData(0x0d); 
	TFT_WriteData(0x01); 
	TFT_WriteData(0x04); 
	TFT_WriteData(0x51); 
	TFT_WriteData(0x77); 
	TFT_WriteData(0x01); 
	TFT_WriteData(0x01); 
	TFT_WriteData(0x0d); 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x80); 
	TFT_WriteData(0x00);  

	TFT_WriteCmd(0x29); //display on



}

/****************************************************************************
* Function Name  : TFT_SetWindow
* Description    : ����Ҫ�����Ĵ��ڷ�Χ
* Input          : xStart��������ʼX����
*                * yStart��������ʼY����
*                * xEnd�����ڽ���X����
*                * yEnd�����ڽ���Y����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
 	TFT_WriteCmd(0x2A);

    TFT_WriteData(xStart>>8);
    TFT_WriteData(xStart&0XFF);
    TFT_WriteData(xEnd>>8);
    TFT_WriteData(xEnd&0XFF);

    TFT_WriteCmd(0x2b);
    TFT_WriteData(yStart>>8);
    TFT_WriteData(yStart&0XFF);
    TFT_WriteData(yEnd>>8);
    TFT_WriteData(yEnd&0XFF);
    TFT_WriteCmd(0x2c);
}

/****************************************************************************
* Function Name  : TFT_ClearScreen
* Description    : ��LCD��������Ӧ����ɫ
* Input          : color��������ɫ
* Output         : None
* Return         : None
****************************************************************************/
	  
void TFT_ClearScreen(uint16_t color)
{
 	uint16_t i, j ;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //��������
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
			TFT_WriteData(color);
		}
	}
}


