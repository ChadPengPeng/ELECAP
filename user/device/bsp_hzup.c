//-----------------------------------------------------------------
// ��������:
//     �ֿ������������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����:
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)	�ֿ����
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��4.3��/2.8��
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "bsp_hzup.h"
#include "string.h"
#include "stdio.h"
#include "ff.h"
#include "dev_w25qxx_lz.h"
#include "dev_lcd.h"
#include "bsp_delay.h"
#include "usart.h"
#include "diskio.h"
//-----------------------------------------------------------------
// �ֿ�����ռ�õ�����������С(4���ֿ�+unigbk��+�ֿ���Ϣ=6302984�ֽ�,Լռ1539��W25QXX����,һ������4K�ֽ�)
#define FONTSECSIZE 1539
// �ֿ�����ʼ��ַ
#define FONTINFOADDR 1024 * 1024 * 8 // Apollo STM32�������Ǵ�8M��ַ�Ժ�ʼ����ֿ�
                                     // ǰ��8M��fatfsռ����.
                                     // 8M�Ժ����4���ֿ�+UNIGBK.BIN,�ܴ�С6.01M,���ֿ�ռ����,���ܶ�!
                                     // 14.01M�Ժ�,�û���������ʹ��.

// ���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;

u8 read_buffer[4096]; // ���ļ�������
u8 buf[4096];         // ��FLASH������
// �ֿ����ڴ����е�·��
u8 *const GBK_PATH[5] =
    {
        (u8 *)"0:/SYSTEM/FONT/UNIGBK.BIN", // UNIGBK.BIN�Ĵ��λ��
        (u8 *)"0:/SYSTEM/FONT/GBK12.FON",  // GBK12�Ĵ��λ��
        (u8 *)"0:/SYSTEM/FONT/GBK16.FON",  // GBK16�Ĵ��λ��
        (u8 *)"0:/SYSTEM/FONT/GBK24.FON",  // GBK24�Ĵ��λ��
        (u8 *)"0:/SYSTEM/FONT/GBK32.FON",  // GBK32�Ĵ��λ��
};
// ����ʱ����ʾ��Ϣ
u8 *const UPDATE_REMIND_TBL[5] =
    {
        (u8 *)"Updating UNIGBK.BIN", // ��ʾ���ڸ���UNIGBK.bin
        (u8 *)"Updating GBK12.FON ", // ��ʾ���ڸ���GBK12
        (u8 *)"Updating GBK16.FON ", // ��ʾ���ڸ���GBK16
        (u8 *)"Updating GBK24.FON ", // ��ʾ���ڸ���GBK24
        (u8 *)"Updating GBK32.FON ", // ��ʾ���ڸ���GBK32
};

//-----------------------------------------------------------------
// u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos)
//-----------------------------------------------------------------
//
// ��������: ��ʾ��ǰ������½���
// ��ڲ���: u16 x��X����
//					 u16 y��Y����
//					 u8 size�������С
//					 u32 fsize�������ļ���С
//					 u32 pos����ǰ�ļ�ָ��λ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
u32 fupd_prog(u16 x, u16 y, u8 size, u32 fsize, u32 pos)
{
    float prog;
    u8 t = 0XFF;
    prog = (float)pos / fsize;
    prog *= 100;
    if (t != prog)
    {
        LCD_ShowString(x + 3 * size / 2, y, 240, 320, size, "%");
        t = prog;
        if (t > 100)
            t = 100;
        LCD_ShowNum(x, y, t, 3, size); // ��ʾ��ֵ
    }
    return 0;
}

