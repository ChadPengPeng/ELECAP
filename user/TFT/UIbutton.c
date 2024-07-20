#include "UIbutton.h"

#define FoldPixel 40
UIobject *drawer = NULL;
enum FOLDSTATE
{
    FOLD,
    UNFOLD
};

#define div 5

void buttonOnUpdate(UIobject *this, int deltaT)
{
    this->param[2] = FadeColor(this->param[2], 255 * (div - 1) / div);
}

void buttonCallback(UIobject *this, int x, int y)
{
    this->x = x + (short)this->param[3];
    this->y = y + (short)this->param[4];
}

void buttonOnClick(UIobject *this, Event event)
{
    this->param[2] = FadeColor(this->param[2], 255 * (div - 1) / div) + FadeColor(this->param[6], 255 / div);
    if (eventCodeMask(event) == OnClick)
    {
        // todo:your button
    }
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
void buttonUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
{
    UIobject *result = getUIobject();
    result->x = centerx + drawer->x;
    result->y = centery + drawer->y;
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
    result->update = buttonOnUpdate;
    result->shader = buttonShader;
    result->priority = priority;
    UIobject *button = drawer;
    if (button == NULL) return;
    while (button->selfStruct != NULL)
    {
        button = (UIobject *)button->selfStruct;
    }
    button->selfStruct = (void *)result;
    priorityInsert(result);
}

/*
drawer!!!


*/

void onFoldingUpdate(UIobject *this, int deltaT)
{
    if (this->param[3] == FOLD)
    {
        if (this->x == this->param[4])
        {
            this->update = NULL;
            return;
        }
        this->x = this->x * 4 / 5 + this->param[4] / 5;
    }
    if (this->param[3] == UNFOLD)
    {
        if (this->x == this->param[4] - FoldPixel)
        {
            this->update = NULL;
            return;
        }
        this->x = this->x * (div - 1) / div + (this->param[4] - FoldPixel) / div;
    }
    UIobject *button = (UIobject *)this->selfStruct;
    while (button != NULL)
    {
        buttonCallback(button, this->x, this->y);
        button = (UIobject *)button->selfStruct;
    }
}

void onCursor(UIobject *this, Event event)
{

    if (eventCodeMask(event) == OnClick)
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
    1:heigt
    2:backgroudColor
    3:ifUnfold
    4:initX
    5:initY
    6:initBackgroudColor
*/
void drawerUI(int centerx, int centery, int width, int height, u16 color, u16 backgroudColor, int priority)
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
    result->param[4] = centerx;
    result->param[5] = centery;
    result->param[6] = backgroudColor;
    result->eventListener = onCursor;
    result->shader = drawerShader;
    result->priority = priority;
    drawer = result;
    priorityInsert(result);
}