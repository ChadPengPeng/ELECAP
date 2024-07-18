//-----------------------------------------------------------------
// 程序描述:
//     W25QXX驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期:
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)W25QXX初始化和配置
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "dev_w25qxx_lz.h"
#include "bsp_spi.h"
#include "bsp_delay.h"
//-----------------------------------------------------------------

u16 W25QXX_TYPE = 0; // 默认是无

//-----------------------------------------------------------------
// void W25QXX_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: 初始化SPI FLASH的IO口
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Init(void)
{
    u8 temp;
    W25Q_SPI_Enable();
    W25QXX_CS_H;                   // 取消片选
    W25QXX_TYPE = W25QXX_ReadID(); // 读取FLASH ID.
    if (W25QXX_TYPE == W25Q128)    // 若SPI FLASH为W25Q256
    {
        temp = W25QXX_ReadSR(3); // 读取状态寄存器3，判断地址模式
        if ((temp & 0X01) == 0)  // 如果不是4字节地址模式,则进入4字节地址模式
        {
            W25QXX_CS_L;                              // 选中
            SPI1_ReadWriteByte(W25X_Enable4ByteAddr); // 发送进入4字节地址模式指令
            W25QXX_CS_H;                              // 取消片选
        }
    }
}

//-----------------------------------------------------------------
// u8 W25QXX_ReadSR(u8 regno)
//-----------------------------------------------------------------
//
// 函数功能: 读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
// 入口参数: u8 regno：状态寄存器号，范围:1~3
// 返 回 值: u8 byte：状态寄存器值
// 注意事项: 状态寄存器1：
//					 BIT7  6   5   4   3   2   1   0
//					 SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//					 SPR:默认0,状态寄存器保护位,配合WP使用
//					 TB,BP2,BP1,BP0:FLASH区域写保护设置
//					 WEL:写使能锁定
//					 BUSY:忙标记位(1,忙;0,空闲)
//					 默认:0x00
//					 状态寄存器2：
//					 BIT7  6   5   4   3   2   1   0
//					 SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//					 状态寄存器3：
//					 BIT7      6    5    4   3   2   1   0
//					 HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//
//-----------------------------------------------------------------
u8 W25QXX_ReadSR(u8 regno)
{
    u8 byte = 0, command = 0;
    switch (regno)
    {
    case 1:
        command = W25X_ReadStatusReg1; // 读状态寄存器1指令
        break;
    case 2:
        command = W25X_ReadStatusReg2; // 读状态寄存器2指令
        break;
    case 3:
        command = W25X_ReadStatusReg3; // 读状态寄存器3指令
        break;
    default:
        command = W25X_ReadStatusReg1;
        break;
    }
    W25QXX_CS_L;                     // 使能器件
    SPI1_ReadWriteByte(command);     // 发送读取状态寄存器命令
    byte = SPI1_ReadWriteByte(0Xff); // 读取一个字节
    W25QXX_CS_H;                     // 取消片选
    return byte;
}

//-----------------------------------------------------------------
// void W25QXX_Write_SR(u8 regno,u8 sr)
//-----------------------------------------------------------------
//
// 函数功能: 写W25QXX状态寄存器
// 入口参数: u8 regno：状态寄存器号，范围:1~3
//					 u8 sr：写入一个字节
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Write_SR(u8 regno, u8 sr)
{
    u8 command = 0;
    switch (regno)
    {
    case 1:
        command = W25X_WriteStatusReg1; // 写状态寄存器1指令
        break;
    case 2:
        command = W25X_WriteStatusReg2; // 写状态寄存器2指令
        break;
    case 3:
        command = W25X_WriteStatusReg3; // 写状态寄存器3指令
        break;
    default:
        command = W25X_WriteStatusReg1;
        break;
    }
    W25QXX_CS_L;                 // 使能器件
    SPI1_ReadWriteByte(command); // 发送写取状态寄存器命令
    SPI1_ReadWriteByte(sr);      // 写入一个字节
    W25QXX_CS_H;                 // 取消片选
}

//-----------------------------------------------------------------
// void W25QXX_Write_Enable(void)
//-----------------------------------------------------------------
//
// 函数功能: W25QXX写使能
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Write_Enable(void)
{
    W25QXX_CS_L;                          // 使能器件
    SPI1_ReadWriteByte(W25X_WriteEnable); // 发送写使能
    W25QXX_CS_H;                          // 取消片选
}

