#include "UImessage.h"


void messageOnFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(300, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->param[0] == this->param[4]/5)
        {
            this->update = NULL;
            this->param[6] = BLACK;
            this->box[0][0] = -this->param[0] / 2;
            this->box[0][1] = this->param[0] / 2;
            this->box[1][0] = -this->param[1] / 2;
            this->box[1][1] = this->param[1] / 2;
            return;
        }
        this->param[0] = approachDiv(this->param[0], this->param[4]/5, div) ;
        this->param[1] = approachDiv(this->param[1], this->param[5]/5, div) ;
        this->param[6] = approachColorDiv(this->param[6], BLACK, div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->param[0] == this->param[4])
        {
            this->update = NULL;
            this->param[6] = WHITE;
            this->box[0][0] = -this->param[0] / 2;
            this->box[0][1] = this->param[0] / 2;
            this->box[1][0] = -this->param[1] / 2;
            this->box[1][1] = this->param[1] / 2;
            return;
        }
        this->param[0] = approachDiv(this->param[0], this->param[4], div) ;
        this->param[1] = approachDiv(this->param[1], this->param[5], div) ;
        this->param[6] = approachColorDiv(this->param[6], WHITE, div);
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
    cacheRoundedRecBackground(this->x, this->y, this->param[0], this->param[1], this->param[1]/2, this->color, this->param[2]);
    cacheOneCenter(this->x, this->y, 12, (char*)"hello world", this->param[6]);
}
/*
param:
    0:width
    1:heigt
    2:backgroudColor
    3:ifUnfold
    4:initWidth
    5:initHeight
    6:messageColor
*/
UIobject *messageUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx;
    result->y = centery;
    result->param[0] = width;
    result->param[1] = height;
    result->box[0][0] = -width / 2;
    result->box[0][1] = width / 2;
    result->box[1][0] = -height / 2;
    result->box[1][1] = height / 2;
    
    result->color = color;
    result->param[2] = backgroudColor;
    result->param[3] = FOLD;
    result->param[4] = result->param[0];
    result->param[5] = result->param[1];
    result->param[6] = WHITE;
    result->eventListener = messageOnCursor;
    result->shader = messageShader;
    result->priority = priority;
    priorityInsert(result);
    return result;
}