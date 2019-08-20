#include "ds18b20.h"

/*******************************************************************************
* �� �� ��         : ds18b20_init
* ��������		   : IO�˿�ʱ�ӳ�ʼ������	   
* ��    ��         : ��
* ��    ��         : 0:��⵽DS18B20��1��û�м�⵽
*******************************************************************************/
u8 ds18b20_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Pin=dq;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_ds18b20,&GPIO_InitStructure);
	GPIO_SetBits(GPIO_ds18b20,dq);	   //����

	ds18b20rst();	  //����һ����ʼ������ȴ�DS18B20Ӧ��
	return ds18b20_Check();
}

void ds18b20rst()
{
	DQOUTINT();//���
	ds18b20_dq_L;
	delay_us(750);//��ʱ750΢��	
	ds18b20_dq_H;
	delay_us(15);//��ʱ15΢��
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 ds18b20_Check(void) 	   
{   
	u8 retry=0;
	DQININT();	//SET PG11 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

/*******************************************************************************
* �� �� ��         : DQININT
* ��������		   : ��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void DQININT()	 //��������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=dq;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_ds18b20,&GPIO_InitStructure);	
}

/*******************************************************************************
* �� �� ��         : DQOUTINT
* ��������		   : �������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void DQOUTINT()	 //�������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=dq;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_ds18b20,&GPIO_InitStructure);	
}

/*******************************************************************************
* �� �� ��         : ds18b20init
* ��������		   : DS18B20��ʼ��ʱ��	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ds18b20init()
{
	DQOUTINT();//���
	ds18b20_dq_L;
	delay_us(480);//��ʱ480΢��	
	ds18b20_dq_H;
	delay_us(480);//��ʱ480΢��
}

/*******************************************************************************
* �� �� ��         : ds18b20wr
* ��������		   : DS18B20д����ʱ��	   
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/
void ds18b20wr(u8 dat)
{
	u8 i=0;
	DQOUTINT();//���

	for(i=0;i<8;i++)
	{
		ds18b20_dq_L;	 //����
		delay_us(15);//��ʱ15΢��
		
		if((dat&0x01)==1)
		{
			ds18b20_dq_H;
		}
		else
		{
			ds18b20_dq_L;
		}
		delay_us(60);//��ʱ60΢��
		ds18b20_dq_H;
		
		dat>>=1;//׼����һλ���ݵķ���	
	}
}

/*******************************************************************************
* �� �� ��         : DS18b20rd
* ��������		   : DS18B20������ʱ��	   
* ��    ��         : ��
* ��    ��         : value
*******************************************************************************/
u8 DS18b20rd()
{
	u8 i=0,value=0;

	for(i=0;i<8;i++)
	{
		value>>=1;
		DQOUTINT();//���
		ds18b20_dq_L;	 //����
		delay_us(4);//��ʱ4΢��
		ds18b20_dq_H;
		delay_us(10);//��ʱ10΢��
		DQININT();	 //��������

		if(GPIO_ReadInputDataBit(GPIO_ds18b20,dq)==1)
		{
		   value|=0x80;//������ �ӵ�λ��ʼ
		}

		delay_us(45);//��ʱ45΢��
	}

	return value;	
}

/*******************************************************************************
* �� �� ��         : readtemp
* ��������		   : DS18B2�¶ȼĴ������ã��¶ȶ�ȡ	   
* ��    ��         : ��
* ��    ��         : value
*******************************************************************************/
double readtemp()			  //��ȡ�¶�����Ҫ��λ��
{
	u16 temp;
	u8 a,b;
	double value;
	ds18b20init();		//��ʼ��
	ds18b20wr(0xcc);   //���ͺ���ROMָ��
	ds18b20wr(0x44);   //�����¶�ת��ָ��
	delay_ms(10);
	ds18b20init();	   //��ʼ��
	ds18b20wr(0xcc);   //���ͺ���ROMָ��
	ds18b20wr(0xbe);   //�����ݴ���ָ��
	a=DS18b20rd();	 //�¶ȵĵͰ�λ
	b=DS18b20rd();	 //�¶ȵĸ߰�λ
	temp=b;
	temp=(temp<<8)+a;
	if((temp&0xf800)==0xf800)
	{
		temp=(~temp)+1;
		value=temp*(-0.0625);
	}
	else
	{
		value=temp*0.0625;	
	}
	return value;
}

