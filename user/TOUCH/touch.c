//-----------------------------------------------------------------
// ��������:
//     ��������������
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

//-----------------------------------------------------------------
// ͷ�ļ�����
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

u32 FLASH_LEN=16*1024*1024;	// FLASH ��СΪ16M�ֽ�
//-----------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------
u16 Time = 1;		// ��ʱ����
u8 higth = 1;

#define ts_delay   delay_us

//-----------------------------------------------------------------
// void TP_GPIO_Init(void)
//-----------------------------------------------------------------
//
// �������ܣ��������������ų�ʼ��
// ��ڲ�������
// ���ز�������
// ע�����
//
//-----------------------------------------------------------------
void TP_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();						// ����GPIOAʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();						// ����GPIODʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_3;            // PD3->TP_CS
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // �������
	GPIO_Initure.Pull=GPIO_PULLUP;          // ����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; // ����
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     // ��ʼ��
		
	GPIO_Initure.Pin=GPIO_PIN_6;            // PD6->SD_CS
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // �������
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     // ��ʼ��
	
	GPIO_Initure.Pin=GPIO_PIN_8; 						// PA8�ж�����
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING; // �����ش���
	GPIO_Initure.Pull=GPIO_PULLUP;					// ��������
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);			// ��ʼ��
		
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);   // ��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);         // ʹ���ж���9-5
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);// TP_CS�ø�
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);// ��ֹSD��
}

//-----------------------------------------------------------------
// void EXTI9_5_IRQHandler(void)
//-----------------------------------------------------------------
//
// ��������: �ж���9-5�жϷ������������жϴ����ú���
// ��ڲ���: ��
// ���ز���: ��
// ע������: ��
//-----------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);// �����жϴ����ú���
}

//-----------------------------------------------------------------
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//-----------------------------------------------------------------
//
// ��������: �жϷ����������Ҫ��������
// ��ڲ���: uint16_t GPIO_Pin���ж�����
// ���ز���: ��
// ע������: ��HAL�������е��ⲿ�жϷ�����������ô˺���
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
// ������
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void SPI_GPIO_Init(void)
//-----------------------------------------------------------------
//
// �������ܣ�SPI���ų�ʼ��
// ��ڲ�������
// ���ز�������
// ע�������
//
//-----------------------------------------------------------------
void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();						// ����GPIOBʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_3;            // PB3->CLK
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // ���
	GPIO_Initure.Pull=GPIO_PULLUP;          // ����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;// ����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     // ��ʼ��
	
	GPIO_Initure.Pin=GPIO_PIN_5;            // PB5->MOSI
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     // ��ʼ��
	
	GPIO_Initure.Pin=GPIO_PIN_4;            // PB4->MISO
	GPIO_Initure.Mode=GPIO_MODE_INPUT;  		// ����
	GPIO_Initure.Pull=GPIO_PULLUP;          // ����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;// ����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);     // ��ʼ��
}

