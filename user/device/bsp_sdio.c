//-----------------------------------------------------------------
// ��������:
//     SD����������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����:
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)	SD����ʼ���Ͷ�д
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��SD��
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "bsp_sdio.h"
#include "string.h"
//-----------------------------------------------------------------
SD_HandleTypeDef SDCARD_Handler;
HAL_SD_CardInfoTypeDef sd_info;

// SD_ReadDisk/SD_WriteDisk����ר��buf,�����������������ݻ�������ַ����4�ֽڶ����ʱ��,
// ��Ҫ�õ�������,ȷ�����ݻ�������ַ��4�ֽڶ����.
__ALIGN_BEGIN u8 SDIO_DATA_BUFFER[512];

//���ͱ�־λ
__IO uint8_t TX_Flag = 0;
//���ܱ�־λ
__IO uint8_t RX_Flag = 0;

//-----------------------------------------------------------------
// u8 SD_Init(void)
//-----------------------------------------------------------------
//
// ��������: SD����ʼ��
// ��ڲ���: ��
// �� �� ֵ: 0 ��ʼ����ȷ������ֵ����ʼ������
// ע������: ��
//
//-----------------------------------------------------------------
u8 SD_Init(void)
{
    SDCARD_Handler.Instance = SDMMC1;                                              // ����SDMMC���
    SDCARD_Handler.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;                       // ��������Ч
    SDCARD_Handler.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;           // �رս���ģʽ
    SDCARD_Handler.Init.BusWide = SDMMC_BUS_WIDE_4B;                               // ���߿��Ϊ4
    SDCARD_Handler.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE; // ����Ӳ��������
    SDCARD_Handler.Init.ClockDiv = 2;                                              // ʱ�ӷ�Ƶ����Ϊ2
    if (HAL_SD_Init(&SDCARD_Handler) != HAL_OK)                                    // HAL SD ��ʼ��
        return 1;

    // ����SD����λ������Ϊ4bitģʽ
    if (HAL_SD_ConfigWideBusOperation(&SDCARD_Handler, SDCARD_Handler.Init.BusWide) != HAL_OK)
        return 2;

    return 0;
}

//-----------------------------------------------------------------
// void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
//-----------------------------------------------------------------
//
// ��������: SDIO�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
// ��ڲ���: huart:���ھ��
// ���ز���: ��
// ע������: �˺����ᱻHAL_SD_Init()����
//
//-----------------------------------------------------------------
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_SDMMC1_CLK_ENABLE(); // ����SDMMC1ʱ��

    __HAL_RCC_GPIOC_CLK_ENABLE(); // ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE(); // ʹ��GPIODʱ��

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // �������
    GPIO_InitStruct.Pull = GPIO_NOPULL;                // ��������
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // ����
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;       // SDIO1����
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);            // ��ʼ��

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // �������
    GPIO_InitStruct.Pull = GPIO_NOPULL;                // ��������
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // ����
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;       // SDIO1����
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);            // ��ʼ��
}

//-----------------------------------------------------------------
// u8 SD_Erase(u32 sector,u32 cnt)
//-----------------------------------------------------------------
//
// ��������: ����SD��
// ��ڲ���: u32 sector��������ַ
//					 u32 cnt����������
// �� �� ֵ: 0 ��ʼ����ȷ������ֵ����ʼ������
// ע������: ��
//
//-----------------------------------------------------------------
u8 SD_Erase(u32 sector, u32 cnt)
{
    HAL_SD_Erase(&SDCARD_Handler, sector, sector + cnt);
    //�ȴ��������
    if (Wait_SDCARD_Ready() != HAL_OK)
        return 1;
    else
        return 0;
}

//-----------------------------------------------------------------
// u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt)
//-----------------------------------------------------------------
//
// ��������: дSD��
// ��ڲ���: u8* buf��д���ݻ�����
//					 u32 sector��������ַ
//					 u32 cnt����������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
u8 SD_WriteDisk(u8 *buf, u32 sector, u32 cnt)
{
    u8 n;
    if ((u32)buf % 4 != 0)
    {
        for (n = 0; n < cnt; n++)
        {
            memcpy(SDIO_DATA_BUFFER, buf, 512);
            HAL_SD_WriteBlocks(&SDCARD_Handler, SDIO_DATA_BUFFER, sector++, 1, SD_TIMEOUT);
            if (Wait_SDCARD_Ready() != HAL_OK)
                return 1;
            buf += 512;
        }
    }
    else
    {
        HAL_SD_WriteBlocks(&SDCARD_Handler, buf, sector, cnt, SD_TIMEOUT);
        if (Wait_SDCARD_Ready() != HAL_OK)
            return 1;
    }
    return 0;
}

//-----------------------------------------------------------------
// u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)
//-----------------------------------------------------------------
//
// ��������: ��SD��
// ��ڲ���: u8* buf�������ݻ�����
//					 u32 sector��������ַ
//					 u32 cnt����������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
u8 SD_ReadDisk(u8 *buf, u32 sector, u32 cnt)
{
    u8 n;
    if ((u32)buf % 4 != 0)
    {
        for (n = 0; n < cnt; n++)
        {
            HAL_SD_ReadBlocks(&SDCARD_Handler, SDIO_DATA_BUFFER, sector++, 1, SD_TIMEOUT);
            if (Wait_SDCARD_Ready() != HAL_OK)
                return 1;
            memcpy(buf, SDIO_DATA_BUFFER, 512);
            buf += 512;
        }
    }
    else
    {
        HAL_SD_ReadBlocks(&SDCARD_Handler, buf, sector, cnt, SD_TIMEOUT);
        if (Wait_SDCARD_Ready() != HAL_OK)
            return 1;
    }
    return 0;
}

//-----------------------------------------------------------------
// u8 Wait_SDCARD_Ready(void)
//-----------------------------------------------------------------
//
// ��������: �ȴ�SD���������
// ��ڲ���: ��
// �� �� ֵ: HAL_OK ��ʼ����ȷ������ֵ����ʼ������
// ע������: ��
//
//-----------------------------------------------------------------
u8 Wait_SDCARD_Ready(void)
{
    uint32_t loop = SD_TIMEOUT;

    while (loop > 0)
    {
        loop--;
        if (HAL_SD_GetCardState(&SDCARD_Handler) == HAL_SD_CARD_TRANSFER)
        {
            return HAL_OK;
        }
    }
    return HAL_ERROR;
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
