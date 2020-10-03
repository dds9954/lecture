#ifndef  __ADS115_H_
#define  __ADS115_H_
#include "sys.h"
 /*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/


/***************************************************************************************
*˵������������ʱ��������Ϊ�����źţ�������Ϊ�ص��������źŲ���Ϊ����ѹ(���ܱȵص�λ��)
*	   ˫������ʱ��������Ϊ�����źţ�������Ϊ�����������źŵĲ�ֵ����Ϊ����ѹ
****************************************************************************************/
#define	 SDA_A1     PCout(12)=1   //SDA���
#define	 SDA_A0     PCout(12)=0
#define	 SCL_A1	    PDout(2)=1    //SCL
#define	 SCL_A0	    PDout(2)=0
#define	 SDA_AI	    PCin(12)   //SDA����


//I2C��ַ�Լ���д����
#define  WR_REG 0x92       //д�Ĵ���
#define  RE_REG 0x93       //���Ĵ���

/***********************************�Ĵ���������**********************************************/
#define  DATA_REG  0x00		//ת�����ݼĴ���
#define  CONF_REG  0x01     //���������üĴ���
#define  LOTH_REG  0x02		//��ͷ�ֵ�Ĵ���
#define  HITH_REG  0x03		//��߷�ֵ�Ĵ���

#define  ch0  0xc0       //ͨ��0
#define  ch1  0xd0       //ͨ��1
#define  ch2  0xe0       //ͨ��2
#define  ch3  0xf0       //ͨ��3

/***********************����������*************************************************************
*|  OS | MUX2 | MUX1 | MUX0 | PGA2 | PGA1 | PGA0 | MODE  |------HCMD
*|  DR2| DR1  | DR0  | COMP_MODE | COMP_POL | COMP_LAT |  COMP_QUE1 | COMP_QUE0 |-----LCMD
***********************************************************************************************/
#define  HCMD1    0x64   //AIN0�������� +-4.096����  ����ģʽ  01000100b
#define  LCMD1	  0xf0	 //860sps ���ڱȽ���ģʽ �������Ч  �������ź����� ÿ���ڼ�ֵⷧ 11110000b

/************************��������****************************/
static void ADS1115_delay(u16 D);
void delay_nms(u16 ms);
void delay_nus(u16 us);
void ADS1115_Init(void);
void I2CStart_A(void);
void I2CStop_A(void);
void I2CWriteByte_A(u8 DATA);
u8 I2CReadByte_A(void);
void ADS1115Config_A(u8 LCMD,u8 HCMD);
void SetThresHold_A(u16 L_TH,u16 H_TH);        //�ߵͷ�������
u16 ReadAD_A(void);
u16 getad(u8 LCMD,u8 HCMD);
u16 lvbo(u8 LCMD,u8 HCMD);





#endif		
