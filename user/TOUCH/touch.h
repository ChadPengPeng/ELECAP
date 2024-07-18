//-----------------------------------------------------------------
// 程序描述:
//     触摸屏驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)触摸屏初始化
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、4.3寸/2.8寸 TFT液晶屏
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "system.h"
//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
#define TP_PRES_DOWN 0x80  		// 触屏被按下	  
#define TP_CATH_PRES 0x40  		// 有按键按下了 
#define CT_MAX_TOUCH 10    		// 电容屏支持的点数,固定为5点
#define Line_Size	2				 		// 线条粗细程度

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
// 声明
//-----------------------------------------------------------------
//触摸屏控制器
typedef struct
{
	void (*init)(void);		// 初始化触摸屏控制器
	u8 (*scan)(u8);				// 扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	u16 x[CT_MAX_TOUCH]; 	// 当前坐标
	u16 y[CT_MAX_TOUCH];	// 电容屏有最多10组坐标
	u16 X0;								// 电阻屏原始坐标
	u16 Y0;
	u16 X; 								// 电阻屏最终/暂时坐标
	u16 Y;	
	u16 sta;							// 笔的状态 
												// b15:按下1/松开0; 
	                      // b14:0,没有按键按下;1,有按键按下. 
												// b13~b10:保留
												// b9~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
//---------------触摸屏校准参数(电容屏不需要校准)-----------------								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
// 新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
// b0~2:显示方向	
// b3~6:保留.
// b7:0,电阻屏
//    1,电容屏 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	// 触屏控制器在touch.c里面定义

//-----------------------------------------------------------------
// 函数声明
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
