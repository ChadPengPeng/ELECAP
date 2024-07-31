#include "UIfloatingWindow.h"

#include "UImessage.h"
#define WaitingTick 1000
static int lastTick = 0;

// update function
void onWaitingUpdate(UIobject *this, int deltaT);
void floatingOnFoldingUpdate(UIobject *this, int deltaT);

void resetFloatingWindowCounter()
{
    lastTick = HAL_GetTick();
}
void onWaitingUpdate(UIobject *this, int deltaT)
{
    if ((int)(HAL_GetTick() - lastTick) > WaitingTick)
    {
        this->update = floatingOnFoldingUpdate;
        this->param[3] = FOLD;
    }
}

void floatingOnFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(300, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->width == 0 && this->height == 0)
        {
            this->update = NULL;
            this->shader = NULL;
            this->color = BLACK;
            this->box[0][0] = 0;
            this->box[0][1] = 0;
            this->box[1][0] = 0;
            this->box[1][1] = 0;
            return;
        }
        this->width = approachDiv(this->width, 0, div);
        this->height = approachDiv(this->height, 0, div);
        this->color = approachColorDiv(this->color, BLACK, div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->width == this->param[4] && this->height == this->param[5])
        {
            this->update = onWaitingUpdate;
            resetFloatingWindowCounter();
            this->box[0][0] = -this->width / 2;
            this->box[0][1] = this->width / 2;
            this->box[1][0] = -this->height / 2;
            this->box[1][1] = this->height / 2;
            this->color = this->param[6];
            return;
        }
        this->width = approachDiv(this->width, this->param[4], div);
        this->height = approachDiv(this->height, this->param[5], div);
        this->color = approachColorDiv(this->color, this->param[6], div);
    }
}

void floatingShader(UIobject *this)
{
    cacheRoundedRecBackground(this->x, this->y, this->width, this->height, 12, this->color, this->param[2]);
    cacheCenterString(this->x, this->y, this->param[4] - 24, this->param[5], 12, (char *)(this->selfStruct), this->color);
}
/*
param:
    2:backgroudColor
    3:ifUnfold
    4:initWidth
    5:initHeight
    6:initColor
    7:message
*/
UIobject *thisFloatingWindow;
UIobject *flotingUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *this = getUIobject();
    this->x = centerx;
    this->y = centery;
    this->width = 0;
    this->height = 0;
    this->box[0][0] = 0;
    this->box[0][1] = 0;
    this->box[1][0] = 0;
    this->box[1][1] = 0;
    this->color = BLACK;
    this->param[2] = backgroudColor;
    this->param[3] = FOLD;
    this->param[4] = width;
    this->param[5] = height;
    this->param[6] = color;
    this->priority = priority;
    priorityInsert(this);
    thisFloatingWindow = this;
    return this;
}

void floatingMessage(char *message)
{
    if (thisFloatingWindow == NULL)
    {
        return;
    }
    // make window have the amimation effect of unfolding
    thisFloatingWindow->width = 0;
    thisFloatingWindow->height = 0;
    thisFloatingWindow->color = BLACK;
    updateMessage(message);
}
char floatMessage[64];
void updateMessage(char *message)
{
    if (thisFloatingWindow == NULL)
    {
        return;
    }
    thisFloatingWindow->shader = floatingShader;
    thisFloatingWindow->selfStruct = message;
    thisFloatingWindow->update = floatingOnFoldingUpdate;
    thisFloatingWindow->param[3] = UNFOLD;
    thisFloatingWindow->param[5] = 24 + 12 * getRow(thisFloatingWindow->x, thisFloatingWindow->y, thisFloatingWindow->param[4] - 24, 12, message);
}