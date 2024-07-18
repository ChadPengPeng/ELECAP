#include "shadeCursor.h"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define holdingR 30
#define slideR 15
int weight;
int maxHoldLong = 0;
void shadeCursor()
{
    int x0 = touchingParam.cursorNowX;
    int y0 = touchingParam.cursorNowY;
    int holdLong = abs(touchingParam.cursorNowX - touchingParam.clickX) + abs(touchingParam.cursorNowY - touchingParam.clickY);
    if (maxHoldLong<holdLong) maxHoldLong=holdLong;
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

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        DrawTransparentPoint(x0 + x, y0 + y, color, weight); // down
        DrawTransparentPoint(x0 - x, y0 + y, color, weight);
        DrawTransparentPoint(x0 + y, y0 + x, color, weight); // right
        DrawTransparentPoint(x0 + y, y0 - x, color, weight);
        DrawTransparentPoint(x0 - y, y0 + x, color, weight); // left
        DrawTransparentPoint(x0 - y, y0 - x, color, weight);
        DrawTransparentPoint(x0 - x, y0 - y, color, weight); // up
        DrawTransparentPoint(x0 + x, y0 - y, color, weight);

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