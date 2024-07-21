#include "UIfloatingWindow.h"

#include "UImessage.h"
#define WaitingTick 1000
static int lastTick = 0;

//update function
void onWaitingUpdate(UIobject *this, int deltaT);
void floatingOnFoldingUpdate(UIobject *this, int deltaT);


void onWaitingUpdate(UIobject *this, int deltaT){
    if(lastTick - HAL_GetTick() > WaitingTick){
        this->update = floatingOnFoldingUpdate;
        this->param[3] = FOLD;
    }
}


void floatingOnFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(300, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->param[0] == 0)
        {
            this->update = NULL;
            this->shader = NULL;
            this->param[6] = BLACK;
            this->box[0][0] = 0;
            this->box[0][1] = 0;
            this->box[1][0] = 0;
            this->box[1][1] = 0;
            return;
        }
        this->param[0] = approachDiv(this->param[0], 0, div) ;
        this->param[1] = approachDiv(this->param[1], 0, div) ;
        this->color = FadeColor(this->color, 255*(div-1)/div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->param[0] == this->param[4])
        {
            this->update = onWaitingUpdate;
            this->param[6] = WHITE;
            this->box[0][0] = -this->param[0] / 2;
            this->box[0][1] = this->param[0] / 2;
            this->box[1][0] = -this->param[1] / 2;
            this->box[1][1] = this->param[1] / 2;
            return;
        }
        this->param[0] = approachDiv(this->param[0], this->param[4], div) ;
        this->param[1] = approachDiv(this->param[1], this->param[5], div) ;
        this->color = FadeColor(color, 255*(div-1)/div) + FadeColor(this->param[6] , 255/div);
    }
}



void floatingShader(UIobject *this)
{
    cacheRoundedRecBackground(this->x, this->y, this->param[0], this->param[1], this->param[1]/2, this->color, this->param[2]);
    cacheOneCenter(this->x, this->y, 12, (char*)this->param[7], this->color);
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
UIobject *flotingUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx;
    result->y = centery;
    result->param[0] = width;
    result->param[1] = height;
    result->box[0][0] = 0;
    result->box[0][1] = 0;
    result->box[1][0] = 0;
    result->box[1][1] = 0;
    result->color = color;
    result->param[2] = backgroudColor;
    result->param[3] = FOLD;
    result->param[4] = result->param[0];
    result->param[5] = result->param[1];
    result->param[6] = color;
    result->priority = priority;
    priorityInsert(result);
    return result;
}

void floatingMessage(UIobject *this, char *message)
{
    this->shader = floatingShader;
    this->param[7] = message;
}