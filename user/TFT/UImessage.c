#include "UImessage.h"

#define div 5

void messageOnFoldingUpdate(UIobject *this, int deltaT)
{
    if (this->param[3] == FOLD)
    {
        if (this->param[0] == 0)
        {
            this->update = NULL;
            return;
        }
        this->param[0] = this->param[0] * 4 / 5 ;
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->param[0] == this->param[4])
        {
            this->update = NULL;
            return;
        }
        this->param[0] = this->param[0] * (div - 1) / div + this->param[4] / div;
    }
}

void messageOnCursor(UIobject *this, Event event)
{

    if (eventCodeMask(event) == OnClick)
    {
        this->param[3] = !this->param[3];
        this->update = messageOnFoldingUpdate;
    }
}

void messageShader(UIobject *this)
{
    cacheRoundedRecBackground(this->x, this->y, this->param[0], this->param[1], 15, this->color, this->param[2]);
}
/*
param:
    0:width
    1:heigt
    2:backgroudColor
    3:ifUnfold
    4:initWidth
    5:initHeight
    6:initBackgroudColor
*/
UIobject *messageUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx;
    result->y = centery;
    result->box[0][1] = result->param[0] = width / 2;
    result->box[0][0] = -result->param[0];
    result->box[1][1] = result->param[1] = height / 2;
    result->box[1][0] = -result->param[1];
    result->color = color;
    result->param[2] = backgroudColor;
    result->param[3] = FOLD;
    result->param[4] = width;
    result->param[5] = height;
    result->param[6] = backgroudColor;
    result->eventListener = messageOnCursor;
    result->shader = messageShader;
    result->priority = priority;
    priorityInsert(result);
    return result;
}