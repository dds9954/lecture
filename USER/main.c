#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "uart4.h"
#include "timer.h"
#include "ads115.h"
#include "oled.h"
#include "dma.h"
#include "ADS1292.h"
#include "exti.h"
#include "mlx90614.h" 
#include "beep.h"
#include "adc.h"
/************************************************

2020电子设计竞赛-TI杯  综合测试程序
说明：本程序只是2020 TI杯所要用到的传感器功能测试程序。
作者：李孟然
版本：V1.1

					硬件说明
---------------------------------------------------
硬件名称		|	标号		|	引脚	|	备注
---------------------------------------------------
USART		|	RXD			|	PA10	|
			|	TXD			|	PA9		|
---------------------------------------------------
UART4		|	RXD			|	PC11	|
			|	TXD			|	PC10	|
---------------------------------------------------
KEY			|	KEY_UP		|	PA4		|
			|	KEY_LEFT	|	PA5		|
			|	KEY_RIGHT	|	PA6		|
			|	KEY_DOWN	|	PA7		|
			|	KEY_SURE	|	PC4		|
			|	KEY_CANCEL	|	PC5		|
---------------------------------------------------
LED			|	LED_R		|	PC0		|
			|	LED_G		|	PC1		|
			|	LED_B		|	PC2		|
---------------------------------------------------
BEEP		|	BEEP		|	PC3		|
---------------------------------------------------
PWM			|	TIME4_CH1	|	PB6		|
			|	TIME4_CH2	|	PB7		|
			|	TIME4_CH3	|	PB8		|
			|	TIME4_CH4	|	PB9		|
---------------------------------------------------
ADC			|	ADC1_CH0	|	PA0		|
			|	ADC1_CH1	|	PA1		|
			|	ADC1_CH2	|	PA2		|
			|	ADC1_CH3	|	PA3		|
---------------------------------------------------
ADS1292		|	PWDN		|	PB10	|
			|	START		|	PB11	|
			|	DRDY		|	PC6		|
			|	CS0			|	PB12	|
			|	MOSI		|	PB15	|
			|	SCK			|	PB13	|
			|	MISO		|	PB14	|
---------------------------------------------------
MLX90614	|	SCL			|	PA8		|
			|	SDA			|	PC9		|
---------------------------------------------------
ADS1115 	|	SCL			|	PD2		|
			|	SDA			|	PC12	|
---------------------------------------------------
1.3 OLED	|	SCL			|	PB0		|
			|	SDA			|	PB1		|
---------------------------------------------------
FLASH		|	SCL			|	PC8		|
			|	SDA			|	PC7		|
---------------------------------------------------
USB    		|	USB_DP		|	PA12	|
			|	USB_DM		|	PA11	|


编译结果里面的几个数据的意义：
Code：表示程序所占用 FLASH 的大小（FLASH）
RO-data：即 Read Only-data， 表示程序定义的常量，如 const 类型（FLASH）
RW-data：即 Read Write-data， 表示已被初始化的全局变量（SRAM）
ZI-data：即 Zero Init-data， 表示未被初始化的全局变量(SRAM)

************************************************/
extern unsigned char Plane_BMP[];   

