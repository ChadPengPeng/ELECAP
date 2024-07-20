#include "interference.h"
// using function in interference.c to implement some other function
u16 FadeColor(u16 color, u16 weight)
{
    return Migrate((getR(color) * weight) >> 8, (getG(color) * weight) >> 8, (getB(color) * weight) >> 8);
}

void DrawTransparentPoint(int x, int y, u16 color, u16 weight)
{
    if(x<0||x>=WIDTH)return;
    if(y<0||y>=HEIGHT)return;
    cachePoint(x, y, FadeColor(color, weight) + FadeColor(getPoint(x, y), 256 - weight));
}

void cacheLine(int x1, int y1, int x2, int y2, u16 color)
{
    u8 outPoints = 0;
    if (x1 > WIDTH - 1)
    {
        x1 = WIDTH - 1;
        outPoints |= 0b01;
    }
    if (x1 < 0)
    {
        x1 = 0;
        outPoints |= 0b01;
    }
    if (x2 > WIDTH - 1)
    {
        x2 = WIDTH - 1;
        outPoints |= 0b10;
    }
    if (x2 < 0)
    {
        x2 = 0;
        outPoints |= 0b10;
    }

    if (y1 > HEIGHT - 1)
    {
        y1 = HEIGHT - 1;
        outPoints |= 0b01;
    }
    if (y1 < 0)
    {
        y1 = 0;
        outPoints |= 0b01;
    }
    if (y2 > HEIGHT - 1)
    {
        y2 = HEIGHT - 1;
        outPoints |= 0b10;
    }
    if (y2 < 0)
    {
        y2 = 0;
        outPoints |= 0b10;
    }

    if (outPoints == 0b11)
        return;

    int xDirect, yDirect;
    int xDis, yDis;

    xDirect = sign(x2 - x1);
    yDirect = sign(y2 - y1);
    xDis = absM(x2 - x1);
    yDis = absM(y2 - y1);

    int xCursor, yCursor;
    xCursor = 0;
    yCursor = 0;
    if (xDis >= yDis)
    {
        int yOverflow = 0;
        for (int i = 0; i <= xDis; i++)
        {
            cachePoint(x1 + xCursor, y1 + yCursor, color);
            xCursor += xDirect;
            yOverflow += 2 * yDis;
            if (yOverflow >= xDis)
            {
                yCursor += yDirect;
                yOverflow -= 2 * xDis;
            }
        }
    }
    else
    {
        int xOverflow = 0;
        for (int i = 0; i <= yDis; i++)
        {
            cachePoint(x1 + xCursor, y1 + yCursor, color);
            yCursor += yDirect;
            xOverflow += 2 * xDis;
            if (xOverflow >= yDis)
            {
                xCursor += xDirect;
                xOverflow -= 2 * yDis;
            }
        }
    }
}

void cacheChar(int x, int y, u8 num, u8 size, u16 color)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // 得到字体一个字符对应点阵集所占的字节数
    num = num - ' ';                                           // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for (t = 0; t < csize; t++)
    {
        if (size == 12)
            temp = asc2_1206[num][t]; // 调用1206字体
        else if (size == 16)
            temp = asc2_1608[num][t]; // 调用1608字体
        else if (size == 24)
            temp = asc2_2412[num][t]; // 调用2412字体
        else if (size == 32)
            temp = asc2_3216[num][t]; // 调用3216字体
        else
            return; // 没有的字库
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
                cachePoint(x, y, color);
            temp <<= 1;
            y++;
            if (y >= HEIGHT)
                return; // 超区域了
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                if (x >= WIDTH)
                    return; // 超区域了
                break;
            }
        }
    }
}

void cacheString(int x, int y, int width, int height, u8 size, char *p, u16 color)
{
    u8 x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }
        if (y >= height)
            break; // 退出
        if ((x + size / 2) > WIDTH)
        {
            x = 0;
            y += size;
        }
        cacheChar(x, y, *p, size, color);
        x += size / 2;
        p++;
    }
}

void cacheCenterString(int x, int y, int width, int height, u8 size, char *p, u16 color)
{
    int row = 1;
    int length = 0;
    int x0 = x;
    int y0 = y;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
            row++;
        }
        if (y >= height)
            break; // 退出
        if ((x + size / 2) > WIDTH)
        {
            x = 0;
            y += size;
        }
        // cacheChar(x, y, *p, size, color);
        x += size / 2;
        length++;
        p++;
    }

    x = x0;
    y = y0;
    if (row == 1)
        while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
        {
            cacheChar(x - length * size / 4, y - row * size / 2, *p, size, color);
            x += size / 2;
            p++;
        }
    else
        while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
        {
            if (x >= width)
            {
                x = x0;
                y += size;
            }
            if (y >= height)
                break; // 退出
            if ((x + size / 2) > WIDTH)
            {
                x = 0;
                y += size;
            }
            cacheChar(x - width / 2, y - row * size / 2, *p, size, color);
            x += size / 2;
            p++;
        }
}

void cacheOneCenter(int x, int y, u8 size, char *p, u16 color)
{
    char *p0 = p;
    int length = 0;
    while ((*p0 <= '~') && (*p0 >= ' '))
    {
        p0++;
        length++;
    }
    while ((*p <= '~') && (*p >= ' '))
    {
        // protect
        if ((x - length * size / 4 + size / 2) > WIDTH)
            return;

        cacheChar(x - length * size / 4, y - size / 2, *p, size, color);
        x += size / 2;
        p++;
    }
}

void cacheRec(int x, int y, int width, int height, u16 color)
{
    int dx = width / 2;
    int dy = height / 2;
    cacheLine(x - dx, y + dy, x + dx - 1, y + dy, color); // down
    cacheLine(x + dx, y + dy, x + dx, y - dy + 1, color); // right
    cacheLine(x + dx, y - dy, x - dx + 1, y - dy, color); // up
    cacheLine(x - dx, y - dy, x - dx, y + dy - 1, color); // left
}

