#ifndef __OSC__
#define __OSC__

// #include "mathDef.h"
#include "volDef.h"
#include "adc.h"
#include "tim.h"
#include "interface.h" //otherwise define WIDTH your TFT screen width

extern volatile int busy;

typedef enum
{
    Amplify0x5 = 20,   // yScale = 20
    Amplify1x = 40,    // yScale = 40
    Amplify2x5 = 100,  // yscale = 100
    Amplify5x = 200,   // yScale = 200
    Amplify10x = 400,  // yScale = 400
    Amplify20x = 800,  // yScale = 800
    Amplify50x = 2000, // yScale = 2000
    Amplify100x = 4000 // yScale = 4000
} AmplifyNum;

typedef enum
{
    DC,
    AC
} InputMode;

typedef enum
{
    Auto,
    Manual
} TriggerMode;

typedef struct
{
    int max;
    int min;
    int avg;
    int freq;
    int length;
    int xBias;  // 延迟像素数
    int yBias;  // 外界偏执电压像素数
    int xScale; // 频率
    int yScale; // 1v含像素数
    int trigger;
    int *waveCh1; // 用于CH1波形UI显示
    int *waveCh2; // 用于CH2波形UI显示
    InputMode inputMode;
    TriggerMode triggerMode;
} OscData;

extern OscData *initOscData(int xBias, int yBias, int xScale, int yScale, InputMode inputMode, TriggerMode triggermode);
extern void getWaveCH1(uint16_t *wave, int length);
extern void getWaveCH2(uint16_t *wave, int length);
extern int ifBusy();
extern void processWave(OscData *data, uint16_t *wave, int length, int *waveUIlist);
extern int getVoltageNum();
extern void setXscale(OscData *data, int xscale);
extern void setYscale(OscData *data, int yscale);
extern void setYbias(OscData *data, int ybias);
extern void setTrigger(OscData *data, int trigger);
extern void bindOscWaveCh1(OscData *data, int *waveUIlist);
extern void bindOscWaveCh2(OscData *data, int *waveUIlist);
extern void setInputMode(OscData *data, InputMode mode);

#endif //  __OSC__