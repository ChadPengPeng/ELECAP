//-----------------------------------------------------------------
// ��������:
//     FT5206 ��������
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

//-----------------------------------------------------------------
// ͷ�ļ�����
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
// ��������: ��FT5206д��һ������
// ��ڲ���: u16 reg����ʼ�Ĵ�����ַ
//					 u8 *buf�����ݻ�������
//					 u8 len��д���ݳ���
// �� �� ֵ: 0,�ɹ�;1,ʧ��.
// ע������: ��
//
//-----------------------------------------------------------------
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	// ����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   // ���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    CT_IIC_Send_Byte(buf[i]);  	// ������
		ret=CT_IIC_Wait_Ack();
		if(ret)
			break;  
	}
  CT_IIC_Stop();								// ����һ��ֹͣ����	    
	return ret; 
}
	
//-----------------------------------------------------------------
// void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
//-----------------------------------------------------------------
// 
// ��������: ��FT5206����һ������
// ��ڲ���: u16 reg����ʼ�Ĵ�����ַ
//					 u8 *buf�����ݻ�������
//					 u8 len��д���ݳ���
// �� �� ֵ: 0,�ɹ�;1,ʧ��.
// ע������: ��
//
//-----------------------------------------------------------------
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	// ����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	// ���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	// ���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); // ������	  
	} 
  CT_IIC_Stop();									// ����һ��ֹͣ����     
} 

//-----------------------------------------------------------------
// u8 FT5206_Init(void)
//-----------------------------------------------------------------
// 
// ��������: ��ʼ��FT5206������
// ��ڲ���: ��
// �� �� ֵ: 0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
// ע������: ��
//
//-----------------------------------------------------------------
u8 FT5206_Init(void)
{
	u8 temp[2]; 
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			// ����GPIOAʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			// ����GPIODʱ��
							
	GPIO_Initure.Pin=GPIO_PIN_8;            // PA8�ж�����
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      // ����
	GPIO_Initure.Pull=GPIO_PULLUP;          // ����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; // ����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);     // ��ʼ��
					
	GPIO_Initure.Pin=GPIO_PIN_3;            // PD3��λ
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // �������
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     // ��ʼ��
   
	CT_IIC_Init();      // ��ʼ����������I2C����  

	FT_RST_MCU_L;				// ��λ
	delay_ms(20);
	FT_RST_MCU_H;				// �ͷŸ�λ

	delay_ms(50);  	
	temp[0]=0;
	FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);	// ������������ģʽ 
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);		// ��ѯģʽ 
	temp[0]=80;														// ������Чֵ��80��ԽСԽ����	
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);// ���ô�����Чֵ
	temp[0]=12;														// �������ڣ�����С��12�����14
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	// ��ȡ�汾�ţ��ο�ֵ��0x3003
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)// �汾:0X3003/0X0001/0X0002
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
// ��������: ɨ�败����(���ò�ѯ��ʽ)
// ��ڲ���: u8 mode������ɨ��.
// �� �� ֵ: 0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
// ע������: ��
//
//-----------------------------------------------------------------
u8 FT5206_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);// ��ȡ�������״̬  
	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFF<<(mode&0XF);// ����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
		tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
		for(i=0;i<5;i++)
		{
			if(tp_dev.sta&(1<<i))		// ������Ч
			{
				FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	// ��ȡXY����ֵ 
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
					tp_dev.x[i]=tp_dev.y[i]=0;// ������contact�¼�������Ϊ��Ч
			}			
		} 
		res=1;
		if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	// ���������ݶ���0,����Դ˴�����
	}
	if((mode&0X1F)==0)// �޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	// ֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	// ��ǰ����ɿ�
		}
		else						// ֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	// �������Ч���	
		}	 
	}
	return res;
}
 
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
