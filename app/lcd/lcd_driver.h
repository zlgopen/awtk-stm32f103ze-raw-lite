#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

/* ����ͷ�ļ� */
#include"stm32f10x.h"

/* TFT��ַ�ṹ�� */
typedef struct
{
	u16 TFT_CMD;
	u16 TFT_DATA;
} TFT_TypeDef;
/* ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� */ 
/* ע������ʱSTM32�ڲ�������һλ����! 111110=0X3E */			    
#define TFT_BASE        ((uint32_t)(0x6C000000 | 0x000007FE))
#define TFT             ((TFT_TypeDef *) TFT_BASE)

/* �������Ĵ�С */
#define TFT_XMAX 319		//����TFT���Ĵ�С
#define TFT_YMAX 479

/* ������ɫ�ĺ� */

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ


#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ) 

/* ����ȫ�ֱ��� */
void TFT_WriteCmd(uint16_t cmd);
void TFT_WriteData(uint16_t dat);
void TFT_Init(void);
void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void TFT_ClearScreen(uint16_t color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
u16 LCD_ReadPoint(u16 x,u16 y);
#endif

