#ifndef _rtc_H
#define _rtc_H
#include "sys.h"
#include "delay.h"

//ʱ��ṹ��
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//������������
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��
extern u8 rtc_data[7],rtc_bit;
extern u8 const mon_table[12];	//�·��������ݱ�

u8 rtc_init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec);//����ʱ��
#endif
