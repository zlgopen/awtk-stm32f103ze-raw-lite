#include "dac.h"

void dac1_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef	 DAC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //�򿪶˿�A PA4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);  //��DACʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PA4	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	 //ģ������ģʽ 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	 //����
	
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;	  //��ʹ�ô�������
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;	 //��ʹ�ò��η�����
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//ʹ�ò��η���������Ч
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;	//�ر��������
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1,ENABLE);	//ʹ��DACͨ��1
	DAC_SetChannel1Data(DAC_Align_12b_R,0); //����DAC�����Ҷ���ͳ�ʼ��DACֵ
}
void Set_Channel_val(u16 dat)  //����DACͨ������ 0-3300��0-3.3V��
{
	float temp;
	u16 val=0;
	temp=dat/1000;
	val=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,val); 		
}
