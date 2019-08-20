#include "spi.h"
void SPI2_Init()  //SPI2��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef	 SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//��SPI2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		//PB13 PB14 PB15	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			     
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //����
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //	SPI ����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;	//����SPIΪ��ģʽ
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;	//����SPI���ݴ�СΪ8λ
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;	//����SPIĬ��ʱ������Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;	//����SPI���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;	//����SPI NSS�ܽ����������
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;	//����SPI ������Ԥ��ƵֵΪ 256
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//����SPI ���ݴӸ�λ����
	SPI_InitStructure.SPI_CRCPolynomial=7;	//����SPI CRCֵ����Ķ���ʽ
	SPI_Init(SPI2,&SPI_InitStructure);	
	
	SPI_Cmd(SPI2,ENABLE); //ʹ��SPI2			
}

u8 SPI2_ReadWriteByte(u8 dat)		 //SPI2��дһ���ֽ�
{
	u8 t;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)	//�ȴ����ͻ�������
	{
		t++;
		if(t>=200)return 0;	//��ʱ���ش����־	
	}
	SPI_I2S_SendData(SPI2,dat); //��������
	t=0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)	//�ȴ�����
	{
		t++;
		if(t>=200)return 0;	//��ʱ���ش����־	
	}
	return SPI_I2S_ReceiveData(SPI2); //�������SPI2���յ�����			
}
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)	//����SPI2���ٶ�
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	
	SPI2->CR1&=0XFFC7;		 //�޸�BR[2:0]ֵ
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE);			
}


