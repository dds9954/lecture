#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_UP==0||KEY_LEFT==0||KEY_RIGHT==0||KEY_DOWN==0||KEY_SURE==0||KEY_CANCEL==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_UP==0)return KEY_UP_PRES;
		else if(KEY_LEFT==0)return KEY_LEFT_PRES;
		else if(KEY_RIGHT==0)return KEY_RIGHT_PRES;
		else if(KEY_DOWN==0)return KEY_DOWN_PRES;
		else if(KEY_SURE==0)return KEY_SURE_PRES;
		else if(KEY_CANCEL==0)return KEY_CANCEL_PRES;
		
	}else if(KEY_UP==1&&KEY_LEFT==1&&KEY_RIGHT==1&&KEY_DOWN==1&&KEY_SURE==1&&KEY_CANCEL==1)key_up=1; 	    
 	return 0;// �ް�������
}
