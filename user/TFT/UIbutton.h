#ifndef __UIBUTTON__
#define __UIBUTTON__

#include "UIobject.h"
#include "interface.h"
#include "UIfloatingWindow.h"
extern UIobject *buttonUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority);
extern UIobject *sliderUI(short centerx, short centery, short width, short height, int priority);
extern void setSliderY(UIobject *this, short y);
extern UIobject *drawerUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority);

#endif // !__UIBUTTON__