//-----------------------------------------------------------------
// 程序描述:
//     触摸屏驱动程序
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

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "touch.h" 
#include "ft5206.h"
#include "dev_lcd.h"
#include "bsp_delay.h"
#include "stdlib.h"
#include "math.h"
#include "dev_w25qxx_lz.h"
//-----------------------------------------------------------------

_m_tp_dev tp_dev=
{
	TP_Init,
	FT5206_Scan,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	
};					

u8 CMD_RDX =0XD0;
u8 CMD_RDY =0X90;

u32 FLASH_LEN=16*1024*1024;	// FLASH 大小为16M字节
//-----------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------
u16 Time = 1;		// 延时长度
u8 higth = 1;

#define ts_delay   delay_us

//-----------------------------------------------------------------
// void TP_GPIO_Init(void)
//-----------------------------------------------------------------
//
// 函数功能：电阻屏触摸引脚初始化
// 入口参数：无
// 返回参数：无
// 注意事项：
//
//-----------------------------------------------------------------
void TP_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();						// 开启GPIOA时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();						// 开启GPIOD时钟
	
	GPIO_Initure.Pin=GPIO_PIN_3;            // PD3->TP_CS
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          // 上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; // 高速
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     // 初始化
		
	GPIO_Initure.Pin=GPIO_PIN_6;            // PD6->SD_CS
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // 推挽输出
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     // 初始化
	
	GPIO_Initure.Pin=GPIO_PIN_8; 						// PA8中断引脚
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING; // 下升沿触发
	GPIO_Initure.Pull=GPIO_PULLUP;					// 无上下拉
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);			// 初始化
		
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);   // 抢占优先级为2，子优先级为0
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);         // 使能中断线9-5
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);// TP_CS置高
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);// 禁止SD卡
}

//-----------------------------------------------------------------
// void EXTI9_5_IRQHandler(void)
//-----------------------------------------------------------------
//
// 函数功能: 中断线9-5中断服务函数，调用中断处理公用函数
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//-----------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);// 调用中断处理公用函数
}

//-----------------------------------------------------------------
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//-----------------------------------------------------------------
//
// 函数功能: 中断服务程序中需要做的事情
// 入口参数: uint16_t GPIO_Pin：中断引脚
// 返回参数: 无
// 注意事项: 在HAL库中所有的外部中断服务函数都会调用此函数
//
//-----------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_8)
	{
		__HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_8);
		tp_dev.sta=Key_Down;
	}
}

//-----------------------------------------------------------------
// 功能区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void SPI_GPIO_Init(void)
//-----------------------------------------------------------------
//
// 函数功能：SPI引脚初始化
// 入口参数：无
// 返回参数：无
// 注意事项：无
//
//-----------------------------------------------------------------
void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();						// 开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_3;            // PB3->CLK
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // 输出
	GPIO_Initure.Pull=GPIO_PULLUP;          // 上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;// 高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     // 初始化
	
	GPIO_Initure.Pin=GPIO_PIN_5;            // PB5->MOSI
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     // 初始化
	
	GPIO_Initure.Pin=GPIO_PIN_4;            // PB4->MISO
	GPIO_Initure.Mode=GPIO_MODE_INPUT;  		// 输入
	GPIO_Initure.Pull=GPIO_PULLUP;          // 上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;// 高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     // 初始化
}

//-----------------------------------------------------------------
// void TP_SPI_Write_Byte(u8 Digital)
//-----------------------------------------------------------------
//
// 函数功能：用IO口模拟SPI的写一个字节
// 入口参数：无
// 返回参数：无
// 注意事项：数据发送顺序：高位在前，低位在后
//		(1) 只有用IO模拟SPI时序的方法才用的这些函数
// 		(2) 注意SPI主机发送从机接收引脚MOSI的数据有效是在时钟上升沿
//				有效，而主机接收从机发送引脚MISO是在时钟下降沿数据有效，
//				SPI可以全双工通信，但是在本处，主机先发送命令给从机然后
//				从从机发送回馈数据给主机，有先后关系在调用此函数之前，请
//				将片选信号有效，否则写数据不成功
// 
//-----------------------------------------------------------------
void TP_SPI_Write_Byte(u8 Digital)
{
  u8 a = 0;

  for (a = 0; a < 8; a++)
  {
    if (Digital & 0x80)
      T_MOSI_H;
    else
      T_MOSI_L;
    Digital <<= 1;

    ts_delay(Time);
    T_CLK_H;
    ts_delay(Time);
    T_CLK_L;
  }
}