//-----------------------------------------------------------------
// u8 update_font(u16 x,u16 y,u8 size)
//-----------------------------------------------------------------
//
// ��������: ���������ļ�,UNIGBK,GBK12,GBK16,GBK24,GBK32һ�����
// ��ڲ���: u16 x��X����
//					 u16 y��Y����
//					 u8 size�������С
// �� �� ֵ: 0,���³ɹ�;
//		 			 ����,�������.
// ע������: ��
//
//-----------------------------------------------------------------
u8 update_font(u16 x, u16 y, u8 size)
{
    FIL file;           // �ļ�����
    static FRESULT res; // FRESULT���������������
    unsigned int counter;
    u16 i, j, k;
    u32 flashaddr = 0;
    u32 offx = 0;

    LCD_ShowString(x + 10, y, 240, 320, size, "Erasing sectors... "); // ��ʾ���ڲ�������
    for (i = 0; i < FONTSECSIZE; i++)                                 // �Ȳ����ֿ�����,���д���ٶ�
    {
        fupd_prog(x + 20 * size / 2, y, size, FONTSECSIZE, i);            // ������ʾ
        W25QXX_Read((u8 *)buf, ((FONTINFOADDR / 4096) + i) * 4096, 4096); // ������������������
        for (j = 0; j < 1024; j++)                                        // У������
        {
            if (buf[j] != 0XFF)
                break; // ��Ҫ����
        }
        if (j != 1024)
            W25QXX_Erase_Sector((FONTINFOADDR / 4096) + i); // ��Ҫ����������
    }
    res = f_mount(&fatfs_sd, "0:", 1); // ����SD��
    if (res != FR_OK)
    {
        printf("f_mount error!\r\n"); //����ʧ��
        while (1)
        {
            return 0xFF;
        }
    }
    else
        printf("f_mount successful!\r\n"); // ���سɹ�

    for (k = 0; k < 5; k++)
    {
        res = f_open(&file, (const TCHAR *)GBK_PATH[k], FA_READ); // ��������0�ϵ�Դ�ļ�
        if (res != FR_OK)
        {
            printf("f_open error!\r\n"); // ���ļ�ʧ��
            while (1)
            {
                return 1 + i;
            }
        }
        else
        {
            printf("f_open successful!\r\n");
        }

        res = f_lseek(&file, 0); // �ƶ���ָ�뵽�ļ���
        if (res != FR_OK)
        {
            printf("f_lseek error!\r\n");
            while (1)
            {
                return 1 + i;
            }
        }
        else
        {
            printf("f_lseek successful!\r\n");
        }

        switch (k)
        {
        case 0:                                              // ����UNIGBK.BIN
            ftinfo.ugbkaddr = FONTINFOADDR + sizeof(ftinfo); // ��Ϣͷ֮�󣬽���UNIGBKת�����
            ftinfo.ugbksize = file.fsize;                    // UNIGBK��С
            flashaddr = ftinfo.ugbkaddr;                     // UNIGBK����ʼ��ַ
            break;
        case 1:
            ftinfo.f12addr = ftinfo.ugbkaddr + ftinfo.ugbksize; // UNIGBK֮�󣬽���GBK12�ֿ�
            ftinfo.gbk12size = file.fsize;                      // GBK12�ֿ��С
            flashaddr = ftinfo.f12addr;                         // GBK12����ʼ��ַ
            break;
        case 2:
            ftinfo.f16addr = ftinfo.f12addr + ftinfo.gbk12size; // GBK12֮�󣬽���GBK16�ֿ�
            ftinfo.gbk16size = file.fsize;                      // GBK16�ֿ��С
            flashaddr = ftinfo.f16addr;                         // GBK16����ʼ��ַ
            break;
        case 3:
            ftinfo.f24addr = ftinfo.f16addr + ftinfo.gbk16size; // GBK16֮�󣬽���GBK24�ֿ�
            ftinfo.gbk24size = file.fsize;                      // GBK24�ֿ��С
            flashaddr = ftinfo.f24addr;                         // GBK24����ʼ��ַ
            break;
        case 4:
            ftinfo.f32addr = ftinfo.f24addr + ftinfo.gbk24size; // GBK24֮�󣬽���GBK32�ֿ�
            ftinfo.gbk32size = file.fsize;                      // GBK32�ֿ��С
            flashaddr = ftinfo.f32addr;                         // GBK32����ʼ��ַ
            break;
        }
        offx = 0;
        LCD_ShowString(x + 10, y, 240, 320, size, (char *)UPDATE_REMIND_TBL[k]); // ��ʾ���µ��ļ�
        while (res == FR_OK)                                                     // ��ѭ��ִ��
        {
            counter = 0;                                              // ��ȡ���ݸ�������
            res = f_read(&file, read_buffer, 4096, (UINT *)&counter); // ��ȡ���ݣ�counter��ȡ�ĸ���
            if (res != FR_OK)
                break;                                         // ��ȡʧ��
            W25QXX_Write(read_buffer, offx + flashaddr, 4096); // ��0��ʼд��4096������
            offx += counter;
            fupd_prog(x + 20 * size / 2, y, size, file.fsize, offx); // ������ʾ
            if (counter != 4096)
                break; // ������.
        }
        f_close(&file); // �ر��ļ�
    }
    // ��ȫ������
    ftinfo.fontok = 0XAA;
    W25QXX_Write((u8 *)&ftinfo, FONTINFOADDR, sizeof(ftinfo)); // �����ֿ���Ϣ
    return 0;
}

