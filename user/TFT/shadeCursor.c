#include "shadeCursor.h"

#define holdingR 30
#define slideR 15
int weight;
int maxHoldLong = 0;
void shadeCursor()
{
    int x0 = touchingParam.cursorNowX;
    int y0 = touchingParam.cursorNowY;
    int holdLong = absM(touchingParam.cursorNowX - touchingParam.clickX) + absM(touchingParam.cursorNowY - touchingParam.clickY);
    if (maxHoldLong < holdLong)
        maxHoldLong = holdLong;
    int r = touchingParam.holding * 2;
    if (r > holdingR)
        r = holdingR;
    r -= maxHoldLong;
    if (r < slideR)
        r = slideR;

    u16 color;
    if (touchingParam.longHold)
        color = MacaronYELLOW;
    else
        color = WHITE;

    if (touchingParam.touching)
    {
        weight = 256 - (HAL_GetTick() - touchingParam.clickingTick) / 10;
        if (weight < 127)
            weight = 127;
        if (r - slideR > (holdingR - slideR) / 2)
            touchingParam.longHold = 1;
    }
    else
    {
        weight = weight > 3 ? weight - 4 : 0;
        touchingParam.longHold = 0;
        maxHoldLong = 0;
    }
    if (weight == 0)
        return;

    for (int i = 0; i < MAX_TOUCH_POINTS; i++)
    {
        if (touchingParam.multiTouchSta >> i & 0x1)
        {
            x0 = touchingParam.xList[i];
            y0 = touchingParam.yList[i];
            int x = 0;
            int y = r;
            int d = 3 - 2 * r;

            while (x <= y)
            {
                drawTransparentPoint(x0 + x, y0 + y, color, weight); // down
                drawTransparentPoint(x0 - x, y0 + y, color, weight);
                drawTransparentPoint(x0 + y, y0 + x, color, weight); // right
                drawTransparentPoint(x0 + y, y0 - x, color, weight);
                drawTransparentPoint(x0 - y, y0 + x, color, weight); // left
                drawTransparentPoint(x0 - y, y0 - x, color, weight);
                drawTransparentPoint(x0 - x, y0 - y, color, weight); // up
                drawTransparentPoint(x0 + x, y0 - y, color, weight);

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
    }
}