//-----------------------------------------------------------------
// void TP_SPI_Write_Byte(u8 Digital)
//-----------------------------------------------------------------
//
// �������ܣ���IO��ģ��SPI��дһ���ֽ�
// ��ڲ�������
// ���ز�������
// ע��������ݷ���˳�򣺸�λ��ǰ����λ�ں�
//		(1) ֻ����IOģ��SPIʱ��ķ������õ���Щ����
// 		(2) ע��SPI�������ʹӻ���������MOSI��������Ч����ʱ��������
//				��Ч�����������մӻ���������MISO����ʱ���½���������Ч��
//				SPI����ȫ˫��ͨ�ţ������ڱ����������ȷ���������ӻ�Ȼ��
//				�Ӵӻ����ͻ������ݸ����������Ⱥ��ϵ�ڵ��ô˺���֮ǰ����
//				��Ƭѡ�ź���Ч������д���ݲ��ɹ�
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
// �������ܣ���IO��ģ��SPI�Ķ�һ���ֽ�
// ��ڲ�������
// ���ز�������
// ע�����
//		(1) ֻ����IOģ��SPIʱ��ķ������õ���Щ����
// 		(2) ע��SPI�������ʹӻ���������MOSI��������Ч����ʱ��������
//				��Ч�����������մӻ���������MISO����ʱ���½���������Ч��
//				SPI����ȫ˫��ͨ�ţ������ڱ����������ȷ���������ӻ�Ȼ��
//				�Ӵӻ����ͻ������ݸ����������Ⱥ��ϵ�ڵ��ô˺���֮ǰ����
//				��Ƭѡ�ź���Ч������д���ݲ��ɹ�
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
// �������ܣ�SPI��дһ���ֽ�
// ��ڲ�������
// ���ز�������
// ע�������
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
// �ײ��дXPT2046��SPIʱ�������
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// u16 TP_Read_AD(u8 CMD) 
//-----------------------------------------------------------------
//
// �������ܣ���TP����ADCֵ��SPI�����ݣ�
// ��ڲ�����u8 CMD	����
// ���ز�����ADCֵ
// ע������ײ��д����
//
//-----------------------------------------------------------------
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0;
	u16  Num   = 0; 
 	
	T_CLK_L;											// ������ʱ��
	T_CS_L; 											// ѡ��XPT2046 
	TP_SPI_Write_Byte(CMD);				// ����������
	ts_delay(6);									// XPT2046��ת��ʱ���Ϊ6us
  for (count = 0; count < 16; count++)
  {
    Num <<= 1;
    T_CLK_H;
    ts_delay(Time);
    T_CLK_L; // �½�����Ч
    ts_delay(Time);
    if (T_MISO)
      Num++;
  }
  Num >>= 3; // ֻ�и�12λ��Ч
  T_CS_H;    // �ͷ�XPT2046

  return (Num);
}

