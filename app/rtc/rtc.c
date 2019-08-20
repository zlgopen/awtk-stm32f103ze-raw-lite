#include "rtc.h"
#include "usart.h"

_calendar_obj calendar;//ʱ�ӽṹ��
u8 rtc_data[7],rtc_bit=0;

void RTC_NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void RTC_IRQHandler()
{
	if(RTC_GetITStatus(RTC_IT_SEC)!= RESET) //�������ж�
	{
		RTC_Get();  //����ʱ��
		rtc_bit=1;		
	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  	
	  	RTC_Get();				//����ʱ��   
  		printf("Alarm Time:%d-%d-%d %d:%d:%d\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//�������ʱ��		
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();	
}

//����ֵ0����ʼ��������1��������
u8 rtc_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);//�򿪺󱸱�������ʱ�Ӻ͵�Դʱ��
	PWR_BackupAccessCmd(ENABLE); //ʹ��RTC�ͺ�����Ĵ�������
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x5050) //��ȡ�󱸼Ĵ�����BKP_DR1�е�ֵ���������0x5050��һ�γ�ʼ������
	{
		BKP_DeInit(); //��BKP��ȫ���Ĵ�����λ
		RCC_LSEConfig(RCC_LSE_ON);   //�����ⲿ���پ��� LSE��
		/*while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET&&temp<800)//�ж��ⲿ32.768K�����Ƿ�׼����
		{
			temp++;	
			delay_ms(10);
		}
		if(temp>=800)	 //ʱ�Ӽ�ⳬʱ����һ�������ź�
		{
			return 1;
		}*/
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	  //����RTCʱ��
		RCC_RTCCLKCmd(ENABLE);	 //ʹ��RTCʱ��
		RTC_WaitForLastTask(); //�ȴ����һ�ζ� RTC �Ĵ�����д�������
		RTC_WaitForSynchro();  //�ȴ�RTC�Ĵ���ʱ��ͬ��
		RTC_ITConfig(RTC_IT_SEC,ENABLE); //ʹ�����ж�
		RTC_WaitForLastTask(); //�ȴ����һ�ζ� RTC �Ĵ�����д�������
		RTC_EnterConfigMode(); //���� RTC ����ģʽ
		RTC_SetPrescaler(32767);  //����Ԥ��Ƶֵ ʹ��֮ǰһ��Ҫ����RTCģʽ���ã�֮ǰ��Ҫ�ȴ�RTC�Ĵ����������
		RTC_WaitForLastTask(); //�ȴ����һ�ζ� RTC �Ĵ�����д�������
		RTC_Set(2015,10,30,13,59,56);  //���õ�ǰʱ��
		RTC_ExitConfigMode(); //�˳� RTC ����ģʽ
		BKP_WriteBackupRegister(BKP_DR1,0x5050);  //��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else  //�����ǵ�һ�ν���ϵͳ��ô������ʱ,�жϱ�׼����ͨ���󱸼Ĵ���BKP_DR1��ֵ0x5050
	{
		RTC_WaitForSynchro();  //�ȴ�RTC�Ĵ���ʱ��ͬ��
		RTC_ITConfig(RTC_IT_SEC,ENABLE); //ʹ�����ж�
		RTC_WaitForLastTask(); //�ȴ����һ�ζ� RTC �Ĵ�����д�������			
	}
	RTC_NVIC_Config();  //RTC���жϷ������ú���
	RTC_Get();  //RTC����ʱ�亯��
	return 0;		
}


//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��  0,����
//�ܱ�4�������ܱ�100�����Ļ����ܱ�400�����������꣬��������ƽ��
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 		

//�� 1970 �� 1 �� 1 ��0ʱ0��0��Ϊ��׼
//1970~2099 ��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//ƽ���·�����
u8 RTC_Set(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(year<1970||year>2099) //�������޷��ش���
	{
		return 1;
	}
	for(t=1970;t<year;t++)	 //�����������������ۼ�
	{
		if(Is_Leap_Year(t))	  //�жϸ����Ƿ�Ϊ����
		{
			seccount+=31622400;	//���������	
		}
		else
		{
			seccount+=31536000;	//ƽ�������	
		}	
	}
	month-=1;	//���㵽ǰ��һ���£�����ͨ��������������
	for(t=0;t<month;t++)	//��ǰ���·ݵ������ۼ�
	{
		seccount+=(u32)mon_table[t]*86400; //ÿһ�������*���������·�����
		if(Is_Leap_Year(year)&&t==1)  //����������2�·ݱ�ƽ������һ�������
		{
			seccount+=86400; //ÿһ�������*���������·�����	
		}			
	}
	seccount+=(u32)(day-1)*86400; //��ǰ�����ڵ������ۼ�
	seccount+=(u32)hour*3600; //��ʱ�������ۼ�
	seccount+=(u32)min*60; //�ѷֵ������ۼ�
	seccount+=(u32)sec; //����������ۼ�
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE); //ʹ��RTC�ͺ�����Ĵ�������
	RTC_SetCounter(seccount);	//����RTC��������ֵ
	RTC_WaitForLastTask(); //�ȴ����һ�ζ� RTC �Ĵ�����д�������
	return 0;		
}

//��ʼ������		  
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//syear,smon,sday,hour,min,sec�����ӵ�������ʱ����   
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ 			    
	//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	//���������Ǳ����!
	
	RTC_SetAlarm(seccount);
 
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	
	return 0;	    
}


//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  


u8 RTC_Get()  //�õ���ǰ��ʱ�䱣���� calendar �ṹ������  ����0���ɹ���1������
{
	static u16 daycnt=0;
	u32 timecount=0;
	u32 temp=0;
	u16 temp1=0;
	timecount=RTC_GetCounter();	//��ȡRTC�������е�ֵ ����
	temp=timecount/86400; //�õ�����
	if(daycnt!=temp)  //����������ڵ���1
	{
		daycnt=temp;
		temp1=1970;  //��1970�꿪ʼ
		while(temp>=365)
		{
			if(Is_Leap_Year(temp1)) //���������
			{
				if(temp>=366)
				{
					temp-=366;	 //���������
				}
				else 
				{
					temp1++;
					break;
				}
						
			}
			else temp-=365;	 //ƽ�������
			temp1++;
		}
		calendar.w_year=temp1;	//�õ����
		temp1=0;
		while(temp>=28)  //������һ����
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//�����ǲ�������/2 �·�
			{
				if(temp>=29)temp-=29;//�����������
				else break;
			}
			else
			{ 
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;	
		}
		calendar.w_month=temp1+1;	//�õ��·�
		calendar.w_date=temp+1;  	//�õ�����
	}
	temp=timecount%86400; //�õ������� 
	calendar.hour=temp/3600; //Сʱ
	calendar.min=(temp%3600)/60; //����
	calendar.sec=(temp%3600)%60; //����
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);
	//��ȡ����
	return 0;	
}
