#include "UIbutton.h"

#define FoldPixel 80

void buttonOnUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(500, deltaT);
    if (this->param[2] != BLACK)
        this->param[2] = approachColorDiv(this->param[2], BLACK, div);
    else
        this->update = NULL;
}

void buttonOnClick(UIobject *this, Event event)
{
    int div = getDiv(500, 10);
    this->param[2] = approachColorDiv(this->param[2], this->param[6], div);
    if (stateMask(event) == OnClick)
    {
        floatingMessage("Button Clicked!");
    }
    if (stateMask(event) == HoldEnd)
    {
        this->update = buttonOnUpdate;
    }
}

void buttonShader(UIobject *this)
{
    cacheCenterRec(this->x, this->y, this->width, this->height, this->color);
    cacheCenterBlock(this->x, this->y, this->width - 2, this->height - 2, this->param[2]);
}
/*
param:
    2:backgroudColor
    5:callbackHandle
    6:initBackgroudColor
selfStruct:nextButton
*/
UIobject *buttonUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *this = getUIobject();
    this->x = centerx;
    this->y = centery;
    this->relativeX = centerx;
    this->relativeY = centery;
    this->box[0][0] = -width / 2;
    this->box[0][1] = width / 2;
    this->box[1][0] = -height / 2;
    this->box[1][1] = height / 2;
    this->width = width;
    this->height = height;
    this->color = color;
    this->param[2] = BLACK;
    // this->param[5] = buttonCallback;
    this->param[6] = backgroudColor;
    this->eventListener = buttonOnClick;
    this->update = NULL;
    this->shader = buttonShader;
    this->priority = priority;
    priorityInsert(this);
    return this;
}

/*
    slider!!!
*/
#include "touchEvent.h"

void sliderUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(500, deltaT);
    if (this->relativeY == this->aimY)
        this->update = NULL;
    else
        this->relativeY = approachDiv(this->relativeY, this->aimY, div);
}

void sliderEvent(UIobject *this, Event event)
{
    static short lastY = 0;
    int div = getDiv(500, 10);

    if (eventCodeMask(event) == Touch)
    {
        short cursorX = cursorXmask(event);
        short cursorY = cursorYmask(event);
        if (stateMask(event) == OnClick)
        {
            lastY = this->relativeY;
        }
        this->aimY = lastY + cursorY - touchingParam.clickY;
        if (this->child != NULL){
            UIobject *child = this->child;
            short childYMax = child->relativeY + child->box[1][1];
            short childYMin = child->relativeY + child->box[1][0];
            while(child->childNext!= NULL){
                child = child->childNext;

                if (child->relativeY + child->box[1][1] > childYMax)
                    childYMax = child->relativeY + child->box[1][1];
                if (child->relativeY + child->box[1][0] < childYMin)
                    childYMin = child->relativeY + child->box[1][0];
            }
            this->aimY = constrain(this->aimY, - childYMax, - childYMin);
        }
        this->update = sliderUpdate;
    }
}

/*
param:
*/
UIobject *sliderUI(short centerx, short centery, short width, short height, int priority)
{
    UIobject *this = getUIobject();
    this->x = centerx;
    this->y = centery;
    this->relativeX = centerx;
    this->relativeY = centery;
    this->box[0][0] = -width / 2;
    this->box[0][1] = width / 2;
    this->box[1][0] = -height / 2;
    this->box[1][1] = height / 2;
    this->aimX = centerx;
    this->aimY = centerx;
    this->eventListener = sliderEvent;
    this->priority = priority;
    priorityInsert(this);
    return this;
}

void setSliderY(UIobject *this, short y)
{
    this->aimY = y;
    this->update = sliderUpdate;
}

/*
drawer!!!


*/

void onFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(500, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->x == this->aimX)
        {
            this->update = NULL;
            return;
        }
        this->x = approachDiv(this->x, this->aimX, div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->x == this->aimX - FoldPixel)
        {
            this->update = NULL;
            return;
        }
        this->x = approachDiv(this->x, this->aimX - FoldPixel, div);
    }
}

void onCursor(UIobject *this, Event event)
{

    if (stateMask(event) == OnClick)
    {
        this->param[3] = !this->param[3];
        this->update = onFoldingUpdate;
    }
}

void drawerShader(UIobject *this)
{
    cacheRoundedBackgroundRight(this->x, this->y, this->width, this->height, 15, this->color, this->param[2]);
}
/*
param:
    2:backgroudColor
    3:ifUnfold
    6:initBackgroudColor
*/
UIobject *drawerUI(short centerx, short centery, short width, short height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *this = getUIobject();
    this->x = centerx;
    this->y = centery;
    this->box[0][0] = -width / 2;
    this->box[0][1] = width / 2;
    this->box[1][0] = -height / 2;
    this->box[1][1] = height / 2;
    this->width = width;
    this->height = height;
    this->color = color;
    this->param[2] = backgroudColor;
    this->param[3] = FOLD;
    this->aimX = centerx;
    this->aimY = centery;
    this->param[6] = backgroudColor;
    this->eventListener = onCursor;
    this->shader = drawerShader;
    this->priority = priority;
    priorityInsert(this);
    return this;
}
