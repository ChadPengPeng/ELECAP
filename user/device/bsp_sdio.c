//-----------------------------------------------------------------
// 程序描述:
//     SD卡驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期:
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)	SD卡初始化和读写
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、SD卡
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "bsp_sdio.h"
#include "string.h"
//-----------------------------------------------------------------
SD_HandleTypeDef SDCARD_Handler;
HAL_SD_CardInfoTypeDef sd_info;

// SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,
// 需要用到该数组,确保数据缓存区地址是4字节对齐的.
__ALIGN_BEGIN u8 SDIO_DATA_BUFFER[512];

//发送标志位
__IO uint8_t TX_Flag = 0;
//接受标志位
__IO uint8_t RX_Flag = 0;

//-----------------------------------------------------------------
// u8 SD_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: SD卡初始化
// 入口参数: 无
// 返 回 值: 0 初始化正确；其他值，初始化错误
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 SD_Init(void)
{
    SDCARD_Handler.Instance = SDMMC1;                                              // 定义SDMMC句柄
    SDCARD_Handler.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;                       // 上升沿有效
    SDCARD_Handler.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;           // 关闭节能模式
    SDCARD_Handler.Init.BusWide = SDMMC_BUS_WIDE_4B;                               // 总线宽度为4
    SDCARD_Handler.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE; // 开启硬件流控制
    SDCARD_Handler.Init.ClockDiv = 2;                                              // 时钟分频因子为2
    if (HAL_SD_Init(&SDCARD_Handler) != HAL_OK)                                    // HAL SD 初始化
        return 1;

    // 配置SD总线位宽，配置为4bit模式
    if (HAL_SD_ConfigWideBusOperation(&SDCARD_Handler, SDCARD_Handler.Init.BusWide) != HAL_OK)
        return 2;

    return 0;
}

//-----------------------------------------------------------------
// void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
//-----------------------------------------------------------------
//
// 函数功能: SDIO底层初始化，时钟使能，引脚配置，中断配置
// 入口参数: huart:串口句柄
// 返回参数: 无
// 注意事项: 此函数会被HAL_SD_Init()调用
//
//-----------------------------------------------------------------
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_SDMMC1_CLK_ENABLE(); // 开启SDMMC1时钟

    __HAL_RCC_GPIOC_CLK_ENABLE(); // 使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE(); // 使能GPIOD时钟

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;                // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 高速
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;       // SDIO1复用
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);            // 初始化

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;                // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 高速
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;       // SDIO1复用
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);            // 初始化
}

//-----------------------------------------------------------------
// u8 SD_Erase(u32 sector,u32 cnt)
//-----------------------------------------------------------------
//
// 函数功能: 擦除SD卡
// 入口参数: u32 sector：扇区地址
//					 u32 cnt：扇区个数
// 返 回 值: 0 初始化正确；其他值，初始化错误
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 SD_Erase(u32 sector, u32 cnt)
{
    HAL_SD_Erase(&SDCARD_Handler, sector, sector + cnt);
    //等待擦除完成
    if (Wait_SDCARD_Ready() != HAL_OK)
        return 1;
    else
        return 0;
}

//-----------------------------------------------------------------
// u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt)
//-----------------------------------------------------------------
//
// 函数功能: 写SD卡
// 入口参数: u8* buf：写数据缓存区
//					 u32 sector：扇区地址
//					 u32 cnt：扇区个数
// 返 回 值: 无
// 注意事项: 无
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
// 函数功能: 读SD卡
// 入口参数: u8* buf：读数据缓存区
//					 u32 sector：扇区地址
//					 u32 cnt：扇区个数
// 返 回 值: 无
// 注意事项: 无
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
// 函数功能: 等待SD卡操作完成
// 入口参数: 无
// 返 回 值: HAL_OK 初始化正确；其他值，初始化错误
// 注意事项: 无
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