//-----------------------------------------------------------------
// u8 TP_SPI_Read_Byte(void) 
//-----------------------------------------------------------------
//
// 函数功能：用IO口模拟SPI的读一个字节
// 入口参数：无
// 返回参数：无
// 注意事项：
//		(1) 只有用IO模拟SPI时序的方法才用的这些函数
// 		(2) 注意SPI主机发送从机接收引脚MOSI的数据有效是在时钟上升沿
//				有效，而主机接收从机发送引脚MISO是在时钟下降沿数据有效，
//				SPI可以全双工通信，但是在本处，主机先发送命令给从机然后
//				从从机发送回馈数据给主机，有先后关系在调用此函数之前，请
//				将片选信号有效，否则写数据不成功
//
//-----------------------------------------------------------------
u8 TP_SPI_Read_Byte(void) 
{
	unsigned char a=0,b=0;
	for(a=8;a>0;a--)
	{
		T_CLK_H;
		if (higth == 0)
		{
			T_CLK_L;
		}
		else
		{
			ts_delay(Time);	
			T_CLK_L;		
			ts_delay(Time);
		}
		b <<= 1;
		if(T_MISO)
			b |= 0x01;
	}
	T_CLK_H;

	return (b);	
}

//-----------------------------------------------------------------
// u8 TP_SPI_Read_Write_Data(SPI_TypeDef* SPIx,u8 Digital)
//-----------------------------------------------------------------
//
// 函数功能：SPI读写一个字节
// 入口参数：无
// 返回参数：无
// 注意事项：无
//
//-----------------------------------------------------------------
u8 TP_SPI_Read_Write_Data(SPI_TypeDef* SPIx,u8 Digital)
{
	u8 Receivedata=0;
 
	TP_SPI_Write_Byte(Digital);
	Receivedata = TP_SPI_Read_Byte();

	return Receivedata; 
}

//-----------------------------------------------------------------
// 底层读写XPT2046的SPI时序产生区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// u16 TP_Read_AD(u8 CMD) 
//-----------------------------------------------------------------
//
// 函数功能：从TP读出ADC值（SPI读数据）
// 入口参数：u8 CMD	命令
// 返回参数：ADC值
// 注意事项：底层读写函数
//
//-----------------------------------------------------------------
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0;
	u16  Num   = 0; 
 	
	T_CLK_L;											// 先拉低时钟
	T_CS_L; 											// 选中XPT2046 
	TP_SPI_Write_Byte(CMD);				// 发送命令字
	ts_delay(6);									// XPT2046的转换时间最长为6us
  for (count = 0; count < 16; count++)
  {
    Num <<= 1;
    T_CLK_H;
    ts_delay(Time);
    T_CLK_L; // 下降沿有效
    ts_delay(Time);
    if (T_MISO)
      Num++;
  }
  Num >>= 3; // 只有高12位有效
  T_CS_H;    // 释放XPT2046

  return (Num);
}

