#ifndef __OSC__
#define __OSC__

#include "mathDef.h"

#include "adc.h"
#include "tim.h"
#include "interference.h"   //otherwise define WIDTH your TFT screen width

#define toVoltage(x) ((float)(x) * 2.5f / 16383.0)

typedef struct {
    int max;
    int min;
    int avg;
    int length;
    int xBias;        //延迟像素数
    int yBias;        //外界偏执电压像素数
    int xScale;         //频率
    int yScale;         //1v含像素数
    int trigger;
    int* wave;
} OscData;

extern void getWave(uint16_t* wave, int length);
extern int ifBusy();
extern void processWave(uint16_t* wave, int length, OscData* data, int* waveUIlist);
extern int getVoltageNum();
extern void setXscale(OscData* data, int xscale);
extern void setYscale(OscData* data, int yscale);
extern void setYbias(OscData* data, int ybias);

#endif //  __OSC__