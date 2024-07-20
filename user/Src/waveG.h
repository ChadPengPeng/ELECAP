#ifndef __WAVEG__
#define __WAVEG__


#include "mathDef.h"

#include "tim.h"
#include "dac.h"
#define WAVE_LEN 256

#define toDacNum(x) ((uint16_t)((float)(x) / 2.5f * 4095.0f))

extern uint16_t dacWave[];

typedef enum {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
} WaveType;

typedef struct {
    float freq;
    float amp;
    float phase;
    int length;
    WaveType waveType;
} WaveG;

extern WaveG waveG;



extern void initWaveG();
extern void generageWave(void);
extern void generateVoltage(float volt);
extern void setFreq(float freq);
extern void setAmp(float amp);
extern void setPhase(float phase);
extern void setWaveType(WaveType waveType);
extern void setWave(float freq, float amp, float phase, WaveType type);

#endif // !__WAVEG__