//-----------------------------------------------------------------
// u16 TP_Read_XY ( u8 xy )
//-----------------------------------------------------------------
//
// 函数功能：读取一个坐标值
// 入口参数：xy
// 返回参数：坐标值
// 注意事项：连续读取READ TIMES次数据，对这些数据升序排序
//					 然后去掉最低和最高LOST_VAL
//
//-----------------------------------------------------------------
#define READ_TIMES 4 		// 读取次数
#define LOST_VAL 1  		// 丢弃值
u16 TP_Read_XY (u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;

	for ( i = 0; i < READ_TIMES; i ++ )
	{				 
 		buf[i] = TP_Read_AD(xy);	    
	}				    
	for ( i = 0; i < READ_TIMES - 1; i ++ )			// 排序
	{
		for ( j = i + 1; j < READ_TIMES; j ++ )
		{
			if ( buf[i] > buf[j] )					      	// 升序
			{
				temp   = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}	  
	sum = 0;
	for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i ++)
	{
		sum += buf[i];
	}
	temp = sum / ( READ_TIMES - 2*LOST_VAL );
	return temp;   
} 

//-----------------------------------------------------------------
// u8 Read_TP ( u16 *x, u16 *y )
//-----------------------------------------------------------------
//
// 函数功能：带滤波的坐标读取
// 入口参数：存放坐标值的指针
// 返回参数：数据读取状态：0：读取失败，1：读取成功
// 注意事项：最小值不能少于50
//
//-----------------------------------------------------------------
u8 Read_TP ( u16 *x, u16 *y )
{
	u16 xtemp, ytemp;
				 	 		  
	xtemp = TP_Read_XY ( CMD_RDX );
	ytemp = TP_Read_XY ( CMD_RDY );	  												   
	if ( xtemp < 50 || ytemp < 50 )	
		return 0;		// 读书失败
	*x = xtemp;		// 保存坐标值
	*y = ytemp;		// 保存坐标值

	return 1;			// 读书成功
}

//-----------------------------------------------------------------
// u8 Read_TP_2 ( u16 *x, u16 *y ) 
//-----------------------------------------------------------------
//
// 函数功能：2次读取TP的值
// 入口参数：存放坐标值的指针
// 返回参数：数据读取状态：0：读取失败，1：读取成功
// 注意事项：连续读取2次有效的AD值，且这两次的偏差不能超过
//					 50，满足条件，则认为读书正确，否则读书错误。
//					 该函数能大大提高准确度。
//
//-----------------------------------------------------------------
#define ERR_RANGE 50							// 误差范围
u8 Read_TP_2 ( u16 *x, u16 *y ) 
{
	u16 x1, y1;
	u16 x2, y2;
	u8 flag;   
	 
	flag = Read_TP (&x1, &y1);   
	if ( flag == 0 )
	{
		return(0);
	}
	flag = Read_TP ( &x2, &y2 );	   
	if ( flag == 0 )
	{
		return(0); 
	}  
	if ( ((( x2 <= x1 ) & ( x1 < ( x2 + ERR_RANGE ))) | (( x1 <= x2 ) & ( x2 < ( x1 + ERR_RANGE ))))	& 		//前后两次采样在±50内
	 		 ((( y2 <= y1 ) & ( y1 < ( y2 + ERR_RANGE ))) | (( y1 <= y2 ) & ( y2 < ( y1 + ERR_RANGE ))))
  )
	{
		*x = ( x1 + x2 ) / 2;
		*y = ( y1 + y2 ) / 2;
		return 1;
	}
	else 
	{
		return 0;	  
	}
} 

//-----------------------------------------------------------------
// u8 Read_TP_Once (void)
//-----------------------------------------------------------------
//
// 函数功能：读取一次坐标值
// 入口参数：无
// 返回参数：数据读取状态：0：读取失败，1：读取成功
// 注意事项：仅仅读取一次，直到T_PEN松开才返回
//
//-----------------------------------------------------------------
u8 Read_TP_Once (void)
{
	u8 t = 0;	 
	   
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);          // 关闭中断线9-5
	
	tp_dev.sta = Key_Up;
  
	Read_TP_2 ( &tp_dev.X, &tp_dev.Y );
	while (( T_PEN == 0 ) && ( t <= 250 ))
	{
		t ++;
		delay_ms ( 10 );
	}
	__HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_8);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);          // 使能中断线9-5
	
	if ( t >= 250 )	
	{
		return 0;									    				// 按下2.5秒认为无效
	}
	else
	{
		return 1;	
	}
}

