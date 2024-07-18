//-----------------------------------------------------------------
// 程序描述:
//     汉字显示驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期:
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)	汉字显示
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、4.3寸/2.8寸
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "bsp_text.h"
#include "bsp_hzup.h"
#include "dev_w25qxx_lz.h"
#include "dev_lcd.h"
#include "string.h"
#include "usart.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
//-----------------------------------------------------------------
//
// 函数功能: 从字库中查找出字模
// 入口参数: unsigned char *code：字符指针开始
//					 unsigned char *mat：数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
//					 u8 size：字体大小
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Get_HzMat(unsigned char *code, unsigned char *mat, u8 size)
{
    unsigned char qh, ql;
    unsigned char i;
    unsigned long foffset;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); // 得到字体一个字符对应点阵集所占的字节数
    qh = *code;
    ql = *(++code);
    if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) // 非 常用汉字
    {
        for (i = 0; i < csize; i++)
            *mat++ = 0x00; // 填充满格
        return;            // 结束访问
    }
    if (ql < 0x7f)
        ql -= 0x40; // 注意!
    else
        ql -= 0x41;
    qh -= 0x81;
    foffset = ((unsigned long)190 * qh + ql) * csize; // 得到字库中的字节偏移量
    switch (size)
    {
    case 12:
        W25QXX_Read(mat, foffset + ftinfo.f12addr, csize);
        break;
    case 16:
        W25QXX_Read(mat, foffset + ftinfo.f16addr, csize);
        break;
    case 24:
        W25QXX_Read(mat, foffset + ftinfo.f24addr, csize);
        break;
    case 32:
        W25QXX_Read(mat, foffset + ftinfo.f32addr, csize);
        break;
    }
}

//-----------------------------------------------------------------
// void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
//-----------------------------------------------------------------
//
// 函数功能: 显示一个指定大小的汉字
// 入口参数: u16 x：汉字X坐标
//					 u16 y：汉字Y坐标
//					 u8 *font：汉字GBK码
//					 u8 size：字体大小
//					 u8 mode：0,正常显示,1,叠加显示
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Show_Font(u16 x, u16 y, u8 *font, u8 size, u8 mode)
{
    u8 temp, t, t1;
    u16 y0 = y;
    u8 dzk[128];
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); // 得到字体一个字符对应点阵集所占的字节数
    if (size != 12 && size != 16 && size != 24 && size != 32)
        return;                 // 不支持的size
    Get_HzMat(font, dzk, size); // 得到相应大小的点阵数据
    for (t = 0; t < csize; t++)
    {
        temp = dzk[t]; // 得到点阵数据
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
                LCD_Fast_DrawPoint(x, y, POINT_COLOR);
            else if (mode == 0)
                LCD_Fast_DrawPoint(x, y, BACK_COLOR);
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

//-----------------------------------------------------------------
// void Show_Str(u16 x,u16 y,u16 width,u16 height,char *str,u8 size,u8 mode)
//-----------------------------------------------------------------
//
// 函数功能: 在指定位置开始显示一个字符串
// 入口参数: u16 x：起始X坐标
//					 u16 y：汉字起始Y坐标
//					 u16 width：区域宽度
//					 u16 height：区域高度
//					 char *str：字符串
//					 u8 size：字体大小
//					 u8 mode：0,非叠加方式;1,叠加方式
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Show_Str(u16 x, u16 y, u16 width, u16 height, char *str, u8 size, u8 mode)
{
    u16 x0 = x;
    u8 bHz = 0;       // 字符或者中文
    while (*str != 0) // 数据未结束
    {
        if (!bHz)
        {
            if ((u8)*str > 0x80)
                bHz = 1; // 中文
            else         // 字符
            {
                if (x > (x0 + width - size / 2)) // 换行
                {
                    break; // 越界返回
                }
                if ((x + size / 2) > lcddev.width)
                {
                    x = 0;
                    y += size;
                }
                if (*str == 13) // 换行符号
                {
                    y += size;
                    x = x0;
                    str++;
                }
                else
                    LCD_ShowChar(x, y, *str, size, mode); // 有效部分写入
                str++;
                x += size / 2; // 字符,为全字的一半
            }
        }
        else // 中文
        {
            bHz = 0;                     // 有汉字库
            if (x > (x0 + width - size)) // 换行
            {
                break; // 越界返回
            }
            if ((x + size / 2) > lcddev.width)
            {
                x = 0;
                y += size;
            }
            Show_Font(x, y, (u8 *)str, size, mode); // 显示这个汉字,空心显示
            str += 2;
            x += size; // 下一个汉字偏移
        }
    }
}

//-----------------------------------------------------------------
// void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
//-----------------------------------------------------------------
//
// 函数功能: 在指定宽度的中间显示字符串
// 入口参数: u16 x：汉字起始X坐标
//					 u16 y：汉字起始Y坐标
//					 u8*str：字符串
//					 u8 size：字体大小
//					 u8 len：指定要显示的宽度	长度
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Show_Str_Mid(u16 x, u16 y, u8 *str, u8 size, u8 len)
{
    u16 strlenth = 0;
    strlenth = strlen((const char *)str);
    strlenth *= size / 2;
    if (strlenth > len)
        Show_Str(x, y, lcddev.width, lcddev.height, (char *)str, size, 1);
    else
    {
        strlenth = (len - strlenth) / 2;
        Show_Str(strlenth + x, y, lcddev.width, lcddev.height, (char *)str, size, 1);
    }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
