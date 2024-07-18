//-----------------------------------------------------------------
// ��������:
//     ���ݴ�����-IIC��������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)���ݴ�����-IIC��ʼ��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��4.3��/2.8�� TFTҺ����
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "system.h"
//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
// MCU������IO��������
#define MCU_CT_SDA_IN()  {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=0<<5*2;}	// PB5����ģʽ
#define MCU_CT_SDA_OUT() {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=1<<5*2;} 	// PB5���ģʽ
// MCU������IO��������	 
#define MCU_CT_IIC_SCL_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)
#define MCU_CT_IIC_SCL_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)
#define MCU_CT_IIC_SDA_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)
#define MCU_CT_IIC_SDA_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)
#define MCU_CT_READ_SDA 	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)   // ����SDA 

//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
// IIC���в�������
extern void CT_IIC_Init(void);                // ��ʼ��IIC��IO��				 
extern void CT_IIC_Start(void);								// ����IIC��ʼ�ź�
extern void CT_IIC_Stop(void);	  						// ����IICֹͣ�ź�
extern void CT_IIC_Send_Byte(u8 txd);					// IIC����һ���ֽ�
extern u8 CT_IIC_Read_Byte(unsigned char ack);// IIC��ȡһ���ֽ�
extern u8 CT_IIC_Wait_Ack(void); 							// IIC�ȴ�ACK�ź�
extern void CT_IIC_Ack(void);									// IIC����ACK�ź�
extern void CT_IIC_NAck(void);								// IIC������ACK�ź�

#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
