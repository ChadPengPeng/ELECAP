//-----------------------------------------------------------------
// ��������:
//     ������ʾ��������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����:
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)	������ʾ
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��4.3��/2.8��
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
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
// ��������: ���ֿ��в��ҳ���ģ
// ��ڲ���: unsigned char *code���ַ�ָ�뿪ʼ
//					 unsigned char *mat�����ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С
//					 u8 size�������С
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Get_HzMat(unsigned char *code, unsigned char *mat, u8 size)
{
    unsigned char qh, ql;
    unsigned char i;
    unsigned long foffset;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); // �õ�����һ���ַ���Ӧ������ռ���ֽ���
    qh = *code;
    ql = *(++code);
    if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) // �� ���ú���
    {
        for (i = 0; i < csize; i++)
            *mat++ = 0x00; // �������
        return;            // ��������
    }
    if (ql < 0x7f)
        ql -= 0x40; // ע��!
    else
        ql -= 0x41;
    qh -= 0x81;
    foffset = ((unsigned long)190 * qh + ql) * csize; // �õ��ֿ��е��ֽ�ƫ����
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
// ��������: ��ʾһ��ָ����С�ĺ���
// ��ڲ���: u16 x������X����
//					 u16 y������Y����
//					 u8 *font������GBK��
//					 u8 size�������С
//					 u8 mode��0,������ʾ,1,������ʾ
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Show_Font(u16 x, u16 y, u8 *font, u8 size, u8 mode)
{
    u8 temp, t, t1;
    u16 y0 = y;
    u8 dzk[128];
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); // �õ�����һ���ַ���Ӧ������ռ���ֽ���
    if (size != 12 && size != 16 && size != 24 && size != 32)
        return;                 // ��֧�ֵ�size
    Get_HzMat(font, dzk, size); // �õ���Ӧ��С�ĵ�������
    for (t = 0; t < csize; t++)
    {
        temp = dzk[t]; // �õ���������
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
// ��������: ��ָ��λ�ÿ�ʼ��ʾһ���ַ���
// ��ڲ���: u16 x����ʼX����
//					 u16 y��������ʼY����
//					 u16 width��������
//					 u16 height������߶�
//					 char *str���ַ���
//					 u8 size�������С
//					 u8 mode��0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Show_Str(u16 x, u16 y, u16 width, u16 height, char *str, u8 size, u8 mode)
{
    u16 x0 = x;
    u8 bHz = 0;       // �ַ���������
    while (*str != 0) // ����δ����
    {
        if (!bHz)
        {
            if ((u8)*str > 0x80)
                bHz = 1; // ����
            else         // �ַ�
            {
                if (x > (x0 + width - size / 2)) // ����
                {
                    break; // Խ�緵��
                }
                if ((x + size / 2) > lcddev.width)
                {
                    x = 0;
                    y += size;
                }
                if (*str == 13) // ���з���
                {
                    y += size;
                    x = x0;
                    str++;
                }
                else
                    LCD_ShowChar(x, y, *str, size, mode); // ��Ч����д��
                str++;
                x += size / 2; // �ַ�,Ϊȫ�ֵ�һ��
            }
        }
        else // ����
        {
            bHz = 0;                     // �к��ֿ�
            if (x > (x0 + width - size)) // ����
            {
                break; // Խ�緵��
            }
            if ((x + size / 2) > lcddev.width)
            {
                x = 0;
                y += size;
            }
            Show_Font(x, y, (u8 *)str, size, mode); // ��ʾ�������,������ʾ
            str += 2;
            x += size; // ��һ������ƫ��
        }
    }
}

//-----------------------------------------------------------------
// void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
//-----------------------------------------------------------------
//
// ��������: ��ָ����ȵ��м���ʾ�ַ���
// ��ڲ���: u16 x��������ʼX����
//					 u16 y��������ʼY����
//					 u8*str���ַ���
//					 u8 size�������С
//					 u8 len��ָ��Ҫ��ʾ�Ŀ��	����
// �� �� ֵ: ��
// ע������: ��
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
