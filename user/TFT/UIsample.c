#include "UIsample.h"
// example of a cursor following rectangle
void grabCursor(UIobject *this, Event event)
{
    short cursorX = cursorXmask(event);
    short cursorY = cursorYmask(event);
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
    dx = this->width;
    dy = this->height;
    color = this->color;
    cacheLine(x - dx, y + dy, x + dx - 1, y + dy, color);
    cacheLine(x + dx, y + dy, x + dx, y - dy + 1, color);
    cacheLine(x + dx, y - dy, x - dx + 1, y - dy, color);
    cacheLine(x - dx, y - dy, x - dx, y + dy - 1, color);
}

void recUI(short centerx, short centery, short width, short height, u16 color, int priority)
{
    UIobject *this = getUIobject();
    this->x = centerx;
    this->y = centery;
    this->box[0][1] = this->width = width / 2;
    this->box[0][0] = -this->width;
    this->box[1][1] = this->height = height / 2;
    this->box[1][0] = -this->height;
    this->color = color;
    this->eventListener = grabCursor;
    this->shader = recShader;
    this->priority = priority;
    priorityInsert(this);
}

#include "touch.h"
extern int deltaT;
float fpsN = 0;
void debugShader()
{
    // debug
    char message[32];
    fpsN = (0.9f * fpsN + 100.0f / (float)deltaT);
    int node = 0;
    UIobject *head = getHead();
    while (head->next != NULL)
    {
        head = head->next;
        node++;
    }
    sprintf(message, "fps:%4.1f", fpsN);
    cacheString(0, 0, 100, 100, 12, message, 0xaf7d);
    sprintf(message, "x:%.3d", tp_dev.x[0]);
    cacheString(0, 16, 100, 100, 12, message, 0xaf7d);
    sprintf(message, "y:%.3d", tp_dev.y[0]);
    cacheString(0, 32, 100, 100, 12, message, 0xaf7d);
    extern UIobject *cursor;
    sprintf(message, "cursor:%#8x", (uint32_t)cursor);
    cacheString(0, 48, 100, 100, 12, message, 0xaf7d);
    sprintf(message, "event:%d", eventBuffer.head);
    cacheString(0, 64, 100, 100, 12, message, 0xaf7d);
    sprintf(message, "node:%d", node);
    cacheString(0, 80, 100, 100, 12, message, 0xaf7d);
    sprintf(message, "sta:%#hx", tp_dev.sta);
    cacheString(0, 96, 100, 100, 12, message, 0xaf7d);

    sprintf(message, "GPIOE1:%d", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1));
    cacheString(0, 104, 100, 100, 12, message, 0xaf7d);

    // extern float volIn;
    // sprintf(message, "volIn:%.3f", volIn);
    // cacheString(0, 112, 100, 100, 12, message, 0xaf7d);

    // extern float volOut;
    // sprintf(message, "volOut:%.3f", volOut);
    // cacheString(0, 128, 100, 100, 12, message, 0xaf7d);
}

void debugUI()
{
    UIobject *this = getUIobject();
    this->priority = 255;
    this->shader = debugShader;
    priorityInsert(this);
}

void backgroundShader(UIobject *this)
{
    memset(frameCache, 0x0000, frameCacheSize);
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