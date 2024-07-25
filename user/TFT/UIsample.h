#ifndef __UISAMPLE__
#define __UISAMPLE__

#include "UIobject.h"
#include "interface.h"
#include "stdio.h"
#include "touchEvent.h"

extern void recUI(short centerx, short centery, short width, short height, u16 color, int priority);
extern void debugUI();
extern void backgroundUI();

#endif // !__UISAMPLE__