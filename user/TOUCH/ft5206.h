//-----------------------------------------------------------------
// ��������:
//     FT5206 ��������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)FT5206��ʼ��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��4.3��/2.8�� TFTҺ����
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef __FT5206_H
#define __FT5206_H	
#include "system.h"
//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
// ����ݴ��������ӵ�оƬ����(δ����IIC����) 
// MCU�������ӿ�
#define FT_RST_MCU_L  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)// FT5206��λ����
#define FT_RST_MCU_H  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET)// FT5206��λ����
#define FT_INT_MCU    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) // FT5206�ж�����	

   
// I2C��д����	
#define FT_CMD_WR 				0X70    // д����
#define FT_CMD_RD 				0X71		// ������
  
// FT5206 ���ּĴ������� 
#define FT_DEVIDE_MODE 		0x00   	// FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER 0x02		// ����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  // ��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		// �ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		// ���������������ݵ�ַ
#define FT_TP4_REG 				0X15		// ���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		// ��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		// �汾		
#define FT_ID_G_MODE 					0xA4   	// FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP				0x80   	// ������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   	// ����״̬�������üĴ���

//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
extern u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len);
extern void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len);
extern u8 FT5206_Init(void);
extern u8 FT5206_Scan(u8 mode);

#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
