#ifndef __UIFLOATINGWINDOW__
#define __UIFLOATINGWINDOW__

#include "interface.h"
#include "UIobject.h"

extern UIobject *flotingUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority);
extern void floatingMessage(char *message);
extern void resetFloatingWindowCounter();
extern void updateMessage(char *message);
#endif // __UIFLOATINGWINDOW__