//-----------------------------------------------------------------
// void W25QXX_Write_Disable(void)
//-----------------------------------------------------------------
//
// 函数功能: WW25QXX写禁止
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Write_Disable(void)
{
    W25QXX_CS_L;                           // 使能器件
    SPI1_ReadWriteByte(W25X_WriteDisable); // 发送写禁止指令
    W25QXX_CS_H;                           // 取消片选
}

//-----------------------------------------------------------------
// u16 W25QXX_ReadID(void)
//-----------------------------------------------------------------
//
// 函数功能: 读取芯片ID
// 入口参数: 无
// 返 回 值: u16 Temp：芯片型号
// 注意事项: 0XEF13,表示芯片型号为W25Q80
//					 0XEF14,表示芯片型号为W25Q16
//					 0XEF15,表示芯片型号为W25Q32
//					 0XEF16,表示芯片型号为W25Q64
//					 0XEF17,表示芯片型号为W25Q128
//					 0XEF18,表示芯片型号为W25Q256
//
//-----------------------------------------------------------------
u16 W25QXX_ReadID(void)
{
    u16 Temp = 0;
    W25QXX_CS_L;
    SPI1_ReadWriteByte(0x90); // 发送读取ID命令
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    Temp |= SPI1_ReadWriteByte(0xFF) << 8;
    Temp |= SPI1_ReadWriteByte(0xFF);
    W25QXX_CS_H;
    return Temp;
}

//-----------------------------------------------------------------
// void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
//-----------------------------------------------------------------
//
// 函数功能: 读取SPI FLASH，在指定地址开始读取指定长度的数据
// 入口参数: u8* pBuffer：数据存储区
//					 u32 ReadAddr:开始读取的地址(24bit)
//					 u16 NumByteToRead:寥〉淖纸谑�(最大65535)
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    u16 i;
    W25QXX_CS_L;                       // 使能器件
    SPI1_ReadWriteByte(W25X_ReadData); // 发送读取命令
    if (W25QXX_TYPE == W25Q256)        // 如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        SPI1_ReadWriteByte((u8)((ReadAddr) >> 24));
    }
    SPI1_ReadWriteByte((u8)((ReadAddr) >> 16)); // 发送24bit地址
    SPI1_ReadWriteByte((u8)((ReadAddr) >> 8));
    SPI1_ReadWriteByte((u8)ReadAddr);
    for (i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = SPI1_ReadWriteByte(0XFF); // 循环读数
    }
    W25QXX_CS_H;
}

//-----------------------------------------------------------------
// void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
//-----------------------------------------------------------------
//
// 函数功能: PI在一页(0~65535)内写入少于256个字节的数据，在指定地址开始写入最大256字节的数据
// 入口参数: u8* pBuffer：数据存储区
//					 u32 WriteAddr：开始写入的地址(24bit)
//					 u16 NumByteToWrite：要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 i;
    W25QXX_Write_Enable();                // SET WEL
    W25QXX_CS_L;                          // 使能器件
    SPI1_ReadWriteByte(W25X_PageProgram); // 发送写页命令
    if (W25QXX_TYPE == W25Q256)           // 如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        SPI1_ReadWriteByte((u8)((WriteAddr) >> 24));
    }
    SPI1_ReadWriteByte((u8)((WriteAddr) >> 16)); // 发送24bit地址
    SPI1_ReadWriteByte((u8)((WriteAddr) >> 8));
    SPI1_ReadWriteByte((u8)WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        SPI1_ReadWriteByte(pBuffer[i]); // 循环写数
    W25QXX_CS_H;                        // 取消片选
    W25QXX_Wait_Busy();                 // 等待写入结束
}

//-----------------------------------------------------------------
// void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
//-----------------------------------------------------------------
//
// 函数功能: 无检验写SPI FLASH
// 入口参数: u8* pBuffer：数据存储区
//					 u32 WriteAddr：开始写入的地址(24bit)
//					 u16 NumByteToWrite：要写入的字节数(最大65535)
// 返 回 值: 无
// 注意事项: 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//					 具有自动换页功能!
//					 在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//
//-----------------------------------------------------------------
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain = 256 - WriteAddr % 256; // 单页剩余的字节数
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite; // 不大于256个字节
    while (1)
    {
        W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain)
            break; // 写入结束了
        else       // NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain; // 减去已经写入了的字节数
            if (NumByteToWrite > 256)
                pageremain = 256; // 一次可以写入256个字节
            else
                pageremain = NumByteToWrite; // 不够256个字节了
        }
    }
}