//-----------------------------------------------------------------
// void Convert_Pos(void)
//-----------------------------------------------------------------
//
// 函数功能：获取坐标值
// 入口参数：无
// 返回参数：无
// 注意事项：根据触摸屏的校准参数来决定转换后的结果，保存在X0,Y0
//
//-----------------------------------------------------------------
void Convert_Pos(void)
{
  uint16_t Xpos    = 0, Ypos    = 0;
  uint16_t Xpos_TS = 0, Ypos_TS = 0;

  
	if ( Read_TP_2 ( &tp_dev.X, &tp_dev.Y ) )
	{
		Xpos_TS = tp_dev.xfac * tp_dev.X + tp_dev.xoff;
		Ypos_TS = tp_dev.yfac * tp_dev.Y + tp_dev.yoff;
        // 坐标变换
    if ( lcddev.id == ST7789V_ID )
    {
      if (Ypos_TS < lcddev.width)
        Xpos   = lcddev.width - Ypos_TS - 1;   
      else 
        Xpos   = 0;   
      if ( Xpos_TS < lcddev.height)
        Ypos   = lcddev.height - Xpos_TS - 1;   
      else 
        Ypos   = 0;   
    }
    else if ( lcddev.id == SPFD5408_ID )
    {
      if (Ypos_TS < lcddev.width)
        Xpos   = lcddev.width - Ypos_TS - 1;   
      else 
        Xpos   = 0;   
      if ( Xpos_TS < lcddev.height)
        Ypos   = lcddev.height - Xpos_TS - 1;   
      else 
        Ypos   = 0;   
    }
    tp_dev.X0 = Xpos;
    tp_dev.Y0 = Ypos;
	}
}	 

//-----------------------------------------------------------------
// 与LCD部分有关的函数
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void Drow_Touch_Point ( u16 x, u16 y )
//-----------------------------------------------------------------
//
// 函数功能：画一个触摸校准定位标识
// 入口参数：要画点的坐标
// 返回参数：无 
// 注意事项：用来校准用
//
//-----------------------------------------------------------------
void Drow_Touch_Point ( u16 x, u16 y )
{
	POINT_COLOR=RED;
	LCD_DrawLine(x-12, y, x+12, y);		// 画一条横线
	LCD_DrawLine(x, y-12, x, y+12);		// 画一条竖线
	
	LCD_DrawRectangle(x-8,y-8,x+8,y+8);		// 画一个矩形
}

//-----------------------------------------------------------------
// void Draw_Big_Point_1(u8 x, u16 y)
//-----------------------------------------------------------------
//
// 函数功能：画一个触摸点
// 入口参数：要画点的坐标
// 返回参数：无
// 注意事项：用来校准用
//
//-----------------------------------------------------------------
void Draw_Big_Point_1(u16 x, u16 y)
{	
	POINT_COLOR=RED;
	LCD_DrawPoint ( x, y);		// 中心点
}

//-----------------------------------------------------------------
// 使用内部FLASH，存储校准点
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void ADJ_INFO_SHOW(unsigned char *str) 
//-----------------------------------------------------------------
//
// 函数功能：显示提示信息
// 入口参数：要显示的字符串
// 返回参数：无  
// 注意事项：无
// 
//-----------------------------------------------------------------
void ADJ_INFO_SHOW(unsigned char *str)
{
	BACK_COLOR 	= WHITE;
	POINT_COLOR = BLACK;						// 红色
	LCD_ShowString (  (lcddev.width - 160) >> 1,  40, 200,16,16, (char *)"x1:       y1:       ");
	LCD_ShowString (  (lcddev.width - 160) >> 1,  60, 200,16,16, (char *)"x2:       y2:       ");
	LCD_ShowString (  (lcddev.width - 160) >> 1,  80, 200,16,16, (char *)"x3:       y3:       ");
	LCD_ShowString (  (lcddev.width - 160) >> 1, 100, 200,16,16, (char *)"x4:       y4:       ");
 	LCD_ShowString (  (lcddev.width - 160) >> 1, 140, 200,16,16,  (char *)str                             );					   
}

