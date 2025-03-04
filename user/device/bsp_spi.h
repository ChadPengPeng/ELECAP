//-----------------------------------------------------------------
// 程序描述:
//    	延时程序头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 当前版本: V1.0
// 历史版本:
//   - V1.0: 基于STM32的延时：us,ms
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 
//
//-----------------------------------------------------------------
#ifndef BSP_SPI_H_PROTECT
#define BSP_SPI_H_PROTECT
#include "bsp_system.h"

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
extern u8 SPI1_ReadWriteByte(u8 TxData);
extern void W25Q_SPI_Enable(void);
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