void cacheCircle(int x0, int y0, int r, u16 color)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        if (y0 + y < HEIGHT - 1)
        {
            cachePoint(x0 + x, y0 + y, color); // down
            cachePoint(x0 - x, y0 + y, color);
        }
        if (x0 + y < WIDTH - 1)
        {
            cachePoint(x0 + y, y0 + x, color); // right
            cachePoint(x0 + y, y0 - x, color);
        }
        if (x0 - y > 0)
        {
            cachePoint(x0 - y, y0 + x, color); // left
            cachePoint(x0 - y, y0 - x, color);
        }
        if (y0 - y > 0)
        {
            cachePoint(x0 - x, y0 - y, color); // up
            cachePoint(x0 + x, y0 - y, color);
        }

        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void cacheRoundedRec(int x, int y, int width, int height, int r, u16 color)
{
    int dx = width / 2;
    int dy = height / 2;
    cacheLine(x - dx + r, y + dy, x + dx - r, y + dy, color); // down
    cacheLine(x + dx, y + dy - r, x + dx, y - dy + r, color); // right
    cacheLine(x + dx - r, y - dy, x - dx + r, y - dy, color); // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left

    int x0 = x;
    int y0 = y;
    x = 0;
    y = r;
    int d = 3 - 2 * r;

    int xnr = x0 - dx + r;
    int xpr = x0 + dx - r;
    int ynr = y0 - dy + r;
    int ypr = y0 + dy - r;

    while (x <= y)
    {

        cachePoint(xpr + x, ypr + y, color); // rightdown
        cachePoint(xpr + y, ypr + x, color);
        cachePoint(xnr - y, ypr + x, color); // leftdown
        cachePoint(xnr - x, ypr + y, color);
        cachePoint(xnr - x, ynr - y, color); // leftup
        cachePoint(xnr - y, ynr - x, color);
        cachePoint(xpr + y, ynr - x, color); // rightup
        cachePoint(xpr + x, ynr - y, color);

        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void cacheVLine(int x0, int x1, int y, u16 color)
{
    u16 *thisLine = frameCache[y];
    if (y >= HEIGHT)
        return;
    if (y < 0)
        return;
    if (x1 > WIDTH - 1)
        x1 = WIDTH - 1;
    if (x0 < 0)
        x0 = 0;
    for (int i = x0; i <= x1; i++)
    {
        thisLine[i] = color;
    }
}

void cacheCenterRec(int centerX, int centerY, int halfWidth, int halfHeight, u16 color)
{
    cacheLine(centerX - halfWidth, centerY - halfHeight, centerX - halfWidth, centerY + halfHeight - 1, color);
    cacheLine(centerX - halfWidth, centerY + halfHeight, centerX + halfWidth - 1, centerY + halfHeight, color);
    cacheLine(centerX + halfWidth, centerY + halfHeight, centerX + halfWidth, centerY - halfHeight + 1, color);
    cacheLine(centerX + halfWidth, centerY - halfHeight, centerX - halfWidth + 1, centerY - halfHeight, color);
}

void cacheCenterBlock(int centerX, int centerY, int halfWidth, int halfHeight, u16 color)
{
    for (int j = centerY - halfHeight; j <= centerY + halfHeight; j++)
    {
        cacheVLine(centerX - halfWidth, centerX + halfWidth, j, color);
    }
}

void cacheRoundedRight(int x, int y, int width, int height, int r, u16 color)
{
    int dx = width / 2;
    int dy = height / 2;
    cacheLine(x - dx + r, y + dy, WIDTH - 1, y + dy, color);  // down
    cacheLine(WIDTH - 1, y - dy, x - dx + r, y - dy, color);  // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left

    int x0 = x;
    int y0 = y;
    x = 0;
    y = r;
    int d = 3 - 2 * r;

    int xnr = x0 - dx + r;
    int xpr = x0 + dx - r;
    int ynr = y0 - dy + r;
    int ypr = y0 + dy - r;

    while (x <= y)
    {
        cachePoint(xnr - y, ypr + x, color); // leftdown
        cachePoint(xnr - x, ypr + y, color);
        cachePoint(xnr - x, ynr - y, color); // leftup
        cachePoint(xnr - y, ynr - x, color);

        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void cacheRoundedBackgroundRight(int x, int y, int width, int height, int r, u16 color, u16 backgroundColor)
{
    int dx = width / 2;
    int dy = height / 2;
    cacheLine(x - dx + r, y + dy, WIDTH - 1, y + dy, color);  // down
    cacheLine(WIDTH - 1, y - dy, x - dx + r, y - dy, color);  // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left
    for (int j = y - dy + 1; j < y + dy; j++)
    {
        cacheVLine(x - dx + 1 + r, WIDTH - 1, j, backgroundColor);
    }

    int x0 = x;
    int y0 = y;
    x = 0;
    y = r;
    int d = 3 - 2 * r;

    int xnr = x0 - dx + r;
    int xpr = x0 + dx - r;
    int ynr = y0 - dy + r;
    int ypr = y0 + dy - r;

    while (x <= y)
    {
        cachePoint(xnr - y, ypr + x, color); // leftdown
        cachePoint(xnr - x, ypr + y, color);
        cachePoint(xnr - x, ynr - y, color); // leftup
        cachePoint(xnr - y, ynr - x, color);
        cacheLine(xnr - x, ypr + y - 1, xnr - x, ynr - y + 1, backgroundColor);
        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
            if (y != r)
                cacheLine(xnr - y, ypr + x, xnr - y, ynr - x, backgroundColor);
        }
        x++;
    }
}
