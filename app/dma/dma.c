#include "dma.h"

//DMA_CHx:DMA ͨ�� CHx	 DMA1_Channel1-DMA1_Channel7
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����
u16 DMA1_MEM_LEN; 
void dma_init(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//����DMA1ʱ��
	DMA_DeInit(DMA_CHx);  //��λDMA���Ĵ���ֵ
	DMA1_MEM_LEN=cndtr;
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);	  //ʹ��ָ����DMA����

	DMA_InitStructure.DMA_PeripheralBaseAddr =cpar; //�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar; //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	//DMA���ݴ��䷽��  ���ڴ浽���裨Ŀ�ĵأ�
	DMA_InitStructure.DMA_BufferSize = cndtr;//	DMA�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;//�������ݿ��8λ
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;	//�ڴ����ݿ��8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMAͨ�������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //ʧ���ڴ浽�ڴ�
	DMA_Init(DMA_CHx, &DMA_InitStructure);	
	 
}
void DMA_Enable(DMA_Channel_TypeDef* DMA_CHx)  //����DMAһ�δ���
{
	DMA_Cmd(DMA_CHx,DISABLE);  //ʹ��ͨ��DMA
	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);	//����DMA�����С
	DMA_Cmd(DMA_CHx,ENABLE);  //ʹ��ͨ��DMA		
}


