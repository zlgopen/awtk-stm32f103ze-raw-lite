#include "rs485.h"
void rs485_init(u32 baund)   //485��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //����һ���ṹ�������������ʼ��GPIO

	USART_InitTypeDef USART_InitStructure;	 //���ڽṹ�嶨��

	NVIC_InitTypeDef NVIC_InitStructure;	//�жϽṹ�嶨��

	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;	//TX-485	//�������PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);				 	/* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);		   
	GPIO_ResetBits(GPIOG,GPIO_Pin_3);//����Ҫ��485�������ݵĿ��ƶ�

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//RX-485	   //��������PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);			 /* ��ʼ��GPIO */


	USART_InitStructure.USART_BaudRate = baund;			   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	   //���ݳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		   //��Ч��
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		 //�������ͺͽ���ģʽ
	USART_Init(USART2, &USART_InitStructure);	   /* ��ʼ��USART2 */
	USART_Cmd(USART2,ENABLE);	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(USART2,USART_FLAG_TC);	 //���USARTx�Ĵ�������־λ

	/* ����NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 	   //��USART2��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 		 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		 //ʹ��
	NVIC_Init(&NVIC_InitStructure);		
}
void USART2_IRQHandler(void)	//485ͨ���жϺ���
{
	static u8 k;
	USART_ClearFlag(USART2,USART_FLAG_TC);
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//���ָ����USART�жϷ������	
	{
		k=USART_ReceiveData(USART2);
		GPIO_SetBits(GPIOG,GPIO_Pin_3);
		delay_ms(1);
		USART_SendData(USART2,k);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
		delay_ms(2);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_3);	
	}
}