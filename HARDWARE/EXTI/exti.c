#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "ADS1292.h"
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/   
//�ⲿ�жϷ������
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	EXTI_ClearITPendingBit(EXTI_Line6);//����жϱ�־
	//GPIOA.0	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6); 

	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						 //�ⲿ�ж�ʹ��
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			 //ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

}

//�ⲿ�ж�0������� 
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->IMR&EXTI_Line6 && ADS_DRDY==0)//���ݽ����ж�				
	{		
		EXTI_ClearITPendingBit(EXTI_Line6); //���LINE0�ϵ��жϱ�־λ  
		ADS1292_Read_Data(ads1292_Cache);//���ݴ浽9�ֽڻ�����
		ads1292_recive_flag=1;
	}	
}
 

 
