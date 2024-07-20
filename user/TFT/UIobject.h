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
    struct UIobject_struct *child;
    struct UIobject_struct *childNext;
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
    void (*childUpdate) (struct UIobject_struct *this, int deltaT, struct UIobject_struct *father);
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
extern void childInsert(UIobject *father, UIobject *child);

#endif // !__UIOBJECT__