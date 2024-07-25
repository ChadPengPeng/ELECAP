#include "interface.h"
// using function in interference.c to implement some other function
u16 fadeColor(u16 color, u16 weight)
{
    return Migrate((getR(color) * weight) >> 8, (getG(color) * weight) >> 8, (getB(color) * weight) >> 8);
}

u16 approachColor(u16 color, u16 target, u16 weight)
{
    int r = getR(color);
    int g = getG(color);
    int b = getB(color);
    int tr = getR(target);
    int tg = getG(target);
    int tb = getB(target);
    r = approach((float)r, (float)tr, (float)(weight / 256.0));
    g = approach((float)g, (float)tg, (float)(weight / 256.0));
    b = approach((float)b, (float)tb, (float)(weight / 256.0));
    return Migrate(r, g, b);
}

u16 approachColorDiv(u16 color, u16 target, int div)
{
    int r = getR(color);
    int g = getG(color);
    int b = getB(color);
    int tr = getR(target);
    int tg = getG(target);
    int tb = getB(target);
    r = approachDiv(r, tr, div);
    g = approachDiv(g, tg, div);
    b = approachDiv(b, tb, div);
    return Migrate(r, g, b);
}

void drawTransparentPoint(short x, short y, u16 color, u16 weight)
{
    if (x < 0 || x >= WIDTH)
        return;
    if (y < 0 || y >= HEIGHT)
        return;
    cachePoint(x, y, fadeColor(color, weight) + fadeColor(getPoint(x, y), 256 - weight));
}

void cacheLine(short x1, short y1, short x2, short y2, u16 color)
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

    short xDirect, yDirect;
    short xDis, yDis;

    xDirect = sign(x2 - x1);
    yDirect = sign(y2 - y1);
    xDis = absM(x2 - x1);
    yDis = absM(y2 - y1);

    short xCursor, yCursor;
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

void cacheChar(short x, short y, u8 num, u8 size, u16 color)
{
    if (x < 0 || x + (short)size / 2 >= WIDTH)
        return;
    if (y < 0 || y + (short)size >= HEIGHT)
        return;
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

void cacheString(short x, short y, short width, short height, u8 size, char *p, u16 color)
{
    u8 x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ') || (*p == '\n')) // 判断是不是非法字符!
    {
        if (x >= width || *p == '\n')
        {
            x = x0;
            y += size;
        }
        if (y >= height)
            break; // 退出
        if ((*p <= '~') && (*p >= ' '))
            cacheChar(x, y, *p, size, color);
        x += size / 2;
        p++;
    }
}

void cacheCenterString(short x, short y, short width, short height, u8 size, char *p, u16 color)
{
    int row = 1;
    int length = 0;
    short x0 = x;
    short y0 = y;
    char *p0 = p;
    short right = x + width;
    short bottom = y + height;
    while ((*p <= '~') && (*p >= ' ') || (*p == '\n')) // 判断是不是非法字符!
    {
        if (x >= right || *p == '\n')
        {
            x = x0;
            y += size;
            row++;
        }
        if (y >= bottom)
            break; // 退出
        x += size / 2;
        length++;
        p++;
    }

    x = x0;
    y = y0;
    p = p0;
    if (row == 1)
        while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
        {
            if ((x - length * size / 4 + size / 2) > WIDTH)
                return;
            cacheChar(x - length * size / 4, y - row * size / 2, *p, size, color);
            x += size / 2;
            p++;
        }
    else
        while ((*p <= '~') && (*p >= ' ') || (*p == '\n')) // 判断是不是非法字符!
        {
            if (x >= right || *p == '\n')
            {
                x = x0;
                y += size;
            }
            if (y >= bottom)
                break; // 退出
            if ((*p <= '~') && (*p >= ' '))
                cacheChar(x - width / 2, y - row * size / 2, *p, size, color);
            x += size / 2;
            p++;
        }
}

int getRow(short x, short y, short width, short size, char *p)
{
    int row = 1;
    int length = 0;
    short x0 = x;
    width += x;
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
            row++;
        }
        if (y >= HEIGHT)
            break; // 退出
        x += size / 2;
        length++;
        p++;
    }
    return row;
}

void cacheOneCenter(short x, short y, u8 size, char *p, u16 color)
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
        cacheChar(x - length * size / 4, y - size / 2, *p, size, color);
        x += size / 2;
        p++;
    }
}

