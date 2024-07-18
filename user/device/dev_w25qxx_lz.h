//-----------------------------------------------------------------
// 程序描述:
//     W25QXX驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)W25QXX初始化和配置
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef __W25QXX_H
#define __W25QXX_H
#include "bsp_system.h"
//-----------------------------------------------------------------
// 声明
//-----------------------------------------------------------------
extern u16 W25QXX_TYPE;					// 定义W25QXX芯片型号	

//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
// W25X系列/Q系列芯片列表	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

#define	W25QXX_CS_H 		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)  		// W25QXX的片选信号
#define	W25QXX_CS_L 		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)  		// W25QXX的片选信号

//指令表
#define W25X_WriteEnable				0x06 
#define W25X_WriteDisable				0x04 
#define W25X_ReadStatusReg1			0x05 
#define W25X_ReadStatusReg2			0x35 
#define W25X_ReadStatusReg3			0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData						0x03 
#define W25X_FastReadData				0x0B 
#define W25X_FastReadDual				0x3B 
#define W25X_PageProgram				0x02 
#define W25X_BlockErase					0xD8 
#define W25X_SectorErase				0x20 
#define W25X_ChipErase					0xC7 
#define W25X_PowerDown					0xB9 
#define W25X_ReleasePowerDown		0xAB 
#define W25X_DeviceID						0xAB 
#define W25X_ManufactDeviceID		0x90 
#define W25X_JedecDeviceID			0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern void W25QXX_Init(void);
extern u16  W25QXX_ReadID(void);  	    			 // 读取FLASH ID
extern u8 W25QXX_ReadSR(u8 regno);             // 读取状态寄存器 
extern void W25QXX_4ByteAddr_Enable(void);     // 使能4字节地址模式
extern void W25QXX_Write_SR(u8 regno,u8 sr);   // 写状态寄存器
extern void W25QXX_Write_Enable(void);  			 // 写使能 
extern void W25QXX_Write_Disable(void);				 // 写保护
extern void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   // 读取flash
extern void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);// 写入flash
extern void W25QXX_Erase_Chip(void);    	  		// 整片擦除
extern void W25QXX_Erase_Sector(u32 Dst_Addr);	// 扇区擦除
extern void W25QXX_Wait_Busy(void);           	// 等待空闲
extern void W25QXX_PowerDown(void);        			// 进入掉电模式
extern void W25QXX_WAKEUP(void);								// 唤醒
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
