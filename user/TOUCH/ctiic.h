//-----------------------------------------------------------------
// 程序描述:
//     电容触摸屏-IIC驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)电容触摸屏-IIC初始化
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、4.3寸/2.8寸 TFT液晶屏
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "system.h"
//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
// MCU触摸屏IO方向设置
#define MCU_CT_SDA_IN()  {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=0<<5*2;}	// PB5输入模式
#define MCU_CT_SDA_OUT() {GPIOB->MODER&=~(3<<(5*2));GPIOB->MODER|=1<<5*2;} 	// PB5输出模式
// MCU触摸屏IO操作函数	 
#define MCU_CT_IIC_SCL_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)
#define MCU_CT_IIC_SCL_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)
#define MCU_CT_IIC_SDA_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)
#define MCU_CT_IIC_SDA_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)
#define MCU_CT_READ_SDA 	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)   // 输入SDA 

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
// IIC所有操作函数
extern void CT_IIC_Init(void);                // 初始化IIC的IO口				 
extern void CT_IIC_Start(void);								// 发送IIC开始信号
extern void CT_IIC_Stop(void);	  						// 发送IIC停止信号
extern void CT_IIC_Send_Byte(u8 txd);					// IIC发送一个字节
extern u8 CT_IIC_Read_Byte(unsigned char ack);// IIC读取一个字节
extern u8 CT_IIC_Wait_Ack(void); 							// IIC等待ACK信号
extern void CT_IIC_Ack(void);									// IIC发送ACK信号
extern void CT_IIC_NAck(void);								// IIC不发送ACK信号

#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
