#include "waveG.h"
WaveG waveG;
uint16_t dacWave[WAVE_LEN] = {0};
void initWaveG(){
    setWave(1000, 100000, 1.0f, 0, SINE);
    HAL_TIM_Base_Start(&htim7);
    generageWave();
}

void generageWave(){
    HAL_DAC_Stop_DMA(&hdac1,DAC_CHANNEL_1);

    int length = waveG.dacFreq/waveG.freq;
    if(length > WAVE_LEN) return;    //warning: if the frequency is too high, the wave will be too long and may cause the program to crash.
    waveG.length = length;
    switch(waveG.waveType){
        case SINE:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((float)(waveG.amp*sinf(2.0f*3.1416f*waveG.freq*(float)i/waveG.dacFreq+waveG.phase) + VREF/2.0f));
                dacWave[i] = constrain(dacWave[i],0,DAC_MAX);
            }
            break;

        case SAWTOOTH:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((uint16_t)(waveG.amp*2.0f*i/length + VREF/2.0f));
                dacWave[i] = constrain(dacWave[i],0,DAC_MAX);
            }
            break;

        case TRIANGLE:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((uint16_t)(waveG.amp*2.0f*i/length + VREF/2.0f));//todo
                dacWave[i] = constrain(dacWave[i],0,DAC_MAX);
            }
            break;

        case SQUARE:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((uint16_t)(waveG.amp*2.0f*i/length + VREF/2.0f));//todo
                dacWave[i] = constrain(dacWave[i],0,4095);
            }
            break;

        default:
            break;
    }
    HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t*)dacWave,waveG.length,DAC_ALIGN_12B_R);
}

void generateVoltage(float volt){
    HAL_DAC_Stop_DMA(&hdac1,DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,toDacNum(volt));
    HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t*)dacWave,waveG.length,DAC_ALIGN_12B_R);
}

void setFreq(float freq){
    waveG.freq = freq;
}

void setAmp(float amp){
    waveG.amp = amp;
}

void setPhase(float phase){
    waveG.phase = phase;
}

void setWaveType(WaveType type){
    waveG.waveType = type;
}

void setDacFreq(int dacFreq){
    int timerCounter = dacTIMfreq/dacFreq;
    TIM7->ARR = timerCounter-1;
    waveG.dacFreq = dacTIMfreq/timerCounter;
}

void setWave(float freq, int dacFreq, float amp, float phase, WaveType type){
    setFreq(freq);
    setDacFreq(dacFreq);
    setAmp(amp);
    setPhase(phase);
    setWaveType(type);
    generageWave();
}