#ifndef __UIFLOATINGWINDOW__
#define __UIFLOATINGWINDOW__

#include "interface.h"
#include "UIobject.h"

extern UIobject *flotingUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority);
extern void floatingMessage(char *message);
extern void resetFloatingWindowCounter();
extern void updateMessage(char *message);
#endif // __UIFLOATINGWINDOW__