u8 W25QXX_BUFFER[4096];
//-----------------------------------------------------------------
// void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
//-----------------------------------------------------------------
//
// 函数功能: 写SPI FLASH
// 入口参数: u8* pBuffer：数据存储区
//					 u32 WriteAddr：开始写入的地址(24bit)
//					 u16 NumByteToWrite：要写入的字节数(最大65535)
// 返 回 值: 无
// 注意事项: 该函数带擦除操作!
//
//-----------------------------------------------------------------
void W25QXX_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; // 扇区地址
    secoff = WriteAddr % 4096; // 在扇区内的偏移
    secremain = 4096 - secoff; // 扇区剩余空间大小
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite; //不大于4096个字节
    while (1)
    {
        W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); // 读出整个扇区的内容
        for (i = 0; i < secremain; i++)               // 校验数据
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break; // 需要擦除
        }
        if (i < secremain) // 需要擦除
        {
            W25QXX_Erase_Sector(secpos);    // 擦除这个扇区
            for (i = 0; i < secremain; i++) // 复制
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); // 写入整个扇区
        }
        else
            W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); // 写已经擦除了的,直接写入扇区剩余区间.
        if (NumByteToWrite == secremain)
            break; // 写入结束了
        else       // 写入未结束
        {
            secpos++;   // 扇区地址增1
            secoff = 0; // 偏移位置为0

            pBuffer += secremain;        // 指针偏移
            WriteAddr += secremain;      // 写地址偏移
            NumByteToWrite -= secremain; // 字节数递减
            if (NumByteToWrite > 4096)
                secremain = 4096; // 下一个扇区还是写不完
            else
                secremain = NumByteToWrite; // 下一个扇区可以写完了
        }
    }
}

//-----------------------------------------------------------------
// void W25QXX_Erase_Chip(void)
//-----------------------------------------------------------------
//
// 函数功能: 擦除整个芯片
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable(); // SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_L;                        // 使能器件
    SPI1_ReadWriteByte(W25X_ChipErase); // 发送片擦除命令
    W25QXX_CS_H;                        // 取消片选
    W25QXX_Wait_Busy();                 // 等待芯片擦除结束
}

//-----------------------------------------------------------------
// void W25QXX_Erase_Sector(u32 Dst_Addr)
//-----------------------------------------------------------------
//
// 函数功能: 擦除一个扇区
// 入口参数: u32 Dst_Addr：扇区地址 根据实际容量设置
// 返 回 值: 无
// 注意事项: 擦除一个扇区的最少时间:150ms
//
//-----------------------------------------------------------------
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
    Dst_Addr *= 4096;
    W25QXX_Write_Enable(); // SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_L;                          // 使能器件
    SPI1_ReadWriteByte(W25X_SectorErase); // 发送扇区擦除指令
    if (W25QXX_TYPE == W25Q256)           // 如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        SPI1_ReadWriteByte((u8)((Dst_Addr) >> 24));
    }
    SPI1_ReadWriteByte((u8)((Dst_Addr) >> 16)); // 发送24bit地址
    SPI1_ReadWriteByte((u8)((Dst_Addr) >> 8));
    SPI1_ReadWriteByte((u8)Dst_Addr);
    W25QXX_CS_H;        // 取消片选
    W25QXX_Wait_Busy(); // 等待擦除完成
}

//-----------------------------------------------------------------
// void W25QXX_Wait_Busy(void)
//-----------------------------------------------------------------
//
// 函数功能: 等待空闲
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_Wait_Busy(void)
{
    while ((W25QXX_ReadSR(1) & 0x01) == 0x01)
        ; // 等待BUSY位清空
}

//-----------------------------------------------------------------
// void W25QXX_PowerDown(void)
//-----------------------------------------------------------------
//
// 函数功能: 进入掉电模式
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_PowerDown(void)
{
    W25QXX_CS_L;                        // 使能器件
    SPI1_ReadWriteByte(W25X_PowerDown); // 发送掉电命令
    W25QXX_CS_H;                        // 取消片选
    delay_us(3);                        // 等待TPD
}

//-----------------------------------------------------------------
// void W25QXX_WAKEUP(void)
//-----------------------------------------------------------------
//
// 函数功能: 唤醒
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void W25QXX_WAKEUP(void)
{
    W25QXX_CS_L;                               // 使能器件
    SPI1_ReadWriteByte(W25X_ReleasePowerDown); // send W25X_PowerDown command 0xAB
    W25QXX_CS_H;                               // 取消片选
    delay_us(3);                               // 等待TRES1
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
