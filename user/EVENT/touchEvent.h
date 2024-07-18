#ifndef __TOUCHEVENT__
#define __TOUCHEVENT__
#include "event.h"
#include "main.h"


typedef struct touchParam_struct
{
    int clickingTick;
    int holding;
    int clickX, clickY;
    int cursorNowX, cursorNowY;
    int touching;
    int longHold;
}touchParam;
extern touchParam touchingParam;


#define cursorXmask(event) ((event)>>12)
#define cursorYmask(event) (((event)>>4)&0b11111111)
#define getCursorEvent(x,y,eventCode) (((x)<<12)|((y)<<4)|eventCode)

extern void addTouchEvent(int x, int y, int sta);
#endif // !__TOUCHEVENT__