int main(void)
{		
	u16 result;         	//用于ADS1115的变量
	float result_val;

	u16 adcx;           	//用于MCU_ADC的变量

	u16 len,t;          	//用于串口4的变量

	u8 key;             	//用于按键的变量

	u8 test_flag = 0;   	//测试项选择按键

	float temp;         	//LMT70温度变量

	u8 dtbuf[50];       	//OELD字符串显示缓存 

	u16 pwm = 1500;     	//舵机控制变量   0-180°  500-2500

	u8 i,sum;	    	//用于ADS1292的变量
	u8 data_to_send[60];	//串口发送缓存
	u32 cannle[2];	    	//存储两个通道的数据
	s32	p_Temp[2];	    	//数据缓存	

	data_to_send[0]=0xAA;  	//上位机发送数据数据头
	data_to_send[1]=0xAA;
	data_to_send[2]=0xF1;	
	data_to_send[3]=8;	 
	 
	delay_init();	    	 		//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	     		//串口一初始化为115200
	uart4_init(115200);	     		//串口四初始化为115200
	LED_Init();			     		//LED端口初始化
	BEEP_Init(); 					//蜂鸣器初始化
	KEY_Init();  					//按键初始化

	TIM3_Int_Init(4999,7199);				//10Khz的计数频率，计数到5000为500ms  
	TIM4_PWM_Init(19999,71); 				//不分频。PWM频率=72000000/72=1000000/20000=50Hz

	OLED_Init();			 				//初始化OLED  
	OLED_Clear();            				//OLED清屏
	OLED_DrawBMP(0,0,128,8,Plane_BMP);   	//显示飞机图片

	ADS1115_Init();    						//ADS1115初始化

	DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);	//串口1DMA设置
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); 					//DMA使能	
	ADS1292_Init();	        				//初始化ads1292
	EXTIX_Init();         					//初始化外部中断输入 

	Adc_Init();		  						//ADC初始化

	SMBus_Init();           				//MLX90614初始化
	
	
	while(Set_ADS1292_Collect(0))			//0 正常采集  1 1mV1Hz内部侧试信号  2 内部短接噪声测试
	{	
		printf("1292寄存器设置失败\r\n");
		delay_s(1);		
	}	
	printf("寄存器设置成功\r\n");
	delay_s(1);	       	//延时1秒
	
	BEEP = 1;			//蜂鸣器开
	delay_ms(300);		//延时300毫秒
	BEEP = 0;			//蜂鸣器关
	
	OLED_Clear();    	//OLED清屏
		
   	while(1)
	{
		key = KEY_Scan(0);  		//按键扫描函数
		if(key == KEY_SURE_PRES)
		{
			test_flag++;            //测试项加一
			OLED_Clear();           //OLED清屏
			if(test_flag == 7)      //测试完毕，从头测试
				test_flag = 0;
		}
/*----------------------按键测试-----------------------*/
		  
		if(test_flag == 0)
		{

			OLED_ShowNum(64,0,KEY_UP,1,16);
			OLED_ShowNum(32,2,KEY_LEFT,1,16);
			OLED_ShowNum(96,2,KEY_RIGHT,1,16);
			OLED_ShowNum(64,4,KEY_DOWN,1,16);
			OLED_ShowNum(32,6,KEY_SURE,1,16);
			OLED_ShowNum(96,6,KEY_CANCEL,1,16);

		}
/*-------------------红外测温传感器测试-----------------*/		  
		else if(test_flag == 1)
		{
			OLED_ShowString(0,0,"GY906_TEST",16);
			
			temp = Get_GY906();							//得到温度值

			sprintf((char *)dtbuf,"temp:%.3f",temp);	//转变为字符串 
			OLED_ShowString(0,2,dtbuf,16);				//OLED显示

		}
/*-------------------PWM舵机&电机测试-----------------*/		  
		else if(test_flag == 2)
		{
			OLED_ShowString(0,0,"PWM_TEST",16);
			
			TIM_SetCompare1(TIM4,pwm);     			//舵机控制范围500-2500
			TIM_SetCompare2(TIM4,1500);    			//舵机控制范围500-2500
			TIM_SetCompare3(TIM4,1500);    			//舵机控制范围500-2500
			TIM_SetCompare4(TIM4,1500);    			//舵机控制范围500-2500

			sprintf((char *)dtbuf,"PWM:%d  ",pwm);	//转变为字符串 
			OLED_ShowString(0,2,dtbuf,16);			//OLED显示

			if(key == KEY_LEFT_PRES)                //按左键减小角度
				pwm -= 100;
			else if(key == KEY_RIGHT_PRES)			//按右键增加角度
				pwm += 100;		  

		}		
/*-------------------ADS1115&LMT70测试----------------*/		  
		else if(test_flag == 3)
		{
			OLED_ShowString(0,0,"ADS1115_TEST",16);	
			
			result=lvbo(0xe3,0xc2);                 	//得到通道一数值
			result_val=4.096*2*result/65535*1000;     	//转换成电压(单位mv)
			sprintf((char *)dtbuf,"%.3f",result_val);	//转变为字符串 
			OLED_ShowString(0,2,dtbuf,16);				//OLED显示

			result=lvbo(0xe3,0xd2);                 	//得到通道二数值
			result_val=4.096*2*result/65535;           	//转换成电压(单位mv)			  
			sprintf((char *)dtbuf,"%.3f",result_val);	//转变为字符串 
			OLED_ShowString(64,2,dtbuf,16);				//OLED显示

			result=lvbo(0xe3,0xe2);                 	//得到通道三数值
			result_val=4.096*2*result/65535;          	//转换成电压(单位mv)			  
			sprintf((char *)dtbuf,"%.3f",result_val);	//转变为字符串 
			OLED_ShowString(0,4,dtbuf,16);				//OLED显示

			result=lvbo(0xe3,0xf2);                 	//得到通道四数值
			result_val=4.096*2*result/65535;           	//转换成电压(单位mv)		  
			sprintf((char *)dtbuf,"%.3f",result_val);	//转变为字符串 
			OLED_ShowString(64,4,dtbuf,16);				//OLED显示

		}
/*-------------------单片机自带ADC测试----------------*/		  
		else if(test_flag == 4)
		{
			OLED_ShowString(0,0,"MCU_ADC_TEST",16);

			adcx=Get_Adc_Average(ADC_Channel_0,10); 	//得到ADC1的通道0
			sprintf((char *)dtbuf,"%d",adcx);	 		//转变为字符串
			OLED_ShowString(0,2,dtbuf,16);				//OLED显示

			adcx=Get_Adc_Average(ADC_Channel_1,10);		//得到ADC1的通道1
			sprintf((char *)dtbuf,"%d",adcx);			//转变为字符串
			OLED_ShowString(64,2,dtbuf,16);				//OLED显示

			adcx=Get_Adc_Average(ADC_Channel_2,10);		//得到ADC1的通道2
			sprintf((char *)dtbuf,"%d",adcx);			//转变为字符串
			OLED_ShowString(0,4,dtbuf,16);				//OLED显示

			adcx=Get_Adc_Average(ADC_Channel_3,10);		//得到ADC1的通道3
			sprintf((char *)dtbuf,"%d",adcx);			//转变为字符串
			OLED_ShowString(64,4,dtbuf,16);				//OLED显示		  
		}
/*----------------------串口四测试-------------------*/		  
		else if(test_flag == 5)
		{
			OLED_ShowString(0,0,"UART4_TEST",16);	

			if(UART4_RX_STA&0x8000)       	//串口四收到数据
			{					   
				len=UART4_RX_STA&0x3fff;  	//得到此次接收到的数据长度
				u4_printf("\r\n您发送的消息为:\r\n\r\n");
				for(t=0;t<len;t++)			//循环将串口四的数据发送出去
				{
					USART_SendData(UART4, UART4_RX_BUF[t]);					//向串口4发送数据
					while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);	//等待发送结束
				}
				u4_printf("\r\n\r\n");		//插入换行
				UART4_RX_STA=0;
			}
		}