//-----------------------------------------------------------------
// u8 font_init(void)
//-----------------------------------------------------------------
//
// ��������: ��ʼ������
// ��ڲ���: ��
// �� �� ֵ: 0,�ֿ����.
//		 			 ����,�ֿⶪʧ
// ע������: ��
//
//-----------------------------------------------------------------
u8 font_init(void)
{
    u8 t = 0;
    W25QXX_Init();
    while (t < 10) // ������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
    {
        t++;
        W25QXX_Read((u8 *)&ftinfo, FONTINFOADDR, sizeof(ftinfo)); // ����ftinfo�ṹ������
        if (ftinfo.fontok == 0XAA)
            break;
        delay_ms(20);
    }
    if (ftinfo.fontok != 0XAA)
        return 1;
    return 0;
}

//-----------------------------------------------------------------
// ͨ�� ff_convert �������������飬ʵ�� UNICODE �� GBK �Ļ�ת
//-----------------------------------------------------------------
WCHAR ff_convert(           /* Converted code, 0 means conversion error */
                 WCHAR src, /* Character code to be converted */
                 UINT dir   /* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
)
{
    WCHAR t[2];
    WCHAR c;
    u32 i, li, hi;
    u16 n;
    u32 gbk2uni_offset = 0;

    if (src < 0x80)
        c = src; // ASCII,ֱ�Ӳ���ת��.
    else
    {
        if (dir) // GBK 2 UNICODE
        {
            gbk2uni_offset = ftinfo.ugbksize / 2;
        }
        else // UNICODE 2 GBK
        {
            gbk2uni_offset = 0;
        }
        /* Unicode to OEMCP */
        hi = ftinfo.ugbksize / 2; //�԰뿪.
        hi = hi / 4 - 1;
        li = 0;
        for (n = 16; n; n--)
        {
            i = li + (hi - li) / 2;
            W25QXX_Read((u8 *)&t, ftinfo.ugbkaddr + i * 4 + gbk2uni_offset, 4); //����4���ֽ�
            if (src == t[0])
                break;
            if (src > t[0])
                li = i;
            else
                hi = i;
        }
        c = n ? t[1] : 0;
    }
    return c;
}

