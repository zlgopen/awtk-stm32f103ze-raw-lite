#include "adc.h"

void adc_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADCʱ�Ӻ�PA��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //���� ADC ʱ�ӣ� ADCCLK�� 72/6=12M

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	 //ģ������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);	   //������ ADCx ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;   //ADC����ģʽ ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //ADCͨ��ɨ��ģʽ����ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //ADC����ɨ�軹�ǵ���ģʽ������ɨ��
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ADC������ʽ ʹ���������
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //ADC���ݶ��䷽ʽ ʹ�������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=1; //ADC����ת��ͨ���� 1��
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1,ENABLE);  //ʹ��ADC1
	ADC_ResetCalibration(ADC1);  // ����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡ ADC ����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);  //��ʼָ�� ADC ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));	//��ȡָ�� ADC ��У׼����
			
}

void adc5_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADCʱ�Ӻ�PA��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //���� ADC ʱ�ӣ� ADCCLK�� 72/6=12M

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	 //ģ������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);	   //������ ADCx ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;   //ADC����ģʽ ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //ADCͨ��ɨ��ģʽ����ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //ADC����ɨ�軹�ǵ���ģʽ������ɨ��
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ADC������ʽ ʹ���������
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //ADC���ݶ��䷽ʽ ʹ�������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=1; //ADC����ת��ͨ���� 1��
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1,ENABLE);  //ʹ��ADC1
	ADC_ResetCalibration(ADC1);  // ����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡ ADC ����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);  //��ʼָ�� ADC ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));	//��ȡָ�� ADC ��У׼����
			
}
u16 ADC_Getvalue(u8 ch) //��ȡADCת��ֵ	 ch:0-3(ADC_Channel_0  --  ADC_Channel_17)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);//����ADC����ͨ��������ʱ��
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //ADC��ʼ���ת��
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));  //�ȴ�ת�����
	return ADC_GetConversionValue(ADC1);	 //�������һ��ת�����			
}
u16 ADC_Get_Average(u8 ch,u8 time)  //��ȡ��ȡ���ͨ��ADCת����ƽ��ֵ
{
	u8 i;
	u32 temp=0;
	for(i=0;i<time;i++)
	{
		temp+=ADC_Getvalue(ch);	
		delay_ms(5);	//�����ʱ�����
	}
	return (u16)(temp/time);		
}

