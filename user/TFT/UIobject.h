#ifndef __UIOBJECT__
#define __UIOBJECT__

#include "stdlib.h"
#include "event.h"
#include "bsp_system.h"
#include "interference.h"
#include "string.h"

typedef struct UIobject_struct
{
    struct UIobject_struct *this;
    struct UIobject_struct *next;
    int x;
    int y;
    int priority;
    u16 color;
    //u16 color_weight;
    int box[2][2];
    int transformRec[2][2];
    uint16_t param[8];
    void (*eventListener)(struct UIobject_struct *this, Event event);
    void (*update)(struct UIobject_struct *this, int deltaT);
    void (*shader)(struct UIobject_struct *this);
    void *selfStruct;
} UIobject;

extern void headInit();
extern UIobject *getHead();
extern UIobject *getUIobject();
extern void priorityInsert(UIobject *node);
extern void delNext(UIobject *node);

#endif // !__UIOBJECT__