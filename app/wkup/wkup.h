#ifndef _wkup_H
#define _wkup_H
#include "sys.h"
#include "delay.h"
#define WKUP_KD PAin(0)	   //PA0��

void wkup_init(void);
void Sys_Standyby(void);	  //����ģʽ
u8 Check_Wkup(void);	//����1������ ����0������
void Sys_Enter_WkupStandby(void);


#endif
