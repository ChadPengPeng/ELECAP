//-----------------------------------------------------------------
// 程序描述:
//     FT5206 驱动程序
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

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "ft5206.h"
#include "touch.h"
#include "ctiic.h"
#include "dev_lcd.h"
#include "bsp_delay.h" 
#include "string.h" 
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
//-----------------------------------------------------------------
// 
// 函数功能: 向FT5206写入一次数据
// 入口参数: u16 reg：起始寄存器地址
//					 u8 *buf：数据缓缓存区
//					 u8 len：写数据长度
// 返 回 值: 0,成功;1,失败.
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	// 发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   // 发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    CT_IIC_Send_Byte(buf[i]);  	// 发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)
			break;  
	}
  CT_IIC_Stop();								// 产生一个停止条件	    
	return ret; 
}
	
//-----------------------------------------------------------------
// void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
//-----------------------------------------------------------------
// 
// 函数功能: 从FT5206读出一次数据
// 入口参数: u16 reg：起始寄存器地址
//					 u8 *buf：数据缓缓存区
//					 u8 len：写数据长度
// 返 回 值: 0,成功;1,失败.
// 注意事项: 无
//
//-----------------------------------------------------------------
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	// 发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	// 发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	// 发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); // 发数据	  
	} 
  CT_IIC_Stop();									// 产生一个停止条件     
} 

//-----------------------------------------------------------------
// u8 FT5206_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: 初始化FT5206触摸屏
// 入口参数: 无
// 返 回 值: 0,初始化成功;1,初始化失败 
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 FT5206_Init(void)
{
	u8 temp[2]; 
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			// 开启GPIOA时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			// 开启GPIOD时钟
							
	GPIO_Initure.Pin=GPIO_PIN_8;            // PA8中断引脚
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      // 输入
	GPIO_Initure.Pull=GPIO_PULLUP;          // 上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; // 高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);     // 初始化
					
	GPIO_Initure.Pin=GPIO_PIN_3;            // PD3复位
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // 推挽输出
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     // 初始化
   
	CT_IIC_Init();      // 初始化电容屏的I2C总线  

	FT_RST_MCU_L;				// 复位
	delay_ms(20);
	FT_RST_MCU_H;				// 释放复位

	delay_ms(50);  	
	temp[0]=0;
	FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);	// 进入正常操作模式 
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);		// 查询模式 
	temp[0]=80;														// 触摸有效值，80，越小越灵敏	
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);// 设置触摸有效值
	temp[0]=12;														// 激活周期，不能小于12，最大14
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	// 读取版本号，参考值：0x3003
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)// 版本:0X3003/0X0001/0X0002
	{
		return 0;
	} 
	return 1;
}

const u16 FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

//-----------------------------------------------------------------
// u8 FT5206_Scan(u8 mode)
//-----------------------------------------------------------------
// 
// 函数功能: 扫描触摸屏(采用查询方式)
// 入口参数: u8 mode：正常扫描.
// 返 回 值: 0,初始化成功;1,初始化失败 
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 FT5206_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);// 读取触摸点的状态  
	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFF<<(mode&0XF);// 将点的个数转换为1的位数,匹配tp_dev.sta定义 
		tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
		for(i=0;i<5;i++)
		{
			if(tp_dev.sta&(1<<i))		// 触摸有效
			{
				FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	// 读取XY坐标值 
				if(lcddev.dir==DISPLAY_DIR_R)
				{
					tp_dev.y[i]=lcddev.height-(((u16)(buf[0]&0X0F)<<8)+buf[1])-1;
					tp_dev.x[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
				}
				else if(lcddev.dir==DISPLAY_DIR_L)
				{
					tp_dev.y[i]=(((u16)(buf[0]&0X0F)<<8)+buf[1]);
					tp_dev.x[i]=lcddev.width-(((u16)(buf[2]&0X0F)<<8)+buf[3])-1;
				}
				else if(lcddev.dir==DISPLAY_DIR_U)
				{
					tp_dev.x[i]=lcddev.width-(((u16)(buf[0]&0X0F)<<8)+buf[1])-1;
					tp_dev.y[i]=lcddev.height-(((u16)(buf[2]&0X0F)<<8)+buf[3])-1;
				}  
				else
				{
					tp_dev.x[i]=(((u16)(buf[0]&0X0F)<<8)+buf[1]);
					tp_dev.y[i]=(((u16)(buf[2]&0X0F)<<8)+buf[3]);
				} 
				if((buf[0]&0XF0)!=0X80)
					tp_dev.x[i]=tp_dev.y[i]=0;// 必须是contact事件，才认为有效
			}			
		} 
		res=1;
		if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	// 读到的数据都是0,则忽略此次数据
	}
	if((mode&0X1F)==0)// 无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	// 之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	// 标记按键松开
		}
		else						// 之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	// 清除点有效标记	
		}	 
	}
	return res;
}
 
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
