#include "spi.h"
#include "delay.h" 
/*--------------------------------------------------------

2020������ƾ���-TI��  �ۺϲ��Գ���
˵����������ֻ��2020 TI����Ҫ�õ��Ĵ��������ܲ��Գ���
���ߣ�����Ȼ
�汾��V1.1	 

---------------------------------------------------------*/
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 				//����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 		//SPI����8bit����
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 					  //SCLK����Ϊ�ͣ���������������
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 					//�ڶ���ʱ�������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//Ƭѡ���������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //  �ٶ����� 4M
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 		//��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7; 							//CRC����ֵ�Ķ���ʽ
	SPI_Init(SPI2,&SPI_InitStructure); 	

	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
}    

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}

