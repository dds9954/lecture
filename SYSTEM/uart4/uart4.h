#ifndef __UART4_H
#define __UART4_H
#include "stdio.h"	
#include "sys.h" 
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/
 

#define UART4_REC_LEN  			200  	//�����������ֽ��� 200
#define UART4_SEND_LEN  			200  	//����������ֽ��� 200

#define EN_UART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  UART4_RX_BUF[UART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  UART4_TX_BUF[UART4_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 UART4_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart4_init(u32 bound);
void u4_printf(char* fmt,...);  
#endif