void cacheCircle(short x0, short y0, short r, u16 color)
{
    short x = 0;
    short y = r;
    short d = 3 - 2 * r;

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

void cacheRoundedRec(short x, short y, short width, short height, int r, u16 color)
{
    short dx = width / 2;
    short dy = height / 2;
    cacheLine(x - dx + r, y + dy, x + dx - r, y + dy, color); // down
    cacheLine(x + dx, y + dy - r, x + dx, y - dy + r, color); // right
    cacheLine(x + dx - r, y - dy, x - dx + r, y - dy, color); // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left

    short x0 = x;
    short y0 = y;
    x = 0;
    y = r;
    short d = 3 - 2 * r;

    short xnr = x0 - dx + r;
    short xpr = x0 + dx - r;
    short ynr = y0 - dy + r;
    short ypr = y0 + dy - r;

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

void cacheVLine(short x0, short x1, short y, u16 color)
{
    u16 *thisLine = frameCache + y * WIDTH;
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

void cacheRec(short x1, short y1, short x2, short y2, u16 color)
{

    cacheLine(x1, y1, x2, y1, color);
    cacheLine(x2, y1, x2, y2, color);
    cacheLine(x2, y2, x1, y2, color);
    cacheLine(x1, y2, x1, y1, color);
}

void cacheCenterRec(short centerX, short centerY, short width, short height, u16 color)
{
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    cacheLine(centerX - halfWidth, centerY - halfHeight, centerX - halfWidth, centerY + halfHeight - 1, color);
    cacheLine(centerX - halfWidth, centerY + halfHeight, centerX + halfWidth - 1, centerY + halfHeight, color);
    cacheLine(centerX + halfWidth, centerY + halfHeight, centerX + halfWidth, centerY - halfHeight + 1, color);
    cacheLine(centerX + halfWidth, centerY - halfHeight, centerX - halfWidth + 1, centerY - halfHeight, color);
}

void cacheCenterBlock(short centerX, short centerY, short width, short height, u16 color)
{
    short halfWidth = width / 2;
    short halfHeight = height / 2;
    for (int j = centerY - halfHeight; j <= centerY + halfHeight; j++)
    {
        cacheVLine(centerX - halfWidth, centerX + halfWidth, j, color);
    }
}

void cacheRoundedRight(short x, short y, short width, short height, short r, u16 color)
{
    short dx = width / 2;
    short dy = height / 2;
    cacheLine(x - dx + r, y + dy, WIDTH - 1, y + dy, color);  // down
    cacheLine(WIDTH - 1, y - dy, x - dx + r, y - dy, color);  // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left

    short x0 = x;
    short y0 = y;
    x = 0;
    y = r;
    short d = 3 - 2 * r;

    short xnr = x0 - dx + r;
    short xpr = x0 + dx - r;
    short ynr = y0 - dy + r;
    short ypr = y0 + dy - r;

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

void cacheRoundedBackgroundRight(short x, short y, short width, short height, short r, u16 color, u16 backgroundColor)
{
    short dx = width / 2;
    short dy = height / 2;
    cacheLine(x - dx + r, y + dy, WIDTH - 1, y + dy, color);  // down
    cacheLine(WIDTH - 1, y - dy, x - dx + r, y - dy, color);  // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left
    for (int j = y - dy + 1; j < y + dy; j++)
    {
        cacheVLine(x - dx + 1 + r, WIDTH - 1, j, backgroundColor);
    }

    short x0 = x;
    short y0 = y;
    x = 0;
    y = r;
    short d = 3 - 2 * r;

    short xnr = x0 - dx + r;
    short xpr = x0 + dx - r;
    short ynr = y0 - dy + r;
    short ypr = y0 + dy - r;

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

void cacheRoundedRecBackground(short x, short y, short width, short height, short r, u16 color, u16 backgroundColor)
{
    short dx = width / 2;
    short dy = height / 2;
    cacheLine(x - dx + r, y + dy, x + dx - r, y + dy, color); // down
    cacheLine(x + dx, y + dy - r, x + dx, y - dy + r, color); // right
    cacheLine(x + dx - r, y - dy, x - dx + r, y - dy, color); // up
    cacheLine(x - dx, y - dy + r, x - dx, y + dy - r, color); // left
    for (int j = y - dy + 1; j < y + dy; j++)
    {
        cacheVLine(x - dx + 1 + r, x + dx - r - 1, j, backgroundColor);
    }

    short x0 = x;
    short y0 = y;
    x = 0;
    y = r;
    short d = 3 - 2 * r;

    short xnr = x0 - dx + r;
    short xpr = x0 + dx - r;
    short ynr = y0 - dy + r;
    short ypr = y0 + dy - r;

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
        cacheLine(xnr - x, ypr + y - 1, xnr - x, ynr - y + 1, backgroundColor);
        cacheLine(xpr + x, ypr + y - 1, xpr + x, ynr - y + 1, backgroundColor);

        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
            if (y != r)
            {
                cacheLine(xnr - y, ypr + x, xnr - y, ynr - x, backgroundColor);
                cacheLine(xpr + y, ypr + x, xpr + y, ynr - x, backgroundColor);
            }
        }
        x++;
    }
}