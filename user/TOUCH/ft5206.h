//-----------------------------------------------------------------
// 程序描述:
//     FT5206 驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)FT5206初始化
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、4.3寸/2.8寸 TFT液晶屏
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef __FT5206_H
#define __FT5206_H	
#include "system.h"
//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
// 与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
// MCU触摸屏接口
#define FT_RST_MCU_L  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)// FT5206复位引脚
#define FT_RST_MCU_H  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET)// FT5206复位引脚
#define FT_INT_MCU    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) // FT5206中断引脚	

   
// I2C读写命令	
#define FT_CMD_WR 				0X70    // 写命令
#define FT_CMD_RD 				0X71		// 读命令
  
// FT5206 部分寄存器定义 
#define FT_DEVIDE_MODE 		0x00   	// FT5206模式控制寄存器
#define FT_REG_NUM_FINGER 0x02		// 触摸状态寄存器

#define FT_TP1_REG 				0X03	  // 第一个触摸点数据地址
#define FT_TP2_REG 				0X09		// 第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		// 第三个触摸点数据地址
#define FT_TP4_REG 				0X15		// 第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		// 第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		// 版本		
#define FT_ID_G_MODE 					0xA4   	// FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP				0x80   	// 触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   	// 激活状态周期设置寄存器

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len);
extern void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len);
extern u8 FT5206_Init(void);
extern u8 FT5206_Scan(u8 mode);

#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
