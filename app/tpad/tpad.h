#ifndef _tpad_H
#define _tpad_H
#include "sys.h"
#include "delay.h"
extern vu16 tpad_default_val; 
u8 tpad_init(u32 psc);	//����0����ʼ���ɹ�������1����ʼ��ʧ��
u8 tpad_scan(u8 mode);

#endif
