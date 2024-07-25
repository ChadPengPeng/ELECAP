#include "UserInterface.h"

extern OscData *thisOsc;
char message[128];

extern OscData *thisOsc;

void freqButtonShader(UIobject *this);
void inputModeButtonShader(UIobject *this);
void triggerModeButtonShader(UIobject *this);
void freqButtonClick(UIobject *this, Event event);
void inputModeButtonClick(UIobject *this, Event event);
void triggerModeButtonClick(UIobject *this, Event event);

void OscDataMessageShader(UIobject *this);
void OscDataMessageOnCursor(UIobject *this, Event event);
void userInterface()
{
    flotingUI(WIDTH / 2, HEIGHT / 4, 100, 20, MacaronORANGE, BLACK, 254);
    // your init object
    // recUI(120, 160, 100, 100, BLUE, 2);
    // recUI(130, 160, 100, 100, PURPLE, 2);
    // recUI(140, 160, 100, 100, GREEN, 2);
    // recUI(150, 160, 100, 100, YELLOW, 2);
    // recUI(160, 160, 100, 100, ORANGE, 2);
    UIobject *drawer = drawerUI(WIDTH - 1, HEIGHT / 2, 30, HEIGHT - 1, MacaronGREEN, BLACK, 4);

    UIobject *slider = sliderUI(50, 0, 50, HEIGHT - 1, 5);
    childInsert(drawer, slider);
    // drawerUI(100, 100, 100, 100, RED, GRAY, 4);
    UIobject *freqButton = buttonUI(0, -50, 60, 36, MacaronBLUE, BLUE, 6);
    UIobject *inputModeButton = buttonUI(0, 0, 60, 36, MacaronBLUE, BLUE, 7);
    UIobject *triggerModeButton = buttonUI(0, 50, 60, 36, MacaronBLUE, BLUE, 8);
    freqButton->shader = freqButtonShader;
    inputModeButton->shader = inputModeButtonShader;
    triggerModeButton->shader = triggerModeButtonShader;
    freqButton->eventListener = freqButtonClick;
    inputModeButton->eventListener = inputModeButtonClick;
    triggerModeButton->eventListener = triggerModeButtonClick;
    childInsert(slider, freqButton);
    childInsert(slider, inputModeButton);
    childInsert(slider, triggerModeButton);
    UIobject *oscDataMessage = messageUI(WIDTH / 2, HEIGHT * 3 / 4, WIDTH * 3 / 4, HEIGHT / 4, MacaronWHITE, BLACK, 254);
    oscDataMessage->shader = OscDataMessageShader;
    oscDataMessage->eventListener = OscDataMessageOnCursor;
    waveUI(MacaronYELLOW, MacaronPINK, 1, thisOsc);
}

void buttonOnCenter(UIobject *this)
{
    setSliderY(this->father, -this->relativeY);
}

void freqButtonShader(UIobject *this)
{
    cacheCenterRec(this->x, this->y, this->width, this->height, this->color);
    // cacheCenterBlock(this->x, this->y, this->param[0] - 1, this->param[1] - 1, this->param[2]);
    //real freq
    sprintf(message, "freq:\n %d", adcTIMfreq/(TIM6->ARR+1));
    //sprintf(message, "freq:\n %d", thisOsc->xScale);
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}

void inputModeButtonShader(UIobject *this)
{
    cacheCenterRec(this->x, this->y, this->width, this->height, this->color);
    // cacheCenterBlock(this->x, this->y, this->param[0] - 1, this->param[1] - 1, this->param[2]);
    if (thisOsc->inputMode == AC)
        sprintf(message, "input mode: AC");
    else if (thisOsc->inputMode == DC)
        sprintf(message, "input mode: DC");
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}

void triggerModeButtonShader(UIobject *this)
{
    cacheCenterRec(this->x, this->y, this->width, this->height, this->color);
    // cacheCenterBlock(this->x, this->y, this->param[0] - 1, this->param[1] - 1, this->param[2]);
    if (thisOsc->triggerMode == Auto)
        sprintf(message, "trigger mode: Auto");
    else if (thisOsc->triggerMode == Manual)
        sprintf(message, "trigger mode: Manual");
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}

void freqButtonClick(UIobject *this, Event event)
{
    if (eventCodeMask(event) == Select)
        buttonOnCenter(this);
    else if (stateMask(event) != OnClick)
        return;
    int add = 0;
    if (eventCodeMask(event) == Touch)
    {
        if (cursorXmask(event) < this->x)
            add = -1;
        else
            add = 1;
    }
    else if (eventCodeMask(event) >= KEY1)
    {
        switch (eventCodeMask(event))
        {
        case KEY2:
            getKeyBind();
            break;
        case KEY4:
            releaseKeyBind();
            break;

        case KEY1:
            if (ifKeyBindOnObject)
                add = 1;
            break;
        case KEY3:
            if (ifKeyBindOnObject)
                add = -1;
            break;
        default:
            break;
        }
    }
    int freq = thisOsc->xScale;
    if (add == 1)
        freq = highDigitPlusOne(freq);
    else if (add == -1)
        freq = highDigitMinusOne(freq);
    freq = constrain(freq, 50000, 1000000);
    setXscale(thisOsc, freq);
}

void inputModeButtonClick(UIobject *this, Event event)
{
    if (eventCodeMask(event) == Select)
        buttonOnCenter(this);
    else if (stateMask(event) == OnClick)
    {
        if (thisOsc->inputMode == AC)
            thisOsc->inputMode = DC;
        else
            thisOsc->inputMode = AC;
    }
}

void triggerModeButtonClick(UIobject *this, Event event)
{
    if (eventCodeMask(event) == Select)
        buttonOnCenter(this);
    else if (stateMask(event) == OnClick)
    {
        if (thisOsc->triggerMode == Auto)
            thisOsc->triggerMode = Manual;
        else
            thisOsc->triggerMode = Auto;
    }
}

extern void messageShader(UIobject *this);
void OscDataMessageShader(UIobject *this)
{
    messageShader(this);
    sprintf(message, "max: %4.2f", toVoltage(thisOsc->max, thisOsc));
    cacheOneCenter(this->x - 60, this->y - 12, 12, message, this->param[6]);
    sprintf(message, "min: %4.2f", toVoltage(thisOsc->min, thisOsc));
    cacheOneCenter(this->x + 60, this->y - 12, 12, message, this->param[6]);
    sprintf(message, "avg: %4.2f", toVoltage(thisOsc->avg, thisOsc));
    cacheOneCenter(this->x, this->y - 12, 12, message, this->param[6]);
    sprintf(message, "freq: %d", thisOsc->freq);
    cacheOneCenter(this->x, this->y + 12, 12, message, this->param[6]);
}

extern void messageOnCursor(UIobject *this, Event event);
void OscDataMessageOnCursor(UIobject *this, Event event)
{
    messageOnCursor(this, event);
    if (this->shader == messageShader)
        this->shader = OscDataMessageShader;
}