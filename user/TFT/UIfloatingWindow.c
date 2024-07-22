#include "UIfloatingWindow.h"

#include "UImessage.h"
#define WaitingTick 1000
static int lastTick = 0;

//update function
void onWaitingUpdate(UIobject *this, int deltaT);
void floatingOnFoldingUpdate(UIobject *this, int deltaT);

void resetFloatingWindowCounter(){
    lastTick = HAL_GetTick();
}
void onWaitingUpdate(UIobject *this, int deltaT){
    if((int)(HAL_GetTick() - lastTick) > WaitingTick){
        this->update = floatingOnFoldingUpdate;
        this->param[3] = FOLD;
    }
}


void floatingOnFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(300, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->param[0] == 0 && this->param[1] == 0)
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
        this->param[0] = approachDiv(this->param[0], 0, div) ;
        this->param[1] = approachDiv(this->param[1], 0, div) ;
        this->color = approachColorDiv(this->color, BLACK, div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->param[0] == this->param[4] && this->param[1] == this->param[5])
        {
            this->update = onWaitingUpdate;
            resetFloatingWindowCounter();
            this->box[0][0] = -this->param[0] / 2;
            this->box[0][1] = this->param[0] / 2;
            this->box[1][0] = -this->param[1] / 2;
            this->box[1][1] = this->param[1] / 2;
            this->color = this->param[6];
            return;
        }
        this->param[0] = approachDiv(this->param[0], this->param[4], div) ;
        this->param[1] = approachDiv(this->param[1], this->param[5], div) ;
        this->color = approachColorDiv(this->color, this->param[6], div);
    }
}



void floatingShader(UIobject *this)
{
    cacheRoundedRecBackground(this->x, this->y, this->param[0], this->param[1], this->param[1]/2, this->color, this->param[2]);
    cacheOneCenter(this->x, this->y, 12, (char*)(this->selfStruct), this->color);
}
/*
param:
    0:width
    1:heigt
    2:backgroudColor
    3:ifUnfold
    4:initWidth
    5:initHeight
    6:initColor
    7:message
*/
UIobject *thisFloatingWindow;
UIobject *flotingUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx;
    result->y = centery;
    result->param[0] = 0;
    result->param[1] = 0;
    result->box[0][0] = 0;
    result->box[0][1] = 0;
    result->box[1][0] = 0;
    result->box[1][1] = 0;
    result->color = BLACK;
    result->param[2] = backgroudColor;
    result->param[3] = FOLD;
    result->param[4] = width;
    result->param[5] = height;
    result->param[6] = color;
    result->priority = priority;
    priorityInsert(result);
    thisFloatingWindow = result;
    return result;
}

void floatingMessage(char *message)
{
    //make window have the amimation effect of unfolding
    thisFloatingWindow->param[0] = 0;
    thisFloatingWindow->param[1] = 0;
    thisFloatingWindow->color = BLACK;
    updateMessage(message);
}

void updateMessage(char *message)
{
    thisFloatingWindow->shader = floatingShader;
    thisFloatingWindow->selfStruct = message;
    thisFloatingWindow->update = floatingOnFoldingUpdate;
    thisFloatingWindow->param[3] = UNFOLD;
    thisFloatingWindow->param[5] = 12+12*getRow(thisFloatingWindow->x, thisFloatingWindow->y, thisFloatingWindow->param[4] - thisFloatingWindow->param[5], 12, message);
}