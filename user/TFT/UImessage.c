#include "UImessage.h"

void messageOnFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(300, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->width == this->param[4] / 5)
        {
            this->update = NULL;
            this->shader = NULL;
            this->param[6] = BLACK;
            this->box[0][0] = -this->width / 2;
            this->box[0][1] = this->width / 2;
            this->box[1][0] = -this->height / 2;
            this->box[1][1] = this->height / 2;
            return;
        }
        this->width = approachDiv(this->width, this->param[4] / 5, div);
        this->height = approachDiv(this->height, this->param[5] / 5, div);
        this->param[6] = approachColorDiv(this->param[6], BLACK, div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->width == this->param[4])
        {
            this->update = NULL;
            this->param[6] = WHITE;
            this->box[0][0] = -this->width / 2;
            this->box[0][1] = this->width / 2;
            this->box[1][0] = -this->height / 2;
            this->box[1][1] = this->height / 2;
            return;
        }
        this->width = approachDiv(this->width, this->param[4], div);
        this->height = approachDiv(this->height, this->param[5], div);
        this->param[6] = approachColorDiv(this->param[6], WHITE, div);
    }
}

void messageShader(UIobject *this)
{
    cacheRoundedRecBackground(this->x, this->y, this->width, this->height, this->height / 2, this->color, this->param[2]);
}

void messageOnCursor(UIobject *this, Event event)
{
    if (stateMask(event) == OnClick)
    {
        this->param[3] = !this->param[3];
        this->update = messageOnFoldingUpdate;
        this->shader = messageShader;
    }
}

/*
param:
    2:backgroudColor
    3:ifUnfold
    4:initWidth
    5:initHeight
    6:messageColor
*/
UIobject *messageUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *this = getUIobject();
    this->x = centerx;
    this->y = centery;
    this->width = width;
    this->height = height;
    this->box[0][0] = -width / 2;
    this->box[0][1] = width / 2;
    this->box[1][0] = -height / 2;
    this->box[1][1] = height / 2;
    this->color = color;
    this->param[2] = backgroudColor;
    this->param[3] = UNFOLD;
    this->param[4] = this->width;
    this->param[5] = this->height;
    this->param[6] = WHITE;
    this->eventListener = messageOnCursor;
    this->shader = messageShader;
    this->priority = priority;
    priorityInsert(this);
    return this;
}