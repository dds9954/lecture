#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/

u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

 			
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   			
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   

void SPI1_Init(void);			 //��ʼ��SPI��
void SPI2_Init(void);			 //��ʼ��SPI��	


void OLED_SPI_Init(void);



#endif


