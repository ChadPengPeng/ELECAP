#include "waveG.h"
WaveG waveG;
uint16_t dacWave[WAVE_LEN] = {0};
void initWaveG(){
    waveG.freq = 1000.0f;
    waveG.amp = 0.5f;
    waveG.phase = 0;
    waveG.length = WAVE_LEN;
    waveG.waveType = SINE;
    HAL_TIM_Base_Start(&htim7);
    generageWave();
}
#define dacFreq 240000.0f
void generageWave(){
    HAL_DAC_Stop_DMA(&hdac1,DAC_CHANNEL_1);

    int length = dacFreq/waveG.freq;
    if(length > WAVE_LEN) return;    //warning: if the frequency is too high, the wave will be too long and may cause the program to crash.
    waveG.length = length;
    switch(waveG.waveType){
        case SINE:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((float)(waveG.amp*sinf(2.0f*3.1416f*waveG.freq*(float)i/dacFreq+waveG.phase) + 1.25f));
                dacWave[i] = constrain(dacWave[i],0,4095);
            }
            break;

        case SAWTOOTH:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((uint16_t)(waveG.amp*2.0f*i/length + 1.25f));
                dacWave[i] = constrain(dacWave[i],0,4095);
            }
            break;

        case TRIANGLE:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((uint16_t)(waveG.amp*2.0f*i/length + 1.25f));//todo
                dacWave[i] = constrain(dacWave[i],0,4095);
            }
            break;

        case SQUARE:
            for(int i=0;i<length;i++){
                dacWave[i] = toDacNum((uint16_t)(waveG.amp*2.0f*i/length + 1.25f));//todo
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
    generageWave();
}

void setAmp(float amp){
    waveG.amp = amp;
    generageWave();
}

void setPhase(float phase){
    waveG.phase = phase;
    generageWave();
}

void setWaveType(WaveType type){
    waveG.waveType = type;
    generageWave();
}

void setWave(float freq, float amp, float phase, WaveType type){
    waveG.freq = freq;
    waveG.amp = amp;
    waveG.phase = phase;
    waveG.waveType = type;
    generageWave();
}