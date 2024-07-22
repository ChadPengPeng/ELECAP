#include "pipline.h"
// core logic to get each frame

u16 frameCache[HEIGHT][WIDTH] = {0};
UIobject *cursor;
void graphInit()
{
    SCREEN_Init();
    headInit();
    cursor = getHead();
    eventInit();
    backgroundUI();
    extern void userInterface();
    userInterface();
    debugUI();
}

int getTimeInterval()
{
    static uint32_t lastTick = 0;
    uint32_t thisTick = HAL_GetTick();
    int deltaT = (int)(thisTick - lastTick);
    lastTick = thisTick;
    return deltaT;
}

int ifLastEventTouch = 0;
#define KeyableObject(obj) (obj->eventListener != NULL && inScreen(obj->x, obj->y))
void processEvent()
{
    while (haveEvent())
    {
        Event event = dequeueEvent();
        // write your event process function
        UIobject *beforeCursor = NULL;
        if (eventCodeMask(event) == Touch && stateMask(event) == OnClick)
        {
            ifLastEventTouch = 1;
            int cursorX = cursorXmask(event);
            int cursorY = cursorYmask(event);

            UIobject *pointer = getHead();
            UIobject *beforepointer = NULL;
            while (pointer->next != NULL)
            {
                beforepointer = pointer;
                pointer = pointer->next;
                if (cursorX < pointer->x + pointer->box[0][0])
                    continue;
                else if (cursorX > pointer->x + pointer->box[0][1])
                    continue;
                else if (cursorY < pointer->y + pointer->box[1][0])
                    continue;
                else if (cursorY > pointer->y + pointer->box[1][1])
                    continue;
                else
                {
                    cursor = pointer;
                    beforeCursor = beforepointer;
                }
            }
            // if cursor has same priority with next, swap them until they have different priority
            // this make cursor the last object (among other same priority objects) that will be shaded last
            while (cursor->next != NULL)
            {
                if (cursor->priority != cursor->next->priority)
                    break;
                // 交换cursor和next
                UIobject *temp = cursor->next;
                cursor->next = temp->next;
                temp->next = cursor;
                beforeCursor->next = temp;
                beforeCursor = beforeCursor->next;
            }
        }
        if (eventCodeMask(event) >= KEY1) // key event
        {
            ifLastEventTouch = 0;
            Event keyEvent = eventCodeMask(event);
            if (keyEvent == KEY1)
            {
                if (stateMask(event) != OnClick)
                    return;
                UIobject *pointer = getHead();
                UIobject *beforeWithEvent = NULL;
                while (pointer->next != NULL)
                {
                    if (KeyableObject(pointer))
                    {
                        beforeWithEvent = pointer;
                    }
                    if (pointer->next == cursor)
                        break;
                    pointer = pointer->next;
                }
                // cursor pointing the first object, move to the last object
                if (beforeWithEvent == NULL)
                {
                    while (cursor->next != NULL)
                    {
                        if (KeyableObject(cursor))
                        {
                            beforeWithEvent = cursor;
                        }
                        cursor = cursor->next;
                    }
                    cursor = beforeWithEvent;
                }
                else
                    cursor = beforeWithEvent;
                return;
            }
            else if (keyEvent == KEY3)
            {
                if (stateMask(event) != OnClick)
                    return;
                while (cursor->next != NULL)
                {
                    cursor = cursor->next;
                    if (KeyableObject(cursor))
                        break;
                }
                if (!KeyableObject(cursor))
                {
                    cursor = getHead();
                    while (cursor->next != NULL)
                    {
                        cursor = cursor->next;
                        if (KeyableObject(cursor))
                            break;
                    }
                }
                return;
            }
            else if (keyEvent == KEY4)
            {
                // your key4 event process function
            }
        }
        if (cursor->eventListener != NULL)
        {
            cursor->eventListener(cursor, event);
        }
    }
}

void updataUI(int deltaT)
{
    UIobject *pointer = getHead();
    while (pointer->next != NULL)
    {
        pointer = pointer->next;
        if (pointer->update != NULL)
        {
            pointer->update(pointer, deltaT);
            // make sure child objects are updated too
            // expecially when child position is relative to parent
            if (pointer->child != NULL)
            {
                UIobject *child = pointer->child;
                do
                {
                    child->childUpdate(child, deltaT, pointer);
                    child = child->childNext;
                } while (child != NULL);
            }
        }
    }
}
// extern void graph();
// #define ShowBox
void shadeUI()
{
    UIobject *pointer = getHead();
    while (pointer->next != NULL)
    {
        pointer = pointer->next;
        if (pointer->shader != NULL)
            pointer->shader(pointer);
#ifdef ShowBox
        cacheRec(pointer->x + pointer->box[0][0], pointer->y + pointer->box[1][0], pointer->x + pointer->box[0][1], pointer->y + pointer->box[1][1], pointer->color);
#endif
        // HAL_Delay(100);      //debug
        // graph();
    }
    // when key is pressed, cursor object box will be shaded
    if (ifLastEventTouch == 0)
        cacheRec(cursor->x + cursor->box[0][0], cursor->y + cursor->box[1][0], cursor->x + cursor->box[0][1], cursor->y + cursor->box[1][1], PINK);
    // shade touching cursor
    else
        shadeCursor();
}

void graph()
{
    SetWindow(0, 0, WIDTH, HEIGHT);
    // for (int i = 0; i < HEIGHT; i++)
    // {
    //     for (int j = 0; j < WIDTH; j++)
    //     {
    //         WriteColor(frameCache[i][j]);
    //     }
    // }
    for (int i = 0; i < GRAPHICSIZE; i++)
    {
        WriteColor(((u16 *)frameCache)[i]);
    }
}
int deltaT = 1;
void nextGraphic()
{

    deltaT = getTimeInterval();

    // zero

    processEvent();

    updataUI(deltaT);

    shadeUI();

    graph();
}