//-----------------------------------------------------------------
// u16 TP_Read_XY ( u8 xy )
//-----------------------------------------------------------------
//
// �������ܣ���ȡһ������ֵ
// ��ڲ�����xy
// ���ز���������ֵ
// ע�����������ȡREAD TIMES�����ݣ�����Щ������������
//					 Ȼ��ȥ����ͺ����LOST_VAL
//
//-----------------------------------------------------------------
#define READ_TIMES 4 		// ��ȡ����
#define LOST_VAL 1  		// ����ֵ
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
	for ( i = 0; i < READ_TIMES - 1; i ++ )			// ����
	{
		for ( j = i + 1; j < READ_TIMES; j ++ )
		{
			if ( buf[i] > buf[j] )					      	// ����
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
// �������ܣ����˲��������ȡ
// ��ڲ������������ֵ��ָ��
// ���ز��������ݶ�ȡ״̬��0����ȡʧ�ܣ�1����ȡ�ɹ�
// ע�������Сֵ��������50
//
//-----------------------------------------------------------------
u8 Read_TP ( u16 *x, u16 *y )
{
	u16 xtemp, ytemp;
				 	 		  
	xtemp = TP_Read_XY ( CMD_RDX );
	ytemp = TP_Read_XY ( CMD_RDY );	  												   
	if ( xtemp < 50 || ytemp < 50 )	
		return 0;		// ����ʧ��
	*x = xtemp;		// ��������ֵ
	*y = ytemp;		// ��������ֵ

	return 1;			// ����ɹ�
}

//-----------------------------------------------------------------
// u8 Read_TP_2 ( u16 *x, u16 *y ) 
//-----------------------------------------------------------------
//
// �������ܣ�2�ζ�ȡTP��ֵ
// ��ڲ������������ֵ��ָ��
// ���ز��������ݶ�ȡ״̬��0����ȡʧ�ܣ�1����ȡ�ɹ�
// ע�����������ȡ2����Ч��ADֵ���������ε�ƫ��ܳ���
//					 50����������������Ϊ������ȷ������������
//					 �ú����ܴ�����׼ȷ�ȡ�
//
//-----------------------------------------------------------------
#define ERR_RANGE 50							// ��Χ
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
	if ( ((( x2 <= x1 ) & ( x1 < ( x2 + ERR_RANGE ))) | (( x1 <= x2 ) & ( x2 < ( x1 + ERR_RANGE ))))	& 		//ǰ�����β����ڡ�50��
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
// �������ܣ���ȡһ������ֵ
// ��ڲ�������
// ���ز��������ݶ�ȡ״̬��0����ȡʧ�ܣ�1����ȡ�ɹ�
// ע�����������ȡһ�Σ�ֱ��T_PEN�ɿ��ŷ���
//
//-----------------------------------------------------------------
u8 Read_TP_Once (void)
{
	u8 t = 0;	 
	   
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);          // �ر��ж���9-5
	
	tp_dev.sta = Key_Up;
  
	Read_TP_2 ( &tp_dev.X, &tp_dev.Y );
	while (( T_PEN == 0 ) && ( t <= 250 ))
	{
		t ++;
		delay_ms ( 10 );
	}
	__HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_8);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);          // ʹ���ж���9-5
	
	if ( t >= 250 )	
	{
		return 0;									    				// ����2.5����Ϊ��Ч
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
// �������ܣ���ȡ����ֵ
// ��ڲ�������
// ���ز�������
// ע��������ݴ�������У׼����������ת����Ľ����������X0,Y0
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
        // ����任
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
// ��LCD�����йصĺ���
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void Drow_Touch_Point ( u16 x, u16 y )
//-----------------------------------------------------------------
//
// �������ܣ���һ������У׼��λ��ʶ
// ��ڲ�����Ҫ���������
// ���ز������� 
// ע���������У׼��
//
//-----------------------------------------------------------------
void Drow_Touch_Point ( u16 x, u16 y )
{
	POINT_COLOR=RED;
	LCD_DrawLine(x-12, y, x+12, y);		// ��һ������
	LCD_DrawLine(x, y-12, x, y+12);		// ��һ������
	
	LCD_DrawRectangle(x-8,y-8,x+8,y+8);		// ��һ������
}

//-----------------------------------------------------------------
// void Draw_Big_Point_1(u8 x, u16 y)
//-----------------------------------------------------------------
//
// �������ܣ���һ��������
// ��ڲ�����Ҫ���������
// ���ز�������
// ע���������У׼��
//
//-----------------------------------------------------------------
void Draw_Big_Point_1(u16 x, u16 y)
{	
	POINT_COLOR=RED;
	LCD_DrawPoint ( x, y);		// ���ĵ�
}

//-----------------------------------------------------------------
// ʹ���ڲ�FLASH���洢У׼��
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void ADJ_INFO_SHOW(unsigned char *str) 
//-----------------------------------------------------------------
//
// �������ܣ���ʾ��ʾ��Ϣ
// ��ڲ�����Ҫ��ʾ���ַ���
// ���ز�������  
// ע�������
// 
//-----------------------------------------------------------------
void ADJ_INFO_SHOW(unsigned char *str)
{
	BACK_COLOR 	= WHITE;
	POINT_COLOR = BLACK;						// ��ɫ
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
// �������ܣ�����У׼����
// ��ڲ�������
// ���ز�������  
// ע�������
// 
//-----------------------------------------------------------------
// ������	��Ϊ�����洢���ݸ�ʽת����
typedef struct
{
  float     xfac;
  float     yfac;
  uint16_t  xoff;
  uint16_t  yoff;
  uint8_t   touchtype;
  uint8_t   Aj;
} TP_TypeDef;

// ������	��Ϊ�����洢���ݸ�ʽת����
union sfu {
  TP_TypeDef  ff;
  uint8_t     sbuf[32];
};

#define SAVE_ADDR_BASE 35
void Save_AdjData(void)
{
  union sfu save_buf;
  
	// ����У�����   							  
  save_buf.ff.xfac = tp_dev.xfac;

  save_buf.ff.yfac = tp_dev.yfac;
  
 	//����xƫ����
  save_buf.ff.xoff = tp_dev.xoff;
  
	//����yƫ����
  save_buf.ff.yoff = tp_dev.yoff;
  
 	//���津������

  save_buf.ff.touchtype = tp_dev.touchtype;

  save_buf.ff.Aj = 0x5A;
	
	// ��У׼�����ݴ洢��FLASH_W25Q128�ĵ�����20����ַ
	W25QXX_Write((u8*)save_buf.sbuf,FLASH_LEN-20,14);

}

//-----------------------------------------------------------------
// u8 Get_AdjData(void) 
//-----------------------------------------------------------------
//
// �������ܣ��õ�������Ƭ��flash�����У׼
// ��ڲ�������
// ���ز�����1���ɹ���ȡ���ݣ�0��ȡʧ�ܣ�Ҫ����У׼ 
// ע�����
//					 STM32�ڲ�FLASH�洢��Ԫ���壨�����ַΪFLASH_BASE_ADDR��
//					 FLASH_BASE_ADDR     ~ FLASH_BASE_ADDR + 3��xУ׼����
//					 FLASH_BASE_ADDR + 4 ~ FLASH_BASE_ADDR + 7��yУ׼����
//					 FLASH_BASE_ADDR + 8 ~ FLASH_BASE_ADDR + 9��xƫ����
//					 FLASH_BASE_ADDR +10 ~ FLASH_BASE_ADDR +11��yƫ����
//					 FLASH_BASE_ADDR +12����������
//					 FLASH_BASE_ADDR +14��У׼����־
//
//-----------------------------------------------------------------
u8 Get_AdjData(void)
{					  
  union sfu read_buf;
  
  // ����
  if (( lcddev.dir == DISPLAY_DIR_L ) | ( lcddev.dir == DISPLAY_DIR_R ))
  {
    CMD_RDX = 0XD0;
    CMD_RDY = 0X90;	 
  }
  else   // ����
  {
    CMD_RDX = 0X90;
    CMD_RDY = 0XD0;	 
  }
	// ��ȡ����֣����Ƿ�У׼��
	// ��FLASH_W25Q128�ĵ�����20����ַ��У׼�����ݶ�ȡ����
  W25QXX_Read(read_buf.sbuf,FLASH_LEN-20,14);	

 	if( read_buf.ff.Aj == 0X5A )		 // �������Ѿ�У׼��  
	{    					
 		tp_dev.xfac = read_buf.ff.xfac;// �õ�xУ׼����
    
 		tp_dev.yfac = read_buf.ff.yfac;// �õ�yУ׼����
		
		tp_dev.xoff = read_buf.ff.xoff;// �õ�xƫ����	
    
		tp_dev.yoff = read_buf.ff.yoff;// �õ�yƫ����	
    
    tp_dev.touchtype = read_buf.ff.touchtype;
		return 1;	 
	}
	return 0;
}

//-----------------------------------------------------------------
// void Touch_Adjust(void)
//-----------------------------------------------------------------
//
// �������ܣ�������У׼����
// ��ڲ�����Ҫ���������
// ���ز������� 
// ע������õ��ĸ�У׼����
//
//-----------------------------------------------------------------
void Touch_Adjust(void)
{								 
	signed short pos_temp[4][2];							// ���껺��ֵ
	unsigned char  cnt = 0;	
	unsigned int  d1, d2;
	unsigned long tem1, tem2;
	double fac; 	   
	
	cnt         = 0;				
	BACK_COLOR 	= WHITE;

	POINT_COLOR = RED;						// ��ɫ
  
	LCD_Clear ( BLACK );					// ����
  
	Drow_Touch_Point ( 20, lcddev.height - 20 );    // ����1
	

	tp_dev.sta = Key_Up;					// ���������ź�
	tp_dev.xfac    = 0;						// xfa��������Ƿ�У׼��
	while (1)
	{
 		if ( tp_dev.sta == Key_Down )				// ��������
		{												  	
			if ( Read_TP_Once() )							// �õ����ΰ���ֵ
			{  								   
				pos_temp[cnt][0] = tp_dev.X;
				pos_temp[cnt][1] = tp_dev.Y;
				cnt ++;
			}
			switch ( cnt )
			{
				case 1:
					LCD_Clear ( BLACK );		                  		// ����
            Drow_Touch_Point ( 20, 20 );		            // ����2
					break;
				case 2:
					LCD_Clear ( BLACK );	                        // ����
          Drow_Touch_Point ( lcddev.width - 20, 20 );	  // ����3
					break; 
				case 3:
					LCD_Clear (BLACK);		                        // ����
          Drow_Touch_Point ( lcddev.width - 20, lcddev.height - 20 );  // ����4
					break;
				case 4:	 														            // ȫ���ĸ����Ѿ�ȫ���õ�
																												// �Ա����
					//x1-x2, y1-y2
					tem1 = abs ( pos_temp[0][0] - pos_temp[1][0] );	
					tem2 = abs ( pos_temp[0][1] - pos_temp[1][1] );		
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt ( tem1 + tem2 );                    // �õ�1��2�ľ���
					// x3-x4,y3-y4
					tem1 = abs ( pos_temp[2][0] - pos_temp[3][0] );	
					tem2 = abs ( pos_temp[2][1] - pos_temp[3][1] );	
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt ( tem1 + tem2 );					          // �õ�3��4�ľ���
					fac = (float)d1 / d2;
					// ���ϸ�
					if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0)		
					{
						cnt = 0;
						LCD_Clear ( BLACK );                        // ����
            Drow_Touch_Point ( 20, lcddev.height - 20 );// ����
						ADJ_INFO_SHOW ((uint8_t *)"ver fac is:");   
						// ��ʾ��ֵ
						BACK_COLOR 	= WHITE;
						POINT_COLOR = RED;						// ��ɫ
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       40, pos_temp[0][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  40, pos_temp[0][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       60, pos_temp[1][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  60, pos_temp[1][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       80, pos_temp[2][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  80, pos_temp[2][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,      100, pos_temp[3][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80, 100, pos_temp[3][1],3,16);	 
						// ����100����ʾ����ʾ��ֵ������ֵ������95~105��Χ��	
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 90,      140, fac * 100,3,16);				
						continue;
					}
					// x1-x3,y1-y3
					tem1 = abs ( pos_temp[0][0] - pos_temp[2][0] );	
					tem2 = abs ( pos_temp[0][1] - pos_temp[2][1] );		
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt ( tem1 + tem2 );					                      // �õ�1��3�ľ���
					// x2-x4,y2-y4
					tem1 = abs ( pos_temp[1][0] - pos_temp[3][0] );		
					tem2 = abs ( pos_temp[1][1] - pos_temp[3][1] );		
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt ( tem1 + tem2 );					                      // �õ�2��4�ľ���
					fac = (float)d1 / d2;
					if (fac < 0.95 || fac > 1.05)			                        // ���ϸ�
					{
						cnt = 0;
						LCD_Clear ( BLACK );                          					// ����
            Drow_Touch_Point ( 20, lcddev.height - 20 );           	// ����1
						ADJ_INFO_SHOW ( (uint8_t *)"hor fac is:" );   
						ADJ_INFO_SHOW ( (uint8_t *)"ver fac is:" );   
						// ��ʾ��ֵ
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       40, pos_temp[0][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  40, pos_temp[0][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       60, pos_temp[1][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  60, pos_temp[1][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       80, pos_temp[2][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  80, pos_temp[2][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,      100, pos_temp[3][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80, 100, pos_temp[3][1],3,16);	 
						// ����100����ʾ����ʾ��ֵ������ֵ������95~105��Χ֮��
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 90,      140, fac * 100,3,16);					
            // ����100����ʾ����ʾ��ֵ������ֵ������95~105��Χ֮��
 						continue;
					}// ��ȷ��
								   
					// �Խ������
					// x1-x3,y1-y3
					tem1 = abs ( pos_temp[1][0] - pos_temp[2][0] );			
					tem2 = abs ( pos_temp[1][1] - pos_temp[2][1] );			
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt ( tem1 + tem2 );								                // �õ�1��3�ľ���
					// x2-x4,y2-y4
					tem1 = abs(pos_temp[0][0]-pos_temp[3][0]);			
					tem2 = abs(pos_temp[0][1]-pos_temp[3][1]);			
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt ( tem1 + tem2 );								                // �õ�2��4�ľ���
					fac = (float)d1 / d2;
					if ( fac < 0.95 || fac > 1.05 )					                	// ���ϸ�
					{
						cnt=0;
						LCD_Clear ( BLACK );                          					// ����
            Drow_Touch_Point ( 20, lcddev.height - 20 );           	// ����1
						ADJ_INFO_SHOW ( (uint8_t *)"dia fac is:" );  
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       40, pos_temp[0][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  40, pos_temp[0][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       60, pos_temp[1][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  60, pos_temp[1][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,       80, pos_temp[2][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80,  80, pos_temp[2][1],3,16);	 
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24,      100, pos_temp[3][0],3,16);		
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 24 + 80, 100, pos_temp[3][1],3,16);	 
						// ����100����ʾ����ʾ��ֵ������ֵ������95~105��Χ֮��	
						LCD_ShowNum ( (( lcddev.width - 160 ) >> 1 ) + 90,      140, fac * 100,3,16);					
            // ����100����ʾ����ʾ��ֵ������ֵ������95~105��Χ֮��
						continue;
					} 
					// ��ȷ��
					// �Խ������
					// �õ�xfac		
					tp_dev.xfac = (float)( lcddev.height - 40 ) / ( pos_temp[1][0] - pos_temp[0][0] );			
					// �õ�xoff	     
					tp_dev.xoff = ( lcddev.height - tp_dev.xfac * ( pos_temp[1][0] + pos_temp[0][0]) ) / 2;	
				  // �õ�yfac
					tp_dev.yfac = (float)( lcddev.width - 40 ) / ( pos_temp[0][1] - pos_temp[2][1] );					   
					// �õ�yoff   
					tp_dev.yoff = ( lcddev.width - tp_dev.yfac * ( pos_temp[0][1] + pos_temp[2][1] ) ) / 2;	

					POINT_COLOR = BLUE;
					LCD_Clear ( BLACK );                            // ����
					LCD_ShowString((lcddev.width >> 1) - 92, (lcddev.height >> 1) - 8, 200,16,16, (char *)"Touch Screen Adjust OK!");		
					delay_ms(250);
					delay_ms(250);
					delay_ms(250);
					delay_ms(250);
					LCD_Clear ( BLACK );    // ���� 
          Save_AdjData();         // ������ĻУ׼ֵ
					return;                 // У�����			 
			}
		}
	} 
}	

//-----------------------------------------------------------------
// u8 TP_Init(void)
//-----------------------------------------------------------------
// 
// ��������: ��������ʼ��  
// ��ڲ���: �� 
// �� �� ֵ: ��
// ע������: ��������ʼ��Ҫ��LCD��ʼ��֮��
//
//-----------------------------------------------------------------
void TP_Init(void)
{	
	if(lcddev.id == 0x5510)
	{
		tp_dev.touchtype=0;
		FT5206_Init();										// FT5206��ʼ��
		tp_dev.scan=FT5206_Scan;					// ɨ�躯��ָ��FT5206������ɨ��		
		tp_dev.touchtype|=0X80;						// ������ 
		tp_dev.touchtype|=lcddev.dir&0X07;// ��ʾ����		
	}
	else
	{
		tp_dev.touchtype|=0X00;						// ������ 
		tp_dev.touchtype|=lcddev.dir&0X07;// ��ʾ����
		
		TP_GPIO_Init();
		SPI_GPIO_Init();
		
//		if(Get_AdjData())									// ���У׼ֵ
//			return ;
//		else															// û��У׼��ִ��У׼����
//		{
//			LCD_Clear(BLACK);								// ����
//				Touch_Adjust();									// ��ĻУ׼
//			//Save_AdjData();									// ������ĻУ׼ֵ
//		}
	}
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
