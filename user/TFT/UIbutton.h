#ifndef __UIBUTTON__
#define __UIBUTTON__

#include "UIobject.h"
#include "interface.h"

extern UIobject *buttonUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority, UIobject *father);
extern UIobject *drawerUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority);

#endif // !__UIBUTTON__