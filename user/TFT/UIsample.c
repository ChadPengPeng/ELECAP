#include "UIsample.h"
// example of a cursor following rectangle
void grabCursor(UIobject *this, Event event)
{
    int cursorX = cursorXmask(event);
    int cursorY = cursorYmask(event);
    this->x = cursorX / 2 + this->x / 2;
    this->y = cursorY / 2 + this->y / 2;
    if (this->x + this->box[0][0] < 0)
        this->x = -this->box[0][0];
    if (this->x + this->box[0][1] > WIDTH - 1)
        this->x = WIDTH - this->box[0][1] - 1;
    if (this->y + this->box[1][0] < 0)
        this->y = -this->box[1][0];
    if (this->y + this->box[1][1] > HEIGHT - 1)
        this->y = HEIGHT - this->box[1][1] - 1;
}

void recShader(UIobject *this)
{
    u16 x, y, dx, dy, color;
    x = this->x;
    y = this->y;
    dx = this->param[0];
    dy = this->param[1];
    color = this->color;
    cacheLine(x - dx, y + dy, x + dx - 1, y + dy, color);
    cacheLine(x + dx, y + dy, x + dx, y - dy + 1, color);
    cacheLine(x + dx, y - dy, x - dx + 1, y - dy, color);
    cacheLine(x - dx, y - dy, x - dx, y + dy - 1, color);
}

void recUI(u16 centerx, u16 centery, u16 width, u16 height, u16 color, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx;
    result->y = centery;
    result->box[0][1] = result->param[0] = width / 2;
    result->box[0][0] = -result->param[0];
    result->box[1][1] = result->param[1] = height / 2;
    result->box[1][0] = -result->param[1];
    result->color = color;
    result->eventListener = grabCursor;
    result->shader = recShader;
    result->priority = priority;
    priorityInsert(result);
}

extern int deltaT;
extern EventBuffer eventBuffer;
extern u16 debug;
#include "touch.h"
float fpsN = 0;
void debugShader()
{
    // debug
    char fps[16], x[16], y[16], event[16];
    fpsN = (0.9f * fpsN + 100.0f / (float)deltaT);
    int node = 0;
    UIobject *head = getHead();
    while(head->next != NULL){
        head=head->next;
        node++;
    }
    sprintf(fps, "fps:%4.1f", fpsN);
    sprintf(x, "x:%.3d", 360 - tp_dev.Y / 23);
    sprintf(y, "y:%.3d", 256 - tp_dev.X / 30);
    sprintf(event, "debug:%2x", tp_dev.sta);
    cacheString(0, 0, 100, 100, 12, fps, 0xaf7d);
    cacheString(0, 16, 100, 100, 12, x, 0xaf7d);
    cacheString(0, 32, 100, 100, 12, y, 0xaf7d);
    cacheString(0, 48, 100, 100, 12, event, 0xaf7d);
}

void debugUI()
{
    UIobject *result = getUIobject();
    result->priority = 255;
    result->shader = debugShader;
    priorityInsert(result);
}

// extern int wave[300];
// void oscShader(UIobject* this){
//     for (int i){
//         cachePoint(x+i,y+wave[i]);
//     }
// }

// void oscUI(){
//     UIobject* result = getUIobject();
//     result->x=0;
//     result->y=0;
//     result->shader=oscShader;
//     return result;
// }
void backgroundShader(UIobject *this)
{
    memset(frameCache, 0x0000, sizeof(frameCache));
}
void backgroundUI()
{
    UIobject *background = getUIobject();
    background->x = 0;
    background->y = 0;
    background->box[0][1] = WIDTH;
    background->box[0][0] = 0;
    background->box[1][1] = HEIGHT;
    background->box[1][0] = 0;
    background->priority = 0;
    background->shader = backgroundShader;
    priorityInsert(background);
}