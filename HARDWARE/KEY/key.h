#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"	 
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/


#define KEY_UP      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ�ϰ���
#define KEY_LEFT    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ�󰴼�
#define KEY_RIGHT   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ�Ұ���
#define KEY_DOWN    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//��ȡ�°���
#define KEY_SURE    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//��ȡȷ�ϰ���
#define KEY_CANCEL  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//��ȡȡ������

#define KEY_UP_PRES 	1	//KEY_UP����
#define KEY_DOWN_PRES	2	//KEY_LEFT����
#define KEY_LEFT_PRES   3	//KEY_RIGHT����
#define KEY_RIGHT_PRES 	4	//KEY_DOWN����
#define KEY_SURE_PRES	5	//KEY_SURE����
#define KEY_CANCEL_PRES	6	//KEY_CANCEL����

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