//-----------------------------------------------------------------
// void Save_Adjdata(void) 
//-----------------------------------------------------------------
//
// 函数功能：保存校准参数
// 入口参数：无
// 返回参数：无  
// 注意事项：无
// 
//-----------------------------------------------------------------
// 共用体	作为参数存储数据格式转换用
typedef struct
{
  float     xfac;
  float     yfac;
  uint16_t  xoff;
  uint16_t  yoff;
  uint8_t   touchtype;
  uint8_t   Aj;
} TP_TypeDef;

// 共用体	作为参数存储数据格式转换用
union sfu {
  TP_TypeDef  ff;
  uint8_t     sbuf[32];
};

#define SAVE_ADDR_BASE 35
void Save_AdjData(void)
{
  union sfu save_buf;
  
	// 保存校正结果   							  
  save_buf.ff.xfac = tp_dev.xfac;

  save_buf.ff.yfac = tp_dev.yfac;
  
 	//保存x偏移量
  save_buf.ff.xoff = tp_dev.xoff;
  
	//保存y偏移量
  save_buf.ff.yoff = tp_dev.yoff;
  
 	//保存触摸类型

  save_buf.ff.touchtype = tp_dev.touchtype;

  save_buf.ff.Aj = 0x5A;
	
	// 将校准的数据存储在FLASH_W25Q128的倒数第20个地址
	W25QXX_Write((u8*)save_buf.sbuf,FLASH_LEN-20,14);

}

//-----------------------------------------------------------------
// u8 Get_AdjData(void) 
//-----------------------------------------------------------------
//
// 函数功能：得到保存在片内flash里面的校准
// 入口参数：无
// 返回参数：1：成功获取数据，0获取失败，要重新校准 
// 注意事项：
//					 STM32内部FLASH存储单元定义（设基地址为FLASH_BASE_ADDR）
//					 FLASH_BASE_ADDR     ~ FLASH_BASE_ADDR + 3：x校准参数
//					 FLASH_BASE_ADDR + 4 ~ FLASH_BASE_ADDR + 7：y校准参数
//					 FLASH_BASE_ADDR + 8 ~ FLASH_BASE_ADDR + 9：x偏移量
//					 FLASH_BASE_ADDR +10 ~ FLASH_BASE_ADDR +11：y偏移量
//					 FLASH_BASE_ADDR +12：触摸类型
//					 FLASH_BASE_ADDR +14：校准与否标志
//
//-----------------------------------------------------------------
u8 Get_AdjData(void)
{					  
  union sfu read_buf;
  
  // 横屏
  if (( lcddev.dir == DISPLAY_DIR_L ) | ( lcddev.dir == DISPLAY_DIR_R ))
  {
    CMD_RDX = 0XD0;
    CMD_RDY = 0X90;	 
  }
  else   // 竖屏
  {
    CMD_RDX = 0X90;
    CMD_RDY = 0XD0;	 
  }
	// 读取标记字，看是否校准过
	// 从FLASH_W25Q128的倒数第20个地址将校准的数据读取出来
  W25QXX_Read(read_buf.sbuf,FLASH_LEN-20,14);	

 	if( read_buf.ff.Aj == 0X5A )		 // 触摸屏已经校准过  
	{    					
 		tp_dev.xfac = read_buf.ff.xfac;// 得到x校准参数
    
 		tp_dev.yfac = read_buf.ff.yfac;// 得到y校准参数
		
		tp_dev.xoff = read_buf.ff.xoff;// 得到x偏移量	
    
		tp_dev.yoff = read_buf.ff.yoff;// 得到y偏移量	
    
    tp_dev.touchtype = read_buf.ff.touchtype;
		return 1;	 
	}
	return 0;
}

