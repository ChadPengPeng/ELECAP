#ifndef __TOUCHEVENT__
#define __TOUCHEVENT__
#include "event.h"
#include "main.h"

typedef struct touchParam_struct
{
    int clickingTick;
    int holding;
    int touching;
    int clickX, clickY;
    int cursorNowX, cursorNowY;
    int longHold;
} touchParam;
extern touchParam touchingParam;

#define cursorXmask(event) ((event) >> 14)
#define cursorYmask(event) (((event) >> 6) & 0b11111111)
#define getCursorEvent(x, y, eventCode, state) (((x) << 14) | ((y) << 6) | getEvent(eventCode, state))

extern void addTouchEvent(int x, int y, int sta);
#endif // !__TOUCHEVENT__