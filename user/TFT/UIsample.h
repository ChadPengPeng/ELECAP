#ifndef __UISAMPLE__
#define __UISAMPLE__

#include "UIobject.h"
#include "interference.h"
#include "stdio.h"

extern void recUI(u16 centerx, u16 centery, u16 width, u16 height, u16 color, int priority);
extern void debugUI();
extern void backgroundUI();

#endif // !__UISAMPLE__