//-----------------------------------------------------------------
// void Touch_Adjust(void)
//-----------------------------------------------------------------
//
// 函数功能：触摸屏校准代码
// 入口参数：要画点的坐标
// 返回参数：无 
// 注意事项：得到四个校准代码
//
//-----------------------------------------------------------------
void Touch_Adjust(void)
{								 
	signed short pos_temp[4][2];							// 坐标缓存值
	unsigned char  cnt = 0;	
	unsigned int  d1, d2;
	unsigned long tem1, tem2;
	double fac; 	   
	
	cnt         = 0;				
	BACK_COLOR 	= WHITE;

	POINT_COLOR = RED;						// 红色
  
	LCD_Clear ( BLACK );					// 清屏
  
	Drow_Touch_Point ( 20, lcddev.height - 20 );    // 画点1
	

	tp_dev.sta = Key_Up;					// 消除触发信号
	tp_dev.xfac    = 0;						// xfa用来标记是否校准过
	while (1)
	{
 		if ( tp_dev.sta == Key_Down )				// 按键按下
		{												  	
			if ( Read_TP_Once() )							// 得到单次按键值
			{  								   
				pos_temp[cnt][0] = tp_dev.X;
				pos_temp[cnt][1] = tp_dev.Y;
				cnt ++;
			}
			switch ( cnt )
			{
				case 1:
					LCD_Clear ( BLACK );		                  		// 清屏
            Drow_Touch_Point ( 20, 20 );		            // 画点2
					break;
				case 2:
					LCD_Clear ( BLACK );	                        // 清屏
          Drow_Touch_Point ( lcddev.width - 20, 20 );	  // 画点3
					break; 
				case 3:
					LCD_Clear (BLACK);		                        // 清屏
          Drow_Touch_Point ( lcddev.width - 20, lcddev.height - 20 );  // 画点4
					break;
				case 4:	 														            // 全部四个点已经全部得到
																												// 对边相等
					//x1-x2, y1-y2
					tem1 = abs ( pos_temp[0][0] - pos_temp[1][0] );	
					tem2 = abs ( pos_temp[0][1] - pos_temp[1][1] );		
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt ( tem1 + tem2 );                    // 得到1，2的距离
					// x3-x4,y3-y4
					tem1 = abs ( pos_temp[2][0] - pos_temp[3][0] );	
					tem2 = abs ( pos_temp[2][1] - pos_temp[3][1] );	
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt ( tem1 + tem2 );					          // 得到3，4的距离
					fac = (float)d1 / d2;
					// 不合格
					if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0)		
					{
						cnt = 0;
						LCD_Clear ( BLACK );                        // 清屏
            Drow_Touch_Point ( 20, lcddev.height - 20 );// 画点
						ADJ_INFO_SHOW ((uint8_t *)"ver fac is:");   
						// 显示数值
						BACK_COLOR 	= WHITE;
						POINT_COLOR = RED;						// 红色
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       40, pos_temp[0][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  40, pos_temp[0][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       60, pos_temp[1][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  60, pos_temp[1][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       80, pos_temp[2][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  80, pos_temp[2][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,      100, pos_temp[3][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80, 100, pos_temp[3][1],3,16);	 
						// 扩大100倍显示，显示数值，该数值必须在95~105范围内	
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 90,      140, fac * 100,3,16);				
						continue;
					}
					// x1-x3,y1-y3
					tem1 = abs ( pos_temp[0][0] - pos_temp[2][0] );	
					tem2 = abs ( pos_temp[0][1] - pos_temp[2][1] );		
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt ( tem1 + tem2 );					                      // 得到1，3的距离
					// x2-x4,y2-y4
					tem1 = abs ( pos_temp[1][0] - pos_temp[3][0] );		
					tem2 = abs ( pos_temp[1][1] - pos_temp[3][1] );		
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt ( tem1 + tem2 );					                      // 得到2，4的距离
					fac = (float)d1 / d2;
					if (fac < 0.95 || fac > 1.05)			                        // 不合格
					{
						cnt = 0;
						LCD_Clear ( BLACK );                          					// 清屏
            Drow_Touch_Point ( 20, lcddev.height - 20 );           	// 画点1
						ADJ_INFO_SHOW ( (uint8_t *)"hor fac is:" );   
						ADJ_INFO_SHOW ( (uint8_t *)"ver fac is:" );   
						// 显示数值
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       40, pos_temp[0][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  40, pos_temp[0][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       60, pos_temp[1][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  60, pos_temp[1][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       80, pos_temp[2][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  80, pos_temp[2][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,      100, pos_temp[3][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80, 100, pos_temp[3][1],3,16);	 
						// 扩大100倍显示，显示数值，该数值必须在95~105范围之内
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 90,      140, fac * 100,3,16);					
            // 扩大100倍显示，显示数值，该数值必须在95~105范围之内
 						continue;
					}// 正确了
								   
					// 对角线相等
					// x1-x3,y1-y3
					tem1 = abs ( pos_temp[1][0] - pos_temp[2][0] );			
					tem2 = abs ( pos_temp[1][1] - pos_temp[2][1] );			
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt ( tem1 + tem2 );								                // 得到1，3的距离
					// x2-x4,y2-y4
					tem1 = abs(pos_temp[0][0]-pos_temp[3][0]);			
					tem2 = abs(pos_temp[0][1]-pos_temp[3][1]);			
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt ( tem1 + tem2 );								                // 得到2，4的距离
					fac = (float)d1 / d2;
					if ( fac < 0.95 || fac > 1.05 )					                	// 不合格
					{
						cnt=0;
						LCD_Clear ( BLACK );                          					// 清屏
            Drow_Touch_Point ( 20, lcddev.height - 20 );           	// 画点1
						ADJ_INFO_SHOW ( (uint8_t *)"dia fac is:" );  
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       40, pos_temp[0][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  40, pos_temp[0][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       60, pos_temp[1][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  60, pos_temp[1][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       80, pos_temp[2][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  80, pos_temp[2][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,      100, pos_temp[3][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80, 100, pos_temp[3][1],3,16);	 
						// 扩大100倍显示，显示数值，该数值必须在95~105范围之内	
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 90,      140, fac * 100,3,16);					
            // 扩大100倍显示，显示数值，该数值必须在95~105范围之内
						continue;
					} 
					// 正确了
					// 对角线相等
					// 得到xfac		
					tp_dev.xfac = (float)( lcddev.height - 40 ) / ( pos_temp[1][0] - pos_temp[0][0] );			
					// 得到xoff	     
					tp_dev.xoff = ( lcddev.height - tp_dev.xfac * ( pos_temp[1][0] + pos_temp[0][0]) ) / 2;	
				  // 得到yfac
					tp_dev.yfac = (float)( lcddev.width - 40 ) / ( pos_temp[0][1] - pos_temp[2][1] );					   
					// 得到yoff   
					tp_dev.yoff = ( lcddev.width - tp_dev.yfac * ( pos_temp[0][1] + pos_temp[2][1] ) ) / 2;	

					POINT_COLOR = BLUE;
					LCD_Clear ( BLACK );                            // 清屏
					LCD_ShowString((lcddev.width >> 1) - 92, (lcddev.height >> 1) - 8, 200,16,16, (char *)"Touch Screen Adjust OK!");		
					delay_ms(250);
					delay_ms(250);
					delay_ms(250);
					delay_ms(250);
					LCD_Clear ( BLACK );    // 清屏 
          Save_AdjData();         // 保存屏幕校准值
					return;                 // 校正完成			 
			}
		}
	} 
}	

//-----------------------------------------------------------------
// u8 TP_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: 触摸屏初始化  
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 触摸屏初始化要在LCD初始化之后
//
//-----------------------------------------------------------------
void TP_Init(void)
{	
	if(lcddev.id == 0x5510)
	{
		tp_dev.touchtype=0;
		FT5206_Init();										// FT5206初始化
		tp_dev.scan=FT5206_Scan;					// 扫描函数指向FT5206触摸屏扫描		
		tp_dev.touchtype|=0X80;						// 电容屏 
		tp_dev.touchtype|=lcddev.dir&0X07;// 显示方向		
	}
	else
	{
		tp_dev.touchtype|=0X00;						// 电阻屏 
		tp_dev.touchtype|=lcddev.dir&0X07;// 显示方向
		
		TP_GPIO_Init();
		SPI_GPIO_Init();
		
//		if(Get_AdjData())									// 获得校准值
//			return ;
//		else															// 没有校准，执行校准操作
//		{
//			LCD_Clear(BLACK);								// 清屏
//				Touch_Adjust();									// 屏幕校准
//			//Save_AdjData();									// 保存屏幕校准值
//		}
	}
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
