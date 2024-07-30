#include "touchEvent.h"

touchParam touchingParam = {0};
void addTouchEvent(int x, int y, int sta)
{
    static unsigned char lastClick = 0;
    static State state = NoneState;
    touchingParam.cursorNowX = x;
    touchingParam.cursorNowY = y;
    if (state == OnClick)
        state = Hold;
    if (state == HoldEnd)
        state = NoneState;

    if (lastClick == 0b00000111 && sta)
    {
        state = OnClick;
        touchingParam.clickX = x;
        touchingParam.clickY = y;
        touchingParam.holding = 0;
        touchingParam.clickingTick = HAL_GetTick();
    }
    if (lastClick == 0b00001000 && (!sta) && (state == Hold))
    {
        state = HoldEnd;
    }
    if (state == Hold)
    {
        touchingParam.holding++;
    }
    else
    {
        touchingParam.holding = (touchingParam.holding > 0) ? touchingParam.holding - 1 : 0;
    }
    lastClick = ((lastClick << 1) & 0b1111) | (sta != 0); // save last sta 8 times
    if (state)
    {
        addEvent(getCursorEvent(x, y, Touch, state));
    }
    touchingParam.touching = state == Hold;
}