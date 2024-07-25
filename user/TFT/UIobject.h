#ifndef __UIOBJECT__
#define __UIOBJECT__

#include "stdlib.h"
#include "event.h"
#include "bsp_system.h"
#include "interface.h"
#include "string.h"

typedef struct UIobject_struct
{
    struct UIobject_struct *this;
    struct UIobject_struct *next;
    struct UIobject_struct *father;
    struct UIobject_struct *child;
    struct UIobject_struct *childNext;
    short x;
    short y;
    short relativeX;
    short relativeY;
    short aimX;
    short aimY;
    short width;
    short height;
    short priority;
    u16 color;
    // u16 color_weight;
    short box[2][2];
    short transformRec[2][2];
    short param[8];
    void (*eventListener)(struct UIobject_struct *this, Event event);
    void (*update)(struct UIobject_struct *this, int deltaT);
    void (*childUpdate)(struct UIobject_struct *this, int deltaT);
    void (*shader)(struct UIobject_struct *this);
    void *selfStruct;

} UIobject;

typedef void (*selfUpdater)(UIobject *this, int deltaT);
typedef void (*childUpdater)(UIobject *this, int deltaT, UIobject *father);

extern void headInit();
extern UIobject *getHead();
extern UIobject *getUIobject();
extern void priorityInsert(UIobject *node);
extern void delNext(UIobject *node);
extern short getRelativeAxisX(UIobject *child);
extern short getRelativeAxisY(UIobject *child);
extern void childInsert(UIobject *father, UIobject *child);

#endif // !__UIOBJECT__