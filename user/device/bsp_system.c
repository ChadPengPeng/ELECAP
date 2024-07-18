//-----------------------------------------------------------------
// 程序描述:
//     设置系统时钟和MPU内存保护
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期:
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)设置系统时钟和MPU内存保护
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "bsp_system.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
//-----------------------------------------------------------------
//
// 函数功能: 配置MPU
// 入口参数: u32 Number：配置保护区域
//					 u32 BASEADDR：设置基地址
//					 u32 SIZE： 设置保护区域的大小
//					 u8 AP：设置访问权限
//					 u8 DE：是否允许指令访问
//					 u8 S：是否共享
//					 u8 C：是否缓存
//					 u8 B：是否缓冲
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void MPU_Set_Protection(u32 Number, u32 BASEADDR, u32 SIZE, u8 AP, u8 DE, u8 S, u8 C, u8 B)
{
    MPU_Region_InitTypeDef MPU_Initure;

    HAL_MPU_Disable();                         // 配置 MPU 之前先关闭 MPU,配置完成以后在使能 MPU
    MPU_Initure.Enable = MPU_REGION_ENABLE;    // 使能该保护区域
    MPU_Initure.Number = Number;               // 设置保护区域
    MPU_Initure.BaseAddress = BASEADDR;        // 设置基址
    MPU_Initure.Size = SIZE;                   // 设置保护区域大小
    MPU_Initure.SubRegionDisable = 0X00;       // 禁止子区域
    MPU_Initure.TypeExtField = MPU_TEX_LEVEL0; // 设置类型扩展域为 level0
    MPU_Initure.AccessPermission = AP;         // 设置访问权限,
    MPU_Initure.DisableExec = DE;              // 是否允许指令访问(允许读取指令)
    MPU_Initure.IsShareable = S;               // 是否共享
    MPU_Initure.IsCacheable = C;               // 是否缓存
    MPU_Initure.IsBufferable = B;              // 是否缓冲
    HAL_MPU_ConfigRegion(&MPU_Initure);        // 配置 MPU
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);    // 开启 MPU
}

//-----------------------------------------------------------------
// void MPU_Memory_Protection(void)
//-----------------------------------------------------------------
//
// 函数功能: 设置需要的保护区域，AXI SRAM和FMC
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void MPU_Memory_Protection(void)
{
    // 配置AXI SRAM，共512KB字节，全访问，允许指令访问，禁止共享，允许缓存，允许缓冲
    MPU_Set_Protection(
        MPU_REGION_NUMBER0,
        0x24000000,
        MPU_REGION_SIZE_512KB,
        MPU_REGION_FULL_ACCESS,
        MPU_INSTRUCTION_ACCESS_ENABLE,
        MPU_ACCESS_NOT_SHAREABLE,
        MPU_ACCESS_CACHEABLE,
        MPU_ACCESS_BUFFERABLE);

    // 配置FMC，共64MB字节，全访问，允许指令访问，禁止共享，禁止缓存，允许缓冲
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
