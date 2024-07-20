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
    // your init object
    backgroundUI();
    recUI(120, 160, 100, 100, BLUE, 2);
    recUI(130, 160, 100, 100, PURPLE, 2);
    recUI(140, 160, 100, 100, GREEN, 2);
    recUI(150, 160, 100, 100, YELLOW, 2);
    recUI(160, 160, 100, 100, ORANGE, 2);
    UIobject* drawer = drawerUI(WIDTH - 50, HEIGHT / 2, 50, HEIGHT, RED, BLACK, 4);
    //drawerUI(100, 100, 100, 100, RED, GRAY, 4);
    buttonUI(20, -30, 40, 20, BLUE, LGRAYBLUE, 5, drawer);
    buttonUI(20, 0, 40, 20, BLUE, LGRAYBLUE, 5, drawer);
    buttonUI(20, 30, 40, 20, BLUE, LGRAYBLUE, 5, drawer);
    extern OscData oscData;
    waveUI(YELLOW, 1, &oscData);
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

void processEvent()
{
    while (haveEvent())
    {
        Event event = dequeueEvent();
        // write your event process function
        
        UIobject *beforeCursor = NULL;
        if (eventCodeMask(event) == OnClick)
        {
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
                else{
                    cursor = pointer;
                    beforeCursor = beforepointer;
                }
            }
        }
        //if cursor has same priority with next, swap them until they have different priority
        //this make cursor the last object (among other same priority objects) that will be shaded last  
        while(cursor->next != NULL){
            if(cursor->priority != cursor->next->priority) break;
            //交换cursor和next
            UIobject *temp = cursor->next;
            cursor->next = temp->next;
            temp->next = cursor;
            beforeCursor->next = temp;
            beforeCursor = beforeCursor->next;
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
        if (pointer->update != NULL){
            pointer->update(pointer, deltaT);
            //make sure child objects are updated too
            //expecially when child position is relative to parent
            if(pointer->child != NULL){
                UIobject *child = pointer->child;
                do{
                    child->childUpdate(child, deltaT, pointer);
                    child = child->childNext;
                }while(child != NULL);
            }
        }
    }
}
// extern void graph();

void shadeUI()
{
    UIobject *pointer = getHead();
    while (pointer->next != NULL)
    {
        pointer = pointer->next;
        if (pointer->shader != NULL)
            pointer->shader(pointer);
        // HAL_Delay(100);      //debug
        // graph();
    }
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