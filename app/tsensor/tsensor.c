#include "tsensor.h"
#include "adc.h"
void T_adc_init()
{
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADCʱ�Ӻ�PA��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //���� ADC ʱ�ӣ� ADCCLK�� 72/6=12M

	ADC_DeInit(ADC1);	   //������ ADCx ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;   //ADC����ģʽ ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //ADCͨ��ɨ��ģʽ����ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //ADC����ɨ�軹�ǵ���ģʽ������ɨ��
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ADC������ʽ ʹ���������
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //ADC���ݶ��䷽ʽ ʹ�������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=1; //ADC����ת��ͨ���� 1��
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_TempSensorVrefintCmd(ENABLE);	//ʹ���¶ȴ��������ڲ��ο���ѹͨ��
	ADC_Cmd(ADC1,ENABLE);  //ʹ��ADC1
	ADC_ResetCalibration(ADC1);  // ����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡ ADC ����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);  //��ʼָ�� ADC ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));	//��ȡָ�� ADC ��У׼����
}
int Get_Temper()	   //��ȡ�ڲ��¶�ֵ	  
{
	u16 adc_value=0;
	double temp;
	int value;
	adc_value=ADC_Get_Average(ADC_Channel_16,20); 
	temp=adc_value*3.3/4096;
	temp=(1.43-temp)/0.0043+25;  //�ڲ��¶ȼ��㹫ʽ
	value=temp*100;
	return value;					
}