/*----------------------ADS1292测试-------------------*/		  
		else if(test_flag == 6)
		{
			OLED_ShowString(0,0,"ADS1292_TEST",16);

			if(ads1292_recive_flag)
			{										
				cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];	//获取原始数据		
				cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];

				p_Temp[0] = get_volt(cannle[0]);		//把采到的3个字节转成有符号32位数
				p_Temp[1] = get_volt(cannle[1]);		//把采到的3个字节转成有符号32位数

				//有符号数为再转为无符号，无符号数为逻辑右移
				cannle[0] = p_Temp[0];
				cannle[1]	= p_Temp[1];
				data_to_send[4]=cannle[0]>>24;			//25-32位
				data_to_send[5]=cannle[0]>>16;  		//17-24
				data_to_send[6]=cannle[0]>>8;			//9-16
				data_to_send[7]=cannle[0]; 				//1-8

				data_to_send[8]=cannle[1]>>24;			//25-32位
				data_to_send[9]=cannle[1]>>16;  		//17-24
				data_to_send[10]=cannle[1]>>8;			//9-16
				data_to_send[11]=cannle[1];			 	//1-8

				for(i=0;i<12;i++)
				sum += data_to_send[i];							
				data_to_send[12] = sum;					//校验和																		
				DMA_Enable(DMA1_Channel4,13);			//串口1DMA 
													
				ads1292_recive_flag=0;
				sum = 0;	
			}

		}			  
		
	}	 
}


