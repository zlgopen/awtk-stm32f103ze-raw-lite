#ifndef _adc_H
#define _adc_H

#include "sys.h"
#include "delay.h"
void adc_init(void);
void adc5_init(void);
u16 ADC_Getvalue(u8 ch); //��ȡADCת��ֵ	 ch:(ADC_Channel_0  --  ADC_Channel_17)
u16 ADC_Get_Average(u8 ch,u8 time);  //��ȡ��ȡ���ͨ��ADCת����ƽ��ֵ

#endif
