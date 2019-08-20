#include "tpad.h"
#include "usart.h"
#define tpad_arr_max_val 0xffff
vu16 tpad_default_val=0;
void tim5_input2_init(u32 arr,u32 pre)	//TIM5ͨ��1���벶��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // PA0

	TIM_TimeBaseStructure.TIM_Period= arr;  //����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler= pre;//������������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1; //������ʱ�ӷָ� �������0��ôʱ�Ӿ���72M,�������36M
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //ѡ���˼�����ģʽ ���ϼ���
	TIM_TimeBaseInit(TIM5, & TIM_TimeBaseStructure);  //��ʼ��TIM5
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //TIM5ͨ��1 
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; // �����ز���
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ѡ��IC1ӳ��TI1��
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�������벶��Ԥ��Ƶ��
	TIM_ICInitStructure.TIM_ICFilter=0x03;//ʹ���˲��� 8����ʱ��ʱ�������˲�
	TIM_ICInit(TIM5,&TIM_ICInitStructure);

	TIM_Cmd(TIM5,ENABLE);	 //ʹ��TIM5����		
}
void tpad_reset()  //����������λ�����������PA0Ϊ���ģʽ��ʹ���ݷŵ�
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // PA0
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	delay_ms(5);   //��ʱ5ms
	TIM_SetCounter(TIM5,0);	 //��TIM5����������
	TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);  //����жϱ�־
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //��������ģʽ	
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // PA0	
}
u16 tpad_get_val()	 //�õ���ʱ������ֵ�������ʱֱ�ӷ��ض�ʱ������ֵ
{
	tpad_reset(); //��λtpad���зŵ���ȫ
	while(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)	 //�ȴ����
	{
		if(TIM_GetCounter(TIM5)>tpad_arr_max_val-500)  //�����ʱ����ʱ��ֱ�ӷ��ض�ʱ��������ֵ
		{
			return TIM_GetCounter(TIM5); //����TIM5��CNTֵ	
		}			
	}
	return TIM_GetCapture1(TIM5);  //����TIM5ͨ��1�Ĳ���ֵ		
}
u8 tpad_init(u32 psc)	//����0����ʼ���ɹ�������1����ʼ��ʧ��
{
	u8 i,j;
	u16 buf[10],temp;
	tim5_input2_init(tpad_arr_max_val,psc-1); //��1MƵ�ʼ���
	for(i=0;i<10;i++)		 //������ȡ10�β���ֵ
	{
		buf[i]=tpad_get_val();
		delay_ms(10);		
	}
	for(i=0;i<9;i++)   //��С��������
	{
		for(j=i+1;j<10;j++)	
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;		
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)	//ȡ�м�6�����ݵ�ƽ����
	{
		temp+=buf[i];	
	}
	tpad_default_val=temp/6;
	printf("tpad_default_val= %d\r\n",tpad_default_val); //�����ӡĬ�ϵĲ���ʱ��ֵ
	if(tpad_default_val>tpad_arr_max_val/2)return 1;
	return 0;		
}
u16 tpad_get_maxval(u8 n) //��ȡn�Σ�ȡ���ֵ
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=tpad_get_val();
		if(temp>res)
		{
			res=temp;
		}
	}
	return res;		
}
//ɨ�败������
//mode:0,��֧����������,��һ�����ɿ�һ���ٽ��Ű���һ�Σ�1��֧����������
//����ֵ0��û�д�����1���д���
#define tpad_gate_val 100   //��������ֵ������tpad_default_val+tpad_gate_val����Ϊ���д���
u8 tpad_scan(u8 mode)
{
	static u8 keyen=0;	 //0�����Կ�ʼ��⣬>0�����ܿ�ʼ���
	u8 sample=3;	  //Ĭ�ϲ���������3
	u8 res=0;
	u16 rval;
	if(mode)
	{	
		sample=6;	 //���Ϊ�������£�����������6
		keyen=0;
	}
	rval=tpad_get_maxval(sample);
	if(rval>(tpad_default_val+tpad_gate_val)) //����  ��Ч
	{
		if(keyen==0)
		{
			res=1;
		}
		printf("rval= %d\r\n",rval);
		keyen=3;  //����Ҫ����3��֮����㴥����Ч			
	}
	if(keyen)keyen--;
	return res;
} 
