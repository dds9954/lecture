#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/

	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_s(u8 s);

#endif





























