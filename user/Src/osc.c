#include "osc.h"

void getWave(uint16_t* wave, int length)
{
    // Initialize the oscillator
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)wave, length);
    //while (HAL_ADC_GetState(&hadc1)!= HAL_ADC_STATE_EOC);
		HAL_Delay(10);
    HAL_TIM_Base_Stop(&htim6);
    HAL_ADC_Stop_DMA(&hadc1);
}

int waveMax(uint16_t* wave, int length)
{
    int max = wave[0];
    for (int i = 1; i < length; i++) {
        if (wave[i] > max) {
            max = wave[i];
        }
    }        
    return max;
}

int waveMin(uint16_t* wave, int length)
{
    int min = wave[0];
    for (int i = 1; i < length; i++) {
        if (wave[i] < min) {
            min = wave[i];
        }
    }        
    return min;
}

int waveAvg(uint16_t* wave, int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += wave[i];
    }
    return sum / length;
}

//触发点检测
int triggerDetect(uint16_t* wave, int length, OscData* data)
{
    int trigger = WIDTH/2;
    for (int i = WIDTH/2 + constrainMax(data->xBias, 0); i < length - WIDTH/2 + constrainMin(data->xBias, 0); i++) {
        if (wave[i] > data->trigger && data->trigger >= wave[i-1]) {
            trigger = i;
            break;
        }
    }
    //data->xBias = trigger - WIDTH/2;
    return constrain(trigger - WIDTH/2, 0, length - WIDTH);
}

void processWave(uint16_t* wave, int length, OscData* data, int* waveUIlist)
{
    data->max = waveMax(wave, length);
    data->min = waveMin(wave, length);
    data->avg = waveAvg(wave, length);
    data->xBias = triggerDetect(wave, length, data);
    data->length = length;
    for (int i = data->xBias; i < data->xBias + WIDTH; i++) {
        waveUIlist[i-data->xBias] = -((wave[i] * 100.0f)) / 16383 + 50;
    }
}

int getVoltageNum(){
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_GetValue(&hadc1);
    uint16_t voltage;
    voltage = (HAL_ADC_GetValue(&hadc1)+HAL_ADC_GetValue(&hadc1)+HAL_ADC_GetValue(&hadc1)+HAL_ADC_GetValue(&hadc1))/4;
    HAL_ADC_Stop(&hadc1);
    return voltage;
}

void setXscale(OscData* data, int xscale)
{
    data->xScale = xscale;
    //todo
}

void setYscale(OscData* data, int yscale)
{
    data->yScale = yscale;
    //todo
}

void setYbias(OscData* data, int ybias)
{
    data->yBias = ybias;
    //todo
}
