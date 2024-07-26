#include "UIwave.h"

#define boundary 20
#define meshPixel 10
#define numPixel 50
#define dotLineInterval 3

// used for gesures processing
char gesureString[64];
#define gestureThreshold 10
#define zoomPixel 70
extern OscData *thisOsc;

void UIwaveEventlistener(UIobject *this, Event event)
{
    int direction;
    static int lastXbias;
    static int lastYbias;
    static int lastXdist;
    static int lastYdist;
    static int lastXscale;
    static int lastYscale;
    static int gestureValid = 0;
    State state = stateMask(event);
    
    if (state == OnClick)
    {
        lastXbias = thisOsc->xBias;
        lastYbias = thisOsc->yBias;
        gestureValid = 0;
        if ((touchingParam.multiTouchSta >> 1) & 0x01)
        {
            lastXdist = absM(touchingParam.xList[1] - touchingParam.xList[0]);
            lastYdist = absM(touchingParam.yList[1] - touchingParam.yList[0]);
            lastXscale = thisOsc->xScale;
            lastYscale = thisOsc->yScale;
        }
    }
    else if (state == HoldEnd){
        gestureValid = 0;
    }
    else if (touchingParam.longHold)
    {
        int deltaX = touchingParam.cursorNowX - touchingParam.clickX;
        int deltaY = touchingParam.cursorNowY - touchingParam.clickY;
        direction = absM(deltaX) > absM(deltaY);
        if (absM(deltaX) > gestureThreshold || absM(deltaY) > gestureThreshold)
        {
            gestureValid = 1;
        }
        if (gestureValid)
        {
            if (direction)
            {
                setXbias(thisOsc, lastXbias + deltaX);
                sprintf(floatMessage, "xBias:%d", thisOsc->xBias);
                updateMessage(floatMessage);
            }
            else
            {
                setYbias(thisOsc, lastYbias + deltaY);
                sprintf(floatMessage, "yBias:%d", thisOsc->yBias);
                updateMessage(floatMessage);
            }
        }
    }
    else if ((touchingParam.multiTouchSta >> 1) & 0x01)
    {
        int deltaX = absM(touchingParam.xList[1] - touchingParam.xList[0]) - lastXdist;
        int deltaY = absM(touchingParam.yList[1] - touchingParam.yList[0]) - lastYdist;
        direction = absM(deltaX) > abs(deltaY);
        if (absM(deltaX) > gestureThreshold || absM(deltaY) > gestureThreshold)
        {
            gestureValid = 1;
        }
        if (gestureValid)
        {
            if (direction)
            {
                int xscale = lastXscale;
                if (deltaX > 0)
                {
                    for (int i = 0; i < deltaX / zoomPixel; i++)
                    {
                        xscale = highDigitPlusOne(xscale);
                    }
                }
                else
                {
                    for (int i = 0; i < -deltaX / zoomPixel; i++)
                    {
                        xscale = highDigitMinusOne(xscale);
                    }
                }
                xscale = constrain(xscale, 50000, 1000000);
                setXscale(thisOsc, xscale);
                sprintf(floatMessage, "xscale:%d", xscale);
                updateMessage(floatMessage);
            }
            else
            {
                int yscale = lastYscale;
                if (deltaY > 0)
                {
                    for (int i = 0; i < deltaY / zoomPixel; i++)
                    {
                        yscale = highDigitPlusOne(yscale);
                    }
                }
                else
                {
                    for (int i = 0; i < -deltaY / zoomPixel; i++)
                    {
                        yscale = highDigitMinusOne(yscale);
                    }
                }
                yscale = constrain(yscale, 1, 1000000);
                setYscale(thisOsc, yscale);
                sprintf(floatMessage, "yscale:%d", yscale);
                updateMessage(floatMessage);
            }
        }
    }
}

