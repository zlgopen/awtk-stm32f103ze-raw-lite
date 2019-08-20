#include "gui.h"

#ifdef USE_FONT_UPDATE
#include "ff.h"
#include "diskio.h"
#endif

#ifdef USE_FLASH_CHAR
#include "flash.h"
#else
#include "asciicode.h"
#include "chfont.h"
#endif

#ifdef USE_FONT_UPDATE
#include "malloc.h"
#endif


/****************************************************************************
* Function Name  : GUI_Dot
* Description    : �ڲ������滭һ��
* Input          : x�����X����
*                * y�����Y����
*                * color�������ɫ
* Output         : None
* Return         : None
****************************************************************************/

void GUI_Dot(uint16_t x, uint16_t y, uint16_t color)
{  
	TFT_SetWindow(x, y, x, y);  //���õ��λ��
	TFT_WriteData(color);       //����	
}


/****************************************************************************
*��������GUI_Line 
*��  �룺xStart��ֱ�ߵ���ʼX����
*      * yStart��ֱ�ߵ���ʵY����
*      * xEnd��ֱ�ߵĽ���X����
*      * yEnd��ֱ�ߵĽ���Y����
*      * color��ֱ�ߵ���ɫ
*��  ����
*��  �ܣ���һ��ֱ��
****************************************************************************/	  

void GUI_Line(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{
	u16 t;  
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;  
	int incx, incy;  
	u16 row, col;  
	delta_x = xEnd - xStart;//������������  
	delta_y = yEnd - yStart;  
	col = xStart;  
	row = yStart;  
	if (delta_x > 0)
	{
		incx=1;//���õ�������
	} 	  
	else    
	{  
	    if (delta_x == 0)
		{
			incx = 0;//��ֱ�� 
		} 		 
	    else 
			{
				incx = -1;
				delta_x = -delta_x;
			}  
	}  
	if (delta_y > 0)
	{
		incy = 1;
	}	  
	else  
	{  
	    if (delta_y == 0)
		{
			incy = 0;//ˮƽ��  
		} 
	    else 
		{
			incy = -1;
			delta_y = -delta_y;
		}  
	}  
	if (delta_x > delta_y)
	{ 
		distance = delta_x;//ѡȡ��������������  
	}
	else
	{
		distance = delta_y; 
	} 	
	for (t=0; t<=distance+1; t++)  
	{                                     //�������  
	    GUI_Dot(col, row, color);
	    xerr += delta_x;  
	    yerr += delta_y;  
	  	if(xerr > distance)  
	    {  
	        xerr -= distance;  
	        col += incx;  
	    }  
	    if(yerr > distance)  
	    {  
	        yerr -= distance;  
	        row += incy;  
	    }  
	}  
}

//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	GUI_Line(x1,y1,x2,y1,color);
	GUI_Line(x1,y1,x1,y2,color);
	GUI_Line(x1,y2,x2,y2,color);
	GUI_Line(x2,y1,x2,y2,color);
}


