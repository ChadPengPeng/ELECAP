//-----------------------------------------------------------------
// ��������:
//     ��������������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)��������ʼ��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��4.3��/2.8�� TFTҺ����
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "system.h"
//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
#define TP_PRES_DOWN 0x80  		// ����������	  
#define TP_CATH_PRES 0x40  		// �а��������� 
#define CT_MAX_TOUCH 10    		// ������֧�ֵĵ���,�̶�Ϊ5��
#define Line_Size	2				 		// ������ϸ�̶�

#define T_PEN     HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)    	// T_PEN
#define T_MISO    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)    	// T_MISO
#define T_MOSI_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)  // T_MOSI
#define T_MOSI_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)   	// T_MOSI
#define T_CLK_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)  // T_SCK
#define T_CLK_H   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)   	// T_SCK
#define T_CS_L   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)  // T_CS 
#define T_CS_H   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET)   	// T_CS 

#define Key_Down 					0x80
#define Key_Up   					0x00
#define FLASH_BASE_ADDR (0x08020000)
//-----------------------------------------------------------------
// ����
//-----------------------------------------------------------------
//������������
typedef struct
{
	void (*init)(void);		// ��ʼ��������������
	u8 (*scan)(u8);				// ɨ�败����.0,��Ļɨ��;1,��������;	 
	u16 x[CT_MAX_TOUCH]; 	// ��ǰ����
	u16 y[CT_MAX_TOUCH];	// �����������10������
	u16 X0;								// ������ԭʼ����
	u16 Y0;
	u16 X; 								// ����������/��ʱ����
	u16 Y;	
	u16 sta;							// �ʵ�״̬ 
												// b15:����1/�ɿ�0; 
	                      // b14:0,û�а�������;1,�а�������. 
												// b13~b10:����
												// b9~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
//---------------������У׼����(����������ҪУ׼)-----------------								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
// �����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
// b0~2:��ʾ����	
// b3~6:����.
// b7:0,������
//    1,������ 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	// ������������touch.c���涨��

//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
extern void TP_GPIO_Init(void);
extern void SPI_GPIO_Init(void);
extern void TP_SPI_Write_Byte(u8 Digital);
extern u8 TP_SPI_Read_Byte(void) ;
extern u8 TP_SPI_Read_Write_Data(SPI_TypeDef* SPIx,u8 Digital);
extern void Flash_Write(u8 *data, u32 WriteAddr, u16 NumOfFLASHWORD);
extern void Flash_Read(u8 *data, u32 ReadAddr, u16 NumOfByte);
extern u16 TP_Read_AD(u8 CMD)	;
extern u16 TP_Read_XY (u8 xy);
extern u8 Read_TP ( u16 *x, u16 *y );
extern u8 Read_TP_2 ( u16 *x, u16 *y ) ;
extern u8 Read_TP_Once (void);
extern void Convert_Pos(void);
extern void Drow_Touch_Point ( u16 x, u16 y );
extern void Draw_Big_Point_1(u16 x, u16 y);
extern void ADJ_INFO_SHOW(unsigned char *str);
extern void Save_AdjData(void);
extern u8 Get_AdjData(void);
extern void Touch_Adjust(void);
extern void TP_Init(void);
 
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