void drawMesh()
{
    // mesh
    for (int j = boundary + dotLineInterval; j < HEIGHT - boundary; j += dotLineInterval)
    {
        if ((j - HEIGHT / 2) % numPixel != 0)
        {
            for (int i = boundary + meshPixel; i < WIDTH - boundary; i += meshPixel)
            {
                cachePoint(i, j, GRAY);
            }
        }
    }
    for (int j = boundary + meshPixel; j < HEIGHT - boundary; j += meshPixel)
    {
        for (int i = boundary + dotLineInterval; i < WIDTH - boundary; i += dotLineInterval)
        {
            cachePoint(i, j, GRAY);
        }
    }
    // num line
    for (int i = boundary + (WIDTH / 2 - boundary) % numPixel; i < WIDTH - boundary; i += numPixel)
    {
        cacheLine(i, boundary + 1, i, HEIGHT - boundary - 1, GRAY);
    }
    for (int j = boundary + (HEIGHT / 2 - boundary) % numPixel; j < HEIGHT - boundary; j += numPixel)
    {
        cacheLine(boundary + 1, j, WIDTH - boundary - 1, j, GRAY);
    }
    // axis
    cacheLine(WIDTH / 2, boundary, WIDTH / 2, HEIGHT - boundary, LGRAY);
    cacheLine(boundary, HEIGHT / 2, WIDTH - boundary, HEIGHT / 2, LGRAY);
}
void drawBoundary()
{
    cacheLine(boundary, boundary, WIDTH - boundary, boundary, WHITE);
    cacheLine(WIDTH - boundary, boundary, WIDTH - boundary, HEIGHT - boundary, WHITE);
    cacheLine(WIDTH - boundary, HEIGHT - boundary, boundary, HEIGHT - boundary, WHITE);
    cacheLine(boundary, HEIGHT - boundary, boundary, boundary, WHITE);
}
// axis label
void writeVol(int pixel, int yScale, char *pData)
{
    if (pixel / yScale != 0)
    {
        sprintf(pData, "%dV", pixel / yScale);
        return;
    }
    pixel *= 1000;
    if (pixel / yScale != 0)
    {
        sprintf(pData, "%dmV", pixel / yScale);
        return;
    }
    pixel *= 1000;
    if (pixel / yScale != 0)
    {
        sprintf(pData, "%duV", pixel / yScale);
        return;
    }
    sprintf(pData, "0V");
}
void writeTime(int pixel, int xScale, char *pData)
{
    if (pixel / xScale != 0)
    {
        sprintf(pData, "%ds", pixel / xScale);
        return;
    }
    pixel *= 1000;
    if (pixel / xScale != 0)
    {
        sprintf(pData, "%dms", pixel / xScale);
        return;
    }
    pixel *= 1000;
    if (pixel / xScale != 0)
    {
        sprintf(pData, "%dus", pixel / xScale);
        return;
    }
    pixel *= 1000;
    if (pixel / xScale != 0)
    {
        sprintf(pData, "%dns", pixel / xScale);
        return;
    }
    sprintf(pData, "0s");
}
void drawLabel(UIobject *this)
{
    char pData[16];
    UIwaveStruct *selfStruct = (UIwaveStruct *)this->selfStruct;
    for (int i = boundary + (WIDTH / 2 - boundary) % numPixel; i <= WIDTH - boundary; i += numPixel)
    {
        writeTime(i - WIDTH / 2, selfStruct->xScale, pData);
        cacheOneCenter(i, HEIGHT / 2 + 12, 12, pData, WHITE);
    }
    for (int i = boundary + (HEIGHT / 2 - boundary) % numPixel; i <= HEIGHT - boundary; i += numPixel)
    {
        writeVol(-(i - HEIGHT / 2), selfStruct->yScale, pData);
        cacheOneCenter(WIDTH / 2 - 12, i, 12, pData, WHITE);
    }
}

void drawWave(UIobject *this)
{
    UIwaveStruct *selfSturct = (UIwaveStruct *)(this->selfStruct);
    if (selfSturct->waveCh1 != NULL)
        for (int i = boundary; i < WIDTH - boundary; i++)
        {
            cacheLine(i, selfSturct->waveCh1[i] + this->y, i + 1, selfSturct->waveCh1[i + 1] + this->y, this->param[0]);
        }
    if (selfSturct->waveCh2 != NULL)
        for (int i = boundary; i < WIDTH - boundary; i++)
        {
            cacheLine(i, selfSturct->waveCh2[i] + this->y, i + 1, selfSturct->waveCh2[i + 1] + this->y, this->param[1]);
        }
}
void waveShader(UIobject *this)
{
    drawMesh();
    drawLabel(this);
    drawWave(this);
    drawBoundary();
}

/*
param:
    0:colorCh1
    1:colorCh2
*/
void waveUI(u16 colorCh1, u16 colorCh2, int priority, UIwaveStruct *selfStruct)
{
    UIobject *this = getUIobject();
    this->x = WIDTH / 2;
    this->y = HEIGHT / 2;
    this->box[0][0] = -WIDTH / 2 + boundary;
    this->box[0][1] = WIDTH / 2 - boundary;
    this->box[1][0] = -HEIGHT / 2 + boundary;
    this->box[1][1] = HEIGHT / 2 - boundary;
    this->param[0] = colorCh1;
    this->param[1] = colorCh2;
    this->eventListener = UIwaveEventlistener;
    this->shader = waveShader;
    this->priority = priority;
    // UIwaveStruct *selfStruct = (UIwaveStruct *)malloc(sizeof(UIwaveStruct));
    this->selfStruct = selfStruct;

    // selfStruct->xScale = 120000;
    // selfStruct->yScale = 40;
    priorityInsert(this);
}