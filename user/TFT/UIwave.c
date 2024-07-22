#include "UIwave.h"

#define boundary 20
#define meshPixel 10
#define numPixel 50
#define dotLineInterval 3

// used for gesures processing
char gesureString[64];
#define gestureThreshold 100
void UIwaveEventlistener(UIobject *this, Event event)
{
    static int beginLongHold = 0;
    int direction;
    int gesturenum;
    int eventCode = eventCodeMask(event);
    if (touchingParam.longHold)
    {
        int deltaX = touchingParam.cursorNowX - touchingParam.clickX;
        int deltaY = touchingParam.cursorNowY - touchingParam.clickY;
        direction = absM(deltaX) > absM(deltaY);
        if (direction)
        {
            gesturenum = deltaX / gestureThreshold;
            if (deltaX > 0)
            {
                int i;
                for (i = 0; i < gesturenum; i++)
                {
                    gesureString[i] = '>';
                }
                gesureString[i] = 'R';
                gesureString[i + 1] = '\0';
            }
            else
            {
                int i;
                for (i = 0; i < -gesturenum; i++)
                {
                    gesureString[i] = '<';
                }
                gesureString[i] = 'L';
                gesureString[i + 1] = '\0';
            }
        }
        else
        {
            gesturenum = deltaY / gestureThreshold;
            if (deltaY > 0)
            {
                int i;
                for (i = 0; i < gesturenum; i++)
                {
                    gesureString[i] = '^';
                }
                gesureString[i] = 'D';
                gesureString[i + 1] = '\0';
            }
            else
            {
                int i;
                for (i = 0; i < -gesturenum; i++)
                {
                    gesureString[i] = 'v';
                }
                gesureString[i] = 'U';
                gesureString[i + 1] = '\0';
            }
        }

        if (beginLongHold == 1)
        {
            floatingMessage(gesureString);
            beginLongHold = 0;
        }
        else if (eventCode == Touching)
        {
            updateMessage(gesureString);
        }
        else if (eventCode == TouchingEnd)
        {
            // todo
        }
    }
    if (eventCode == OnClick)
    {
        beginLongHold = 1;
    }
}

void drawMesh()
{
    // mesh
    for (int j = boundary + dotLineInterval; j < HEIGHT - boundary; j += dotLineInterval)
    {
        if ((j - HEIGHT / 2) % numPixel != 0)
        {
            u16 *thisLine = frameCache[j];
            for (int i = boundary + meshPixel; i < WIDTH - boundary; i += meshPixel)
            {
                // cachePoint(i, j, GRAY);
                thisLine[i] = GRAY;
            }
        }
    }
    for (int j = boundary + meshPixel; j < HEIGHT - boundary; j += meshPixel)
    {
        u16 *thisLine = frameCache[j];
        for (int i = boundary + dotLineInterval; i < WIDTH - boundary; i += dotLineInterval)
        {
            thisLine[i] = GRAY;
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
    for (int i = boundary + (WIDTH / 2 - boundary) % numPixel; i < WIDTH - boundary; i += numPixel)
    {
        writeTime(i - WIDTH / 2, selfStruct->xScale, pData);
        cacheOneCenter(i, HEIGHT / 2 + 12, 12, pData, WHITE);
    }
    for (int i = boundary + (HEIGHT / 2 - boundary) % numPixel; i < HEIGHT - boundary; i += numPixel)
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
    UIobject *result = getUIobject();
    result->x = WIDTH / 2;
    result->y = HEIGHT / 2;
    result->box[0][0] = -WIDTH / 2 + boundary;
    result->box[0][1] = WIDTH / 2 - boundary;
    result->box[1][0] = -HEIGHT / 2 + boundary;
    result->box[1][1] = HEIGHT / 2 - boundary;
    result->param[0] = colorCh1;
    result->param[1] = colorCh2;
    result->eventListener = UIwaveEventlistener;
    result->shader = waveShader;
    result->priority = priority;
    // UIwaveStruct *selfStruct = (UIwaveStruct *)malloc(sizeof(UIwaveStruct));
    result->selfStruct = selfStruct;

    // selfStruct->xScale = 120000;
    // selfStruct->yScale = 40;
    priorityInsert(result);
}