#include "UIwave.h"

#define boundary 20
#define meshPixel 10
#define numPixel 40
#define dotLineInterval 3

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
    for (int i = boundary + boundary % numPixel; i < WIDTH - boundary; i += numPixel)
    {
        cacheLine(i, boundary + 1, i, HEIGHT - boundary - 1, GRAY);
    }
    for (int j = boundary + boundary % numPixel; j < HEIGHT - boundary; j += numPixel)
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
    for (int i = boundary + boundary % numPixel; i < WIDTH - boundary; i += numPixel)
    {
        writeTime(i - WIDTH / 2, selfStruct->xScale, pData);
        cacheOneCenter(i, HEIGHT / 2 + 12, 12, pData, WHITE);
    }
    for (int i = boundary + boundary % numPixel; i < HEIGHT - boundary; i += numPixel)
    {
        writeVol(i - HEIGHT / 2, selfStruct->xScale, pData);
        cacheOneCenter(WIDTH / 2 - 12, i, 12, pData, WHITE);
    }
}

void drawWave(UIobject *this)
{
    UIwaveStruct *selfSturct = (UIwaveStruct *)(this->selfStruct);
    int *waveList = (int *)(selfSturct->wave);
    for (int i = boundary; i < WIDTH - boundary; i++)
    {
        cacheLine(i, waveList[i] + this->y, i + 1, waveList[i + 1] + this->y, this->param[0]);
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
    0:color
*/
int *waveInt;
void waveUI(u16 color, int priority)
{
    UIobject *result = getUIobject();
    result->x = WIDTH / 2;
    result->y = HEIGHT / 2;
    // result->box[0][1]=0;
    // result->box[0][0]=0;
    // result->box[1][1]=0;
    // result->box[1][0]=0;
    result->param[0] = color;
    result->shader = waveShader;
    result->priority = priority;
    UIwaveStruct *selfStruct = (UIwaveStruct *)malloc(sizeof(UIwaveStruct));
    result->selfStruct = selfStruct;
    selfStruct->xScale = 400;
    selfStruct->yScale = 400;

    waveInt = selfStruct->wave;
    priorityInsert(result);
}