//��LCD�ϵģ�x��y��������
//color�������ɫ
void LCD_Draw_Point(u16 x,u16 y,u16 color)
{
    GUI_Dot(x,y,color);
}
//��ָ��λ�� ��ʾ1��16*16�ĺ���
//(x,y):������ʾ��λ��
//index:tfont��������ĵڼ�������
//color:������ֵ���ɫ
void Test_Show_CH_Font16(u16 x,u16 y,u8 index,u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
    for(t=0;t<32;t++)//ÿ��16*16�ĺ��ֵ��� ��32���ֽ�
    {   
		if(t<16)temp=tfont16[index*2][t];      //ǰ16���ֽ�
		else temp=tfont16[index*2+1][t-16];    //��16���ֽ�	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Draw_Point(x,y,color);//��ʵ�ĵ�
			else LCD_Draw_Point(x,y,GRAY);   //���հ׵㣨ʹ�ñ���ɫ��
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//��ָ��λ�� ��ʾ1��24*24�ĺ���
//(x,y):������ʾ��λ��
//index:tfont��������ĵڼ�������
//color:������ֵ���ɫ
void Test_Show_CH_Font24(u16 x,u16 y,u8 index,u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
    for(t=0;t<72;t++)//ÿ��24*24�ĺ��ֵ��� ��72���ֽ�
    {   
		if(t<24)temp=tfont24[index*3][t];           //ǰ24���ֽ�
		else if(t<48)temp=tfont24[index*3+1][t-24]; //��24���ֽ�	                          
        else temp=tfont24[index*3+2][t-48];         //��24���ֽ�
	    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Draw_Point(x,y,color);//��ʵ�ĵ�
			else LCD_Draw_Point(x,y,GRAY);   //���հ׵㣨ʹ�ñ���ɫ��
			temp<<=1;
			y++;
			if((y-y0)==24)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//����2��������ʾ����
void TEST_FONT(void)
{
	u8 t;
	u8 x,x1;
	x=x1=50;
	for(t=0;t<7;t++)//6������
	{
		Test_Show_CH_Font16(x ,50,t,RED);
		Test_Show_CH_Font24(x1,70,t,RED);
		x+=16;
		x1+=24;
	}
}






/****************************************************************************
* Function Name  : GUI_Box
* Description    : ��һ������Ϳ����ɫ
* Input          : sx����ʼX����, sy����ʵY����, 
*                * ex����ֹX����, ey����ֹY����,
*                * color���������ɫ
* Output         : None
* Return         : None
****************************************************************************/

void GUI_Box(uint16_t xState, uint16_t yState, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{ 
	uint16_t temp;

    if((xState > xEnd) || (yState > yEnd))
    {
        return;
    }   
	TFT_SetWindow(xState, yState, xEnd, yEnd); 
    xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while(xState--)
	{
	 	temp = yState;
		while (temp--)
	 	{	
			TFT_WriteData(color);
		}
	}
}

/****************************************************************************
* Function Name  : GUI_DrowSign
* Description    : ��һ��ʮ�ֵı��
* Input          : x����ǵ�X���ꣻ
*                * y����ǵ�Y����
*                * color����ǵ���ɫ
* Output         : None
* Return         : None
****************************************************************************/

void GUI_DrowSign(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t i;

    /* ���� */
    TFT_SetWindow(x-1, y-1, x+1, y+1);
    for(i=0; i<9; i++)
    {
        TFT_WriteData(color);    
    }

    /* ���� */
    TFT_SetWindow(x-4, y, x+4, y);
    for(i=0; i<9; i++)
    {
        TFT_WriteData(color);    
    }

    /* ���� */
    TFT_SetWindow(x, y-4, x, y+4);
    for(i=0; i<9; i++)
    {
        TFT_WriteData(color);    
    }
}

#ifndef USE_FLASH_CHAR
/****************************************************************************
* Function Name  : GUI_Show12ASCII
* Description    : д12��ASCII��
* Input          : x����ʼX����
*                * y����ʼY����
*                * p���ַ����׵�ַ
*                * wordColor��������ɫ
*                * backColor��������ɫ
* Output         : None
* Return         : None
****************************************************************************/

void GUI_Show12ASCII(uint16_t x, uint16_t y, uint8_t *p, 
                     uint16_t wordColor, uint16_t backColor)
{
	uint8_t i, wordByte, wordNum;
	uint16_t color;

	while(*p != '\0')   //����Ƿ������һ����
	{
        /* ���ֿ��е�ASCII���Ǵӿո�ʼ��Ҳ����32��ʼ�ģ����Լ�ȥ32 */
		wordNum = *p - 32;
		
        TFT_SetWindow(x, y, x+7, y+15);           //�ֿ�*��Ϊ��8*16
		for (wordByte=0; wordByte<16; wordByte++) //ÿ����ģһ����16���ֽ�
		{
			color = ASCII8x16[wordNum][wordByte];
			for (i=0; i<8; i++) 
			{
				if ((color&0x80) == 0x80)
				{
					TFT_WriteData(wordColor);
				} 						
				else
				{
					TFT_WriteData(backColor);
				} 	
				color <<= 1;
			}
		}

		p++;    //ָ��ָ����һ����
		
        /* ��Ļ���괦�� */
        x += 8;
        if(x > 233)   //TFT_XMAX -8
        {
            x = 0;
            y += 16;    
        }
	}
}

#else

/****************************************************************************
* Function Name  : GUI_Show12Char
* Description    : ͨ��FLASH�ֿ���ʾ12����ĸ�ͺ��֣�ʹ��GBK��
* Input          : x����ʼX����
*                * y����ʼY����
*                * ch���ַ����׵�ַ
*                * wordColor��������ɫ
*                * backColor��������ɫ
* Output         : None
* Return         : None
****************************************************************************/

void GUI_Show12Char(uint16_t x, uint16_t y, uint8_t *ch, 
                    uint16_t wordColor, uint16_t backColor)
{
    uint8_t i, j, color, buf[32];
    uint16_t asc;
    uint32_t wordAddr = 0;

    while(*ch != '\0')
    {
        /*��ʾ��ĸ */
        if(*ch < 0x80)  //ASCII���0~127
        {
            /* ���ֿ��е�ASCII���Ǵӿո�ʼ��Ҳ����32��ʼ�ģ����Լ�ȥ32 */
    		wordAddr = *ch - 32;
            wordAddr *= 16;
            wordAddr += GUI_FLASH_ASCII_ADDR;
            
            /* ��ȡFLASH�и��ֵ���ģ */
            FLASH_ReadData(buf, wordAddr, 16);
            
            /* ��ʾ������ */		
            TFT_SetWindow(x, y, x+7, y+15);           //�ֿ�*��Ϊ��8*16
    		for (j=0; j<16; j++) //ÿ����ģһ����16���ֽ�
    		{
    			color = buf[j];
    			for (i=0; i<8; i++) 
    			{
    				if ((color&0x80) == 0x80)
    				{
    					TFT_WriteData(wordColor);
    				} 						
    				else
    				{
    					TFT_WriteData(backColor);
    				} 	
    				color <<= 1;
    			}
    		}
    
    		ch++;    //ָ��ָ����һ����
    		
            /* ��Ļ���괦�� */
            x += 8;
            if(x > 233)   //TFT_XMAX -8
            {
                x = 0;
                y += 16;    
            }            
        }
        /* ��ʾ���� */
        else
        {
            /* �����ֱ���ת������FLASH�еĵ�ַ */
            asc = *ch - 0x81;     //���ֽ��Ǳ�ʾ�����������Ǵ�0x81��0xFE,����ת���ɵ�ַ-0x80
            wordAddr = asc * 190; //ÿ������һ����190����
    
            asc = *(ch + 1); //���ֽڴ���ÿ������ÿ��������λ�ã����Ǵ�0x40��0xFF
            if(asc < 0x7F)   //��0x7Fλ���и���λ����������ȡģ�����գ����Դ���0x7F֮����һ
            {
                asc -= 0x40;
            }
            else
            {
                asc -= 0x41;
            }
            
            wordAddr += asc; //�����GBK���ǵڼ�����
            wordAddr *= 32;  //����λ��ת��λFLASH��ַ
            wordAddr += GUI_FLASH_12CHAR_ADDR; //�����׵�ַ
    
            /* ��ȡFLASH�и��ֵ���ģ */
            FLASH_ReadData(buf, wordAddr, 32);
    
            /* �ڲ���������ʾ */
            TFT_SetWindow(x, y, x+15, y+15);
            for(i=0; i<32; i++)
            {
                 
                color = buf[i];            
                for(j=0; j<8; j++) 
        		{
        			if((color & 0x80) == 0x80)
        			{
        				TFT_WriteData(wordColor);
        			} 						
        			else
        			{
        				TFT_WriteData(backColor);
        			} 
        			color <<= 1;
        		}//for(j=0;j<8;j++)����
            }
    
            /* ��Ļ���괦�� */
            x += 16;
            if(x > 225)   //TFT_XMAX -15
            {
                x = 0;
                y += 16;    
            }
    
            /* д��һ���֣�ÿ������ռ�����ֽ�����+2 */
            ch += 2;             
        }
    }    
}
/****************************************************************************
* Function Name  : GUI_Show12Chinese
* Description    : ͨ��FLASH�ֿ���ʾ12�ź��֣�ʹ��GBK��
* Input          : x����ʼX����
*                * y����ʼY����
*                * cn���ַ����׵�ַ
*                * wordColor��������ɫ
*                * backColor��������ɫ
* Output         : None
* Return         : None
****************************************************************************/

void GUI_Show16Chinese(uint16_t x, uint16_t y, uint8_t *cn, 
                       uint16_t wordColor, uint16_t backColor)
{   
    uint8_t i, j, color, buf[63];
    uint16_t asc;
    uint32_t wordAddr = 0;    
    while(*cn != '\0')
    {  
        /* �����ֱ���ת������FLASH�еĵ�ַ */
        asc = *cn - 0x81;     //���ֽ��Ǳ�ʾ�����������Ǵ�0x81��0xFE,����ת���ɵ�ַ-0x80
        wordAddr = asc * 190; //ÿ������һ����190����

        asc = *(cn + 1); //���ֽڴ���ÿ������ÿ��������λ�ã����Ǵ�0x40��0xFF
        if(asc < 0x7F)   //��0x7Fλ���и���λ����������ȡģ�����գ����Դ���0x7F֮����һ
        {
            asc -= 0x40;
        }
        else
        {
            asc -= 0x41;
        }
        
        wordAddr += asc; //�����GBK���ǵڼ�����
        wordAddr *= 63;  //����λ��ת��λFLASH��ַ
        wordAddr += GUI_FLASH_16CHAR_ADDR; //�����׵�ַ

        /* ��ȡFLASH�и��ֵ���ģ */
        FLASH_ReadData(buf, wordAddr, 63);

        /* �ڲ���������ʾ */
        TFT_SetWindow(x, y, x+23, y+20);
        for(i=0; i<63; i++)
        {
            color = buf[i];            
            for(j=0; j<8; j++) 
    		{
    			if((color & 0x80) == 0x80)
    			{
    				TFT_WriteData(wordColor);
    			} 						
    			else
    			{
    				TFT_WriteData(backColor);
    			} 
    			color <<= 1;
    		}//for(j=0;j<8;j++)����
        }

        /* ��Ļ���괦�� */
        x += 21;
        if(x > 218)   //TFT_XMAX -21
        {
            x = 0;
            y += 21;    
        }

        /* д��һ���֣�ÿ������ռ�����ֽ�����+2 */
        cn += 2;      
    }    
}

#endif


#ifdef PICTURE_SHOW

void GUI_ShowPicture(uint x, uint y, uint wide, uint high)
{
	uint temp = 0;
	long tmp=0,num=0;
	TFT_SetWindow(x, y, x+wide-1, y+high-1);
	num = wide * high*2 ;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		TFT_WriteData(temp);//�����ʾ
		tmp += 2;
	}
	while(tmp < num);	
}

#endif


#ifdef USE_FONT_UPDATE

/****************************************************************************
* Function Name  : GUI_FontUpdate
* Description    : �����ֿ�
* Input          : updateState��ѡ����µ��ֿ�
* Output         : None
* Return         : None
****************************************************************************/

void GUI_FontUpdate(uint8_t updateState)
{
    FRESULT res;
	FIL fsrc;
	UINT  br;
    uint32_t wordAddr, i, j;

#ifdef __MALLOC_H	
    uint8_t *p;
    p = malloc(4096);                  //����һ���ڴ��
    if(p == 0)
    {
        return;
    }
#else
	uint8_t buffer[512];
#endif    	

    /* ����ASCII�ֿ� */
    if((updateState & GUI_UPDATE_ASCII) == GUI_UPDATE_ASCII)
    {
        /* ����д����ʼ��ַ */
        wordAddr = GUI_FLASH_ASCII_ADDR;
        j = 0;

        /* �򿪶�ȡ�ļ� */
        res = f_open(&fsrc, GUI_ASCII_FILE, FA_READ);	
    	if(res == FR_OK)  //�򿪳ɹ�
        { 
         	for (;;)      //��ʼ��ȡ����
         	{       
#ifdef __MALLOC_H	
                res = f_read(&fsrc, p, 4096, &br);
    
                /* ����ȡ��������д��FLASH */
                FLASH_WriteData(p, wordAddr, br);
          	    wordAddr += br;   //д���ַ����

#else
                res = f_read(&fsrc, buffer, sizeof(buffer), &br);
    
                /* ����ȡ��������д��FLASH */
                FLASH_WriteData(buffer, wordAddr, br);
          	    wordAddr += br;   //д���ַ����
#endif
                j += br;
                i = j * 100 / 1456;
                GUI_Box(0, 80, i, 90, RED);    

                if (res || br == 0)
    			{
    				break;    // error or eof 
    			}
            } 
        }
    	 
        f_close(&fsrc);  //�����Ǵ򿪣������½��ļ���һ���ǵùر�
    }

    /* ����12�ź��ֿ� */
    if((updateState & GUI_UPDATE_12CHAR) == GUI_UPDATE_12CHAR)
    {
        wordAddr = GUI_FLASH_12CHAR_ADDR;
        j = 0;

        res = f_open(&fsrc, GUI_12CHAR_FILE, FA_READ);
    	
    	if(res == FR_OK) 
        {  
         	for (;;)  
         	{      
#ifdef __MALLOC_H	
                res = f_read(&fsrc, p, 4096, &br);
    
                /* ����ȡ��������д��FLASH */
                FLASH_WriteData(p, wordAddr, br);
          	    wordAddr += br;   //д���ַ����
#else
                res = f_read(&fsrc, buffer, sizeof(buffer), &br);
    
                FLASH_WriteData(buffer, wordAddr, br);
          	    wordAddr += br;
#endif
                j += br;
                i = j * 100 / 766080;
                GUI_Box(0, 95, i, 105, RED);    

    
                if (res || br == 0)
    			{
    				break;    // error or eof 
    			}
            } 
        }
        f_close(&fsrc);  //�����Ǵ򿪣������½��ļ���һ���ǵùر�
    }
    
    /* ����16�ź��ֿ� */
    if((updateState & GUI_UPDATE_16CHAR) == GUI_UPDATE_16CHAR)
    {
        
        wordAddr = GUI_FLASH_16CHAR_ADDR;
        j = 0;

        res = f_open(&fsrc, GUI_16CHAR_FILE, FA_READ);	
    	if(res == FR_OK) 
        { 
         	for (;;)  
         	{       
#ifdef __MALLOC_H	
                res = f_read(&fsrc, p, 4096, &br);
    
                /* ����ȡ��������д��FLASH */
                FLASH_WriteData(p, wordAddr, br);
          	    wordAddr += br;   //д���ַ����
#else
                res = f_read(&fsrc, buffer, sizeof(buffer), &br);
    
                FLASH_WriteData(buffer, wordAddr, br);
          	    wordAddr += br;
#endif
                j += br;
                i = j * 100 / 1508220;
                GUI_Box(0, 110, i, 120, RED);    

    
                if (res || br == 0)
    			{
    				break;    // error or eof 
    			}
            } 
        }
    	 
        f_close(&fsrc);  //�����Ǵ򿪣������½��ļ���һ���ǵùر�
    }
#ifdef __MALLOC_H	
    free(p);
#endif
} 
 
#endif


