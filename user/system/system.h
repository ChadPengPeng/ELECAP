//-----------------------------------------------------------------
// ��������:
//    	����ϵͳʱ�Ӻ��������Ͷ���ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//   - V1.0: (2020-11-11)����ϵͳʱ�Ӻ�MPU�ڴ汣��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2
// ˵    ��: 
//
//-----------------------------------------------------------------
#ifndef _SYSTEM_H
#define _SYSTEM_H
#include "stm32h7xx.h"
//-----------------------------------------------------------------
// ����һЩ���õ��������Ͷ̹ؼ��� 
//-----------------------------------------------------------------
typedef int32_t  				s32;
typedef int16_t 				s16;
typedef int8_t  				s8;

typedef const int32_t 	sc32;  
typedef const int16_t 	sc16;  
typedef const int8_t 		sc8;  

typedef __IO int32_t  	vs32;
typedef __IO int16_t  	vs16;
typedef __IO int8_t   	vs8;

typedef __I int32_t 		vsc32;  
typedef __I int16_t 		vsc16; 
typedef __I int8_t 			vsc8;   

typedef uint32_t  			u32;
typedef uint16_t 				u16;
typedef uint8_t  				u8;

typedef const uint32_t 	uc32;  
typedef const uint16_t 	uc16;  
typedef const uint8_t 	uc8; 

typedef __IO uint32_t  	vu32;
typedef __IO uint16_t 	vu16;
typedef __IO uint8_t  	vu8;

typedef __I uint32_t 		vuc32;  
typedef __I uint16_t 		vuc16; 
typedef __I uint8_t 		vuc8;  

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void SystemClock_Config(void);// ʱ��ϵͳ����
extern void CPU_CACHE_Enable(void);
extern void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B);
extern void MPU_Memory_Protection(void);
#endif

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
