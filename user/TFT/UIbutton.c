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

void buttonOnParentUpdate(UIobject *this, int deltaT, UIobject *parent)
{
    this->x = parent->x + (short)this->param[3];
    this->y = parent->y + (short)this->param[4];
}

void buttonOnClick(UIobject *this, Event event)
{
    int div = getDiv(500, 10);
    this->param[2] = approachColorDiv(this->param[2], this->param[6], div);
    if (stateMask(event) == OnClick)
    {
        if (this->selfStruct != NULL)
            ((void (*)(void))this->selfStruct)();
    }
    if (stateMask(event) == HoldEnd)
    {
        this->update = buttonOnUpdate;
    }
}

void warningButtonselfStruct()
{
    extern void floatingMessage(char *message);
    floatingMessage("Button Clicked!");
}

void buttonShader(UIobject *this)
{
    cacheCenterRec(this->x, this->y, this->param[0], this->param[1], this->color);
    cacheCenterBlock(this->x, this->y, this->param[0] - 1, this->param[1] - 1, this->param[2]);
}
/*
param:
    0:halfWidth
    1:halfHeight
    2:backgroudColor
    3:relativeX
    4:relativeY
    5:callbackHandle
    6:initBackgroudColor
selfStruct:nextButton
*/
UIobject *buttonUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority, UIobject *father, void *functionHandle)
{
    UIobject *result = getUIobject();
    result->x = centerx + father->x;
    result->y = centery + father->y;
    result->box[0][1] = result->param[0] = width / 2;
    result->box[0][0] = -result->param[0];
    result->box[1][1] = result->param[1] = height / 2;
    result->box[1][0] = -result->param[1];
    result->color = color;
    result->param[2] = BLACK;
    result->param[3] = centerx;
    result->param[4] = centery;
    // result->param[5] = buttonCallback;
    result->param[6] = backgroudColor;
    result->eventListener = buttonOnClick;
    result->update = NULL;
    result->childUpdate = buttonOnParentUpdate;
    result->shader = buttonShader;
    result->priority = priority;
    // to save space in struct
    if (functionHandle != NULL)
        result->selfStruct = functionHandle;
    else
        result->selfStruct = warningButtonselfStruct;
    priorityInsert(result);
    childInsert(father, result);
    return result;
}

/*
drawer!!!


*/

void onFoldingUpdate(UIobject *this, int deltaT)
{
    int div = getDiv(500, deltaT);
    if (this->param[3] == FOLD)
    {
        if (this->x == this->param[4])
        {
            this->update = NULL;
            return;
        }
        this->x = approachDiv(this->x, this->param[4], div);
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->x == this->param[4] - FoldPixel)
        {
            this->update = NULL;
            return;
        }
        this->x = approachDiv(this->x, this->param[4] - FoldPixel, div);
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
    cacheRoundedBackgroundRight(this->x, this->y, this->param[0], this->param[1], 15, this->color, this->param[2]);
}
/*
param:
    0:width
    1:heigt9
    2:backgroudColor
    3:ifUnfold
    4:initX
    5:initY
    6:initBackgroudColor
*/
UIobject *drawerUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx;
    result->y = centery;
    result->box[0][0] = -width / 2;
    result->box[0][1] = width / 2;
    result->box[1][0] = -height / 2;
    result->box[1][1] = height / 2;
    result->param[0] = width;
    result->param[1] = height;
    result->color = color;
    result->param[2] = backgroudColor;
    result->param[3] = FOLD;
    result->param[4] = centerx;
    result->param[5] = centery;
    result->param[6] = backgroudColor;
    result->eventListener = onCursor;
    result->shader = drawerShader;
    result->priority = priority;
    priorityInsert(result);
    return result;
}