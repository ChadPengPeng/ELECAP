#include "touchEvent.h"

unsigned char lastClick = 0;
touchParam touchingParam = {0};
void addTouchEvent(int x, int y, int sta)
{
    int eventCode = NoneEvent;
    touchingParam.cursorNowX = x;
    touchingParam.cursorNowY = y;
    if (lastClick == 0b01111111 && sta)
    {
        eventCode = OnClick;
        touchingParam.clickX = x;
        touchingParam.clickY = y;
        touchingParam.holding = 0;
        touchingParam.clickingTick = HAL_GetTick();
        touchingParam.touching = 1;
    }
    if (lastClick == 0b11111111)
    {
        eventCode = Touching;
        touchingParam.holding++;
    }
    if (lastClick == 0b10000000 && (!sta))
    {
        eventCode = TouchingEnd;
        touchingParam.touching = 0;
    }
    if (lastClick == 0)
    {
        touchingParam.holding--;
    }
    lastClick = (lastClick << 1) | (sta != 0); // save last sta 8 times
    if (eventCode)
    {
        addEvent(getCursorEvent(x, y, eventCode));
    }
}