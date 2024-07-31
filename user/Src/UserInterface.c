#include "UserInterface.h"

#define CH0 AD9959_CHANNEL_0
#define CH1 AD9959_CHANNEL_1
#define CH2 AD9959_CHANNEL_2
#define CH3 AD9959_CHANNEL_3


char message[64];
// implement your own UI
uint8_t selectChannel = CH0;
DDS_Config *selectDDS = NULL;

void channelButtonClick(UIobject *this, Event event);
void freqButtonClick(UIobject *this, Event event);
void ampButtonClick(UIobject *this, Event event);
void phaseButtonClick(UIobject *this, Event event);
void channelButtonShader(UIobject *this);
void freqButtonShader(UIobject *this);
void ampButtonShader(UIobject *this);
void phaseButtonShader(UIobject *this);

void userInterface()
{
    selectDDS = DDS_Init(CH0, 2000000, 512, 0);
    DDS_Init(CH1, 20000000, 512, 0);
    DDS_Init(CH2, 2000000, 512, 0);
    DDS_Init(CH3, 20000000, 512, 0);

    flotingUI(WIDTH / 2, HEIGHT / 4, 100, 20, MacaronORANGE, BLACK, 254);

    //UIobject *drawer = drawerUI(WIDTH - 1, HEIGHT / 2, 30, HEIGHT - 1, MacaronGREEN, BLACK, 4);

    //UIobject *slider = sliderUI(50, 0, 50, HEIGHT - 1, 5);
    //childInsert(drawer, slider);
    // drawerUI(100, 100, 100, 100, RED, GRAY, 4);
    UIobject *channelButton = buttonUI(WIDTH/2, 40, 60, 36, MacaronBLUE, BLUE, 6);
    UIobject *freqButton = buttonUI(WIDTH/2, 80, 60, 36, MacaronBLUE, BLUE, 7);
    UIobject *ampButton = buttonUI(WIDTH/2, 120, 60, 36, MacaronBLUE, BLUE, 8);
    UIobject *phaseButton = buttonUI(WIDTH/2, 160, 60, 36, MacaronBLUE, BLUE, 9);
    channelButton->eventListener = channelButtonClick;
    freqButton->eventListener = freqButtonClick;
    ampButton->eventListener = ampButtonClick;
    phaseButton->eventListener = phaseButtonClick;
    channelButton->shader = channelButtonShader;
    freqButton->shader = freqButtonShader;
    ampButton->shader = ampButtonShader;
    phaseButton->shader = phaseButtonShader;
    //childInsert(slider, channelButton);
    //childInsert(slider, freqButton);
    //childInsert(slider, ampButton);
    //childInsert(slider, phaseButton);
}

// extern void sliderUpdate(UIobject *this, int deltaT);

// void buttonOnSelect(UIobject *this)
// {
//     this->father->aimY = -this->relativeY;
//     this->father->update = sliderUpdate;
// }

int intValueButtonClick(UIobject *this, Event event)
{
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
    return add;
}

void channelButtonClick(UIobject *this, Event event)
{
    if (stateMask(event) == OnClick)
    {
        int add = intValueButtonClick(this, event);
        if (add == 1)
        {
            switch (selectChannel)
            {
            case CH0:
                selectChannel = CH1;
                break;

            case CH1:
                selectChannel = CH2;
                break;

            case CH2:
                selectChannel = CH3;
                break;

            case CH3:
                selectChannel = CH0;
                break;

            default:
                break;
            }
        }

        if (add == -1)
        {
            switch (selectChannel)
            {
            case CH0:
                selectChannel = CH3;
                break;

            case CH1:
                selectChannel = CH0;
                break;

            case CH2:
                selectChannel = CH1;
                break;

            case CH3:
                selectChannel = CH2;
                break;

            default:
                break;
            }
        }
        selectDDS = get_DDS(selectChannel);
    }
}

void freqButtonClick(UIobject *this, Event event)
{
    if (stateMask(event) == OnClick)
    {
        int add = intValueButtonClick(this, event);
        if (add == 1)
        {
            set_freq(selectDDS, selectDDS->freq + 2000000);
        }
        else if (add == -1)
        {
            set_freq(selectDDS, selectDDS->freq - 2000000);
        }
    }
}

void ampButtonClick(UIobject *this, Event event)
{
    if (stateMask(event) == OnClick)
    {
        int add = intValueButtonClick(this, event);
        if (add == 1)
        {
            set_amp(selectDDS, selectDDS->amp + 51);
        }
        else if (add == -1)
        {
            set_amp(selectDDS, selectDDS->amp - 51);
        }
    }
}

void phaseButtonClick(UIobject *this, Event event)
{
    if (stateMask(event) == OnClick)
    {
        int add = intValueButtonClick(this, event);
        if (add == 1)
        {
            set_phase(selectDDS, selectDDS->phase + 45);
        }
        else if (add == -1)
        {
            set_phase(selectDDS, selectDDS->phase - 45);
        }
    }
}

extern void buttonShader(UIobject *this);
void channelButtonShader(UIobject *this)
{
    buttonShader(this);
    switch (selectChannel)
    {
    case CH0:
        sprintf(message, "CH0"); break;

    case CH1:
        sprintf(message, "CH1"); break;

    case CH2:
        sprintf(message, "CH2"); break;

    case CH3:
        sprintf(message, "CH3"); break;
    default:
        break;
    }
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}

void freqButtonShader(UIobject *this)
{
    buttonShader(this);
    sprintf(message, "Freq:%dMHz", selectDDS->freq / 1000000);
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}

void ampButtonShader(UIobject *this)
{
    buttonShader(this);
    sprintf(message, "Amp:%d", selectDDS->amp);
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}


void phaseButtonShader(UIobject *this)
{
    buttonShader(this);
    sprintf(message, "Phase:%d", selectDDS->phase);
    cacheCenterString(this->x, this->y, this->width, this->height, 12, message, this->color);
}