WCHAR ff_wtoupper(          /* Upper converted character */
                  WCHAR chr /* Input character */
)
{
    static const WCHAR tbl_lower[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0xA1, 0x00A2, 0x00A3, 0x00A5, 0x00AC, 0x00AF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0x0FF, 0x101, 0x103, 0x105, 0x107, 0x109, 0x10B, 0x10D, 0x10F, 0x111, 0x113, 0x115, 0x117, 0x119, 0x11B, 0x11D, 0x11F, 0x121, 0x123, 0x125, 0x127, 0x129, 0x12B, 0x12D, 0x12F, 0x131, 0x133, 0x135, 0x137, 0x13A, 0x13C, 0x13E, 0x140, 0x142, 0x144, 0x146, 0x148, 0x14B, 0x14D, 0x14F, 0x151, 0x153, 0x155, 0x157, 0x159, 0x15B, 0x15D, 0x15F, 0x161, 0x163, 0x165, 0x167, 0x169, 0x16B, 0x16D, 0x16F, 0x171, 0x173, 0x175, 0x177, 0x17A, 0x17C, 0x17E, 0x192, 0x3B1, 0x3B2, 0x3B3, 0x3B4, 0x3B5, 0x3B6, 0x3B7, 0x3B8, 0x3B9, 0x3BA, 0x3BB, 0x3BC, 0x3BD, 0x3BE, 0x3BF, 0x3C0, 0x3C1, 0x3C3, 0x3C4, 0x3C5, 0x3C6, 0x3C7, 0x3C8, 0x3C9, 0x3CA, 0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43A, 0x43B, 0x43C, 0x43D, 0x43E, 0x43F, 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447, 0x448, 0x449, 0x44A, 0x44B, 0x44C, 0x44D, 0x44E, 0x44F, 0x451, 0x452, 0x453, 0x454, 0x455, 0x456, 0x457, 0x458, 0x459, 0x45A, 0x45B, 0x45C, 0x45E, 0x45F, 0x2170, 0x2171, 0x2172, 0x2173, 0x2174, 0x2175, 0x2176, 0x2177, 0x2178, 0x2179, 0x217A, 0x217B, 0x217C, 0x217D, 0x217E, 0x217F, 0xFF41, 0xFF42, 0xFF43, 0xFF44, 0xFF45, 0xFF46, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0xFF4B, 0xFF4C, 0xFF4D, 0xFF4E, 0xFF4F, 0xFF50, 0xFF51, 0xFF52, 0xFF53, 0xFF54, 0xFF55, 0xFF56, 0xFF57, 0xFF58, 0xFF59, 0xFF5A, 0};
    static const WCHAR tbl_upper[] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x21, 0xFFE0, 0xFFE1, 0xFFE5, 0xFFE2, 0xFFE3, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0x178, 0x100, 0x102, 0x104, 0x106, 0x108, 0x10A, 0x10C, 0x10E, 0x110, 0x112, 0x114, 0x116, 0x118, 0x11A, 0x11C, 0x11E, 0x120, 0x122, 0x124, 0x126, 0x128, 0x12A, 0x12C, 0x12E, 0x130, 0x132, 0x134, 0x136, 0x139, 0x13B, 0x13D, 0x13F, 0x141, 0x143, 0x145, 0x147, 0x14A, 0x14C, 0x14E, 0x150, 0x152, 0x154, 0x156, 0x158, 0x15A, 0x15C, 0x15E, 0x160, 0x162, 0x164, 0x166, 0x168, 0x16A, 0x16C, 0x16E, 0x170, 0x172, 0x174, 0x176, 0x179, 0x17B, 0x17D, 0x191, 0x391, 0x392, 0x393, 0x394, 0x395, 0x396, 0x397, 0x398, 0x399, 0x39A, 0x39B, 0x39C, 0x39D, 0x39E, 0x39F, 0x3A0, 0x3A1, 0x3A3, 0x3A4, 0x3A5, 0x3A6, 0x3A7, 0x3A8, 0x3A9, 0x3AA, 0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41A, 0x41B, 0x41C, 0x41D, 0x41E, 0x41F, 0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427, 0x428, 0x429, 0x42A, 0x42B, 0x42C, 0x42D, 0x42E, 0x42F, 0x401, 0x402, 0x403, 0x404, 0x405, 0x406, 0x407, 0x408, 0x409, 0x40A, 0x40B, 0x40C, 0x40E, 0x40F, 0x2160, 0x2161, 0x2162, 0x2163, 0x2164, 0x2165, 0x2166, 0x2167, 0x2168, 0x2169, 0x216A, 0x216B, 0x216C, 0x216D, 0x216E, 0x216F, 0xFF21, 0xFF22, 0xFF23, 0xFF24, 0xFF25, 0xFF26, 0xFF27, 0xFF28, 0xFF29, 0xFF2A, 0xFF2B, 0xFF2C, 0xFF2D, 0xFF2E, 0xFF2F, 0xFF30, 0xFF31, 0xFF32, 0xFF33, 0xFF34, 0xFF35, 0xFF36, 0xFF37, 0xFF38, 0xFF39, 0xFF3A, 0};
    int i;

    for (i = 0; tbl_lower[i] && chr != tbl_lower[i]; i++)
        ;

    return tbl_lower[i] ? tbl_upper[i] : chr;
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
