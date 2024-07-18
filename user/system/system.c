//-----------------------------------------------------------------
// ��������:
//     ����ϵͳʱ�Ӻ�MPU�ڴ汣��
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)����ϵͳʱ�Ӻ�MPU�ڴ汣��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "system.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void SystemClock_Config(void)
//-----------------------------------------------------------------
// 
// ��������: ϵͳʱ�ӳ�ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:
//            System Clock source  = PLL (HSE) 25MHz����
//	          SYSCLK(Hz)           = 400000000 (CPU Clock)
//	          HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
//	          AHB Prescaler        = 2
//	          D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
//	          D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
//	          D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
//	          D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
//	          HSE Frequency(Hz)    = 25000000
//	          PLL_M                = 5
//	          PLL_N                = 160
//	          PLL_P                = 2
//	          PLL_Q                = 4
//	          PLL_R                = 2
//	          VDD(V)               = 3.3
//	          Flash Latency(WS)    = 2
//
//-----------------------------------------------------------------
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0); // ��Ӧ���ø�������
  // ������������������õ�ѹ�������ѹ���������������Ƶ�ʹ���
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);	//���õ�ѹ�������ѹ����1

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
  {
    
  }
  // ��ʼ��CPU��AHB��APB����ʱ��
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;	// ʱ��ԴΪHSE
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;										// ��HSE
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;								// ��PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;				// PLLʱ��Դѡ��HSE
  RCC_OscInitStruct.PLL.PLLM = 5;															// ��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:1~63.
  RCC_OscInitStruct.PLL.PLLN = 160;														// ��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:4~512.  
  RCC_OscInitStruct.PLL.PLLP = 2;															// ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
  RCC_OscInitStruct.PLL.PLLQ = 4;															// ����ʱ�ӵķ�Ƶϵ��Q
  RCC_OscInitStruct.PLL.PLLR = 2;															// ����ʱ�ӵķ�Ƶϵ��R
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;					// PLLʱ�����뷶Χ
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;					// PLLʱ�������Χ
  RCC_OscInitStruct.PLL.PLLFRACN = 0;													// ָ��PLL1 VCO�ĳ�����С�����֡���Ӧ����0��8191֮���ֵ
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)				// ��ʼ��
  {
    while(1) 
		{ 
			; 
		}
  }
  
	// ѡ��PLLCLK��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;	// ��Ϊϵͳʱ�ӵ�ʱ��Դ
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;				// ϵͳʱ�ӷ�Ƶ��
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;					// AHBʱ�ӣ�HCLK����Ƶ������ʱ������ϵͳʱ�ӣ�SYSCLK��
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;					// APB3ʱ�ӣ�D1PCLK1����Ƶ������ʱ��Դ��AHBʱ�ӣ�HCLK��
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;					// APB1ʱ�ӣ�PCLK1����Ƶ������ʱ��Դ��AHBʱ�ӣ�HCLK��
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;					// APB2ʱ�ӣ�PCLK2����Ƶ������ʱ��Դ��AHBʱ�ӣ�HCLK��
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;					// APB4ʱ�ӣ�D3PCLK1����Ƶ������ʱ��Դ��AHBʱ�ӣ�HCLK��

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)	
  {
    while(1) 
		{ 
			; 
		}
  }
}

//-----------------------------------------------------------------
// void CPU_CACHE_Enable(void)
//-----------------------------------------------------------------
// 
// ��������: ����CPU����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void CPU_CACHE_Enable(void)
{
  SCB_EnableICache();	// ʹ��ָ���
  SCB_EnableDCache();	// ʹ�����ݻ���
	SCB->CACR|=1<<2;		// ����D-CacheΪWrite Through����֤���ݵ�һ����
}

//-----------------------------------------------------------------
// void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
//-----------------------------------------------------------------
// 
// ��������: ����MPU
// ��ڲ���: u32 Number�����ñ�������
//					 u32 BASEADDR�����û���ַ
//					 u32 SIZE�� ���ñ�������Ĵ�С
//					 u8 AP�����÷���Ȩ��
//					 u8 DE���Ƿ�����ָ�����
//					 u8 S���Ƿ���
//					 u8 C���Ƿ񻺴�
//					 u8 B���Ƿ񻺳�
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
{
	MPU_Region_InitTypeDef MPU_Initure;
	
	HAL_MPU_Disable(); 											// ���� MPU ֮ǰ�ȹر� MPU,��������Ժ���ʹ�� MPU
	MPU_Initure.Enable=MPU_REGION_ENABLE; 	// ʹ�ܸñ�������
	MPU_Initure.Number=Number; 							// ���ñ�������
	MPU_Initure.BaseAddress=BASEADDR; 			// ���û�ַ
	MPU_Initure.Size=SIZE; 									// ���ñ��������С
	MPU_Initure.SubRegionDisable=0X00; 			// ��ֹ������
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;// ����������չ��Ϊ level0
	MPU_Initure.AccessPermission=AP; 				// ���÷���Ȩ��,
	MPU_Initure.DisableExec=DE; 						// �Ƿ�����ָ�����(�����ȡָ��)
	MPU_Initure.IsShareable=S; 							// �Ƿ���
	MPU_Initure.IsCacheable=C; 							// �Ƿ񻺴�
	MPU_Initure.IsBufferable=B; 						// �Ƿ񻺳�
	HAL_MPU_ConfigRegion(&MPU_Initure); 		// ���� MPU
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT); // ���� MPU
}

//-----------------------------------------------------------------
// void MPU_Memory_Protection(void)
//-----------------------------------------------------------------
// 
// ��������: ������Ҫ�ı�������AXI SRAM��FMC
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void MPU_Memory_Protection(void)
{
	// ����AXI SRAM����512KB�ֽڣ�ȫ���ʣ�����ָ����ʣ���ֹ���������棬������
	MPU_Set_Protection(
		MPU_REGION_NUMBER0,
		0x24000000,						
		MPU_REGION_SIZE_512KB,
		MPU_REGION_FULL_ACCESS,
		MPU_INSTRUCTION_ACCESS_ENABLE,
		MPU_ACCESS_NOT_SHAREABLE,
		MPU_ACCESS_CACHEABLE,
		MPU_ACCESS_BUFFERABLE
	);
	
	// ����FMC����64MB�ֽڣ�ȫ���ʣ�����ָ����ʣ���ֹ������ֹ���棬������
	MPU_Set_Protection(
		MPU_REGION_NUMBER1,
		0x60000000,				
		MPU_REGION_SIZE_64MB,
		MPU_REGION_FULL_ACCESS,
		MPU_INSTRUCTION_ACCESS_ENABLE,
		MPU_ACCESS_NOT_SHAREABLE,
		MPU_ACCESS_NOT_CACHEABLE,
		MPU_ACCESS_BUFFERABLE
	);
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
