#include "UImessage.h"

void messageOnFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(300, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->param[0] == this->param[4] / 5)
        {
            this->update = NULL;
            this->shader = NULL;
            this->param[6] = BLACK;
            this->box[0][0] = -this->param[0] / 2;
            this->box[0][1] = this->param[0] / 2;
            this->box[1][0] = -this->param[1] / 2;
            this->box[1][1] = this->param[1] / 2;
            return;
        }
        this->param[0] = approachDiv(this->param[0], this->param[4] / 5, div);
        this->param[1] = approachDiv(this->param[1], this->param[5] / 5, div);
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
        this->param[0] = approachDiv(this->param[0], this->param[4], div);
        this->param[1] = approachDiv(this->param[1], this->param[5], div);
        this->param[6] = approachColorDiv(this->param[6], WHITE, div);
    }
}

#include "osc.h"
#include "stdio.h"
extern OscData *thisOsc;
void messageShader(UIobject *this)
{
    cacheRoundedRecBackground(this->x, this->y, this->param[0], this->param[1], this->param[1] / 2, this->color, this->param[2]);
    char message[64];
    sprintf(message, "max: %4.2f", toVoltage(thisOsc->max, thisOsc));
    cacheOneCenter(this->x - 60, this->y - 12, 12, message, this->param[6]);
    sprintf(message, "min: %4.2f", toVoltage(thisOsc->min, thisOsc));
    cacheOneCenter(this->x + 60, this->y - 12, 12, message, this->param[6]);
    sprintf(message, "avg: %4.2f", toVoltage(thisOsc->avg, thisOsc));
    cacheOneCenter(this->x, this->y - 12, 12, message, this->param[6]);
    sprintf(message, "freq: %d", thisOsc->freq);
    cacheOneCenter(this->x, this->y + 12, 12, message, this->param[6]);
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
    result->param[3] = UNFOLD;
    result->param[4] = result->param[0];
    result->param[5] = result->param[1];
    result->param[6] = WHITE;
    result->eventListener = messageOnCursor;
    result->shader = messageShader;
    result->priority = priority;
    priorityInsert(result);
    return result;
}