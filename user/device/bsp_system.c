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
#include "bsp_system.h"
//-----------------------------------------------------------------

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
void MPU_Set_Protection(u32 Number, u32 BASEADDR, u32 SIZE, u8 AP, u8 DE, u8 S, u8 C, u8 B)
{
    MPU_Region_InitTypeDef MPU_Initure;

    HAL_MPU_Disable();                         // ���� MPU ֮ǰ�ȹر� MPU,��������Ժ���ʹ�� MPU
    MPU_Initure.Enable = MPU_REGION_ENABLE;    // ʹ�ܸñ�������
    MPU_Initure.Number = Number;               // ���ñ�������
    MPU_Initure.BaseAddress = BASEADDR;        // ���û�ַ
    MPU_Initure.Size = SIZE;                   // ���ñ��������С
    MPU_Initure.SubRegionDisable = 0X00;       // ��ֹ������
    MPU_Initure.TypeExtField = MPU_TEX_LEVEL0; // ����������չ��Ϊ level0
    MPU_Initure.AccessPermission = AP;         // ���÷���Ȩ��,
    MPU_Initure.DisableExec = DE;              // �Ƿ�����ָ�����(�����ȡָ��)
    MPU_Initure.IsShareable = S;               // �Ƿ���
    MPU_Initure.IsCacheable = C;               // �Ƿ񻺴�
    MPU_Initure.IsBufferable = B;              // �Ƿ񻺳�
    HAL_MPU_ConfigRegion(&MPU_Initure);        // ���� MPU
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);    // ���� MPU
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
        MPU_ACCESS_BUFFERABLE);

    // ����FMC����64MB�ֽڣ�ȫ���ʣ�����ָ����ʣ���ֹ������ֹ���棬������
    MPU_Set_Protection(
        MPU_REGION_NUMBER1,
        0x60000000,
        MPU_REGION_SIZE_64MB,
        MPU_REGION_FULL_ACCESS,
        MPU_INSTRUCTION_ACCESS_ENABLE,
        MPU_ACCESS_NOT_SHAREABLE,
        MPU_ACCESS_NOT_CACHEABLE,
        MPU_ACCESS_BUFFERABLE);
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
