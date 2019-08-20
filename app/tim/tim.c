#include "tim.h"
#include "sys.h"
#include "delay.h"

u8 TIM5CH1_CAPTURE_STA=0;
u16 TIM5CH1_CAPTURE_VAL=0;

extern u16 xval,yval,Scores,Level_Up,Level,Vel;
extern u8 k,m,k1;
extern u16 xc[4],yc[4];
void tim3_pwm_init(u32 arr,u32 pre)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;	
	TIM_OCInitTypeDef 	TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʹ��TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOC, ENABLE);  //ʹ�ܶ˿�Cʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE); //��TIM3����CH2��ȫӳ�䵽PC7��

	TIM_TimeBaseStructure.TIM_Period= arr;  //����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler= pre;//������������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1; //������ʱ�ӷָ� �������0��ôʱ�Ӿ���72M,�������36M
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //ѡ���˼�����ģʽ ���ϼ���
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);  //��ʼ��TIM3
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;		//ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //�Ƚ����ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //������Ը�
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);	//TIM3 �� CCR2 �ϵ�Ԥװ�ؼĴ���ʹ��
	TIM_Cmd(TIM3,ENABLE);	 //ʹ��TIM3����

}
void TIM3_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
void tim5_input1_init(u32 arr,u32 pre)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);  //ʹ�ܶ˿�Aʱ��
	
	TIM_TimeBaseStructure.TIM_Period= arr;  //����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler= pre;//������������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1; //������ʱ�ӷָ� �������0��ôʱ�Ӿ���72M,�������36M
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //ѡ���˼�����ģʽ ���ϼ���
	TIM_TimeBaseInit(TIM5, & TIM_TimeBaseStructure);  //��ʼ��TIM5
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //TIM5ͨ��1 
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; // �����ز���
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ѡ��IC1ӳ��TI1��
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�������벶��Ԥ��Ƶ��
	TIM_ICInitStructure.TIM_ICFilter=0x00;//��ʹ���˲���
	TIM_ICInit(TIM5,&TIM_ICInitStructure);

	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);	//������ʱ�������жϺ����벶���ж�

	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;  //��ʱ��5
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM5,ENABLE);	 //ʹ��TIM5����		
}
void TIM5_IRQHandler(void)
{
	if((TIM5CH1_CAPTURE_STA&0x80)==0)	   //û�гɹ�����
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)	  //��������ж�
		{
			if((TIM5CH1_CAPTURE_STA&0x40)==1)	//�ɹ�����ߵ�ƽ
			{
				if((TIM5CH1_CAPTURE_STA&0x3f)==0x3f) //�ߵ�ƽʱ�����
				{	
					TIM5CH1_CAPTURE_STA|=0x80; //ǿ�Ƴɹ�����һ��
					TIM5CH1_CAPTURE_VAL=0xffff;		
				}
				else
				{			   
					TIM5CH1_CAPTURE_STA++;	
				}			
			}		
		}
		if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)	//���������ж�
		{
			if(TIM5CH1_CAPTURE_STA&0x40)  //����һ���½���
			{
				TIM5CH1_CAPTURE_STA|=0x80;  //��־���λ1���ɹ�����һ��
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);	//��ȡTIM5ͨ��1����ֵ
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);//���������ز���		
			}
			else
			{
				TIM5CH1_CAPTURE_STA=0;
				TIM5CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM5,0);	 //���TIM5������
				TIM5CH1_CAPTURE_STA|=0x40;	//��ǲ�����������
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);//�����½��ز���		
			}
		}				
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);	 //������ºͲ����жϱ�־	
}
/*void TIM3_IRQHandler(void)
{
	u8 flag=0,i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	}
}  */



