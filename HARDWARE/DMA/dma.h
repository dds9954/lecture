#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/


void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar);
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);
	   
#endif




