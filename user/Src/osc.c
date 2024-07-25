#include "osc.h"

volatile int busy = 0;
void getWaveCH1(uint16_t *wave, int length)
{
    // Initialize the oscillator
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)wave, length);
    busy |= 1;
}

void getWaveCH2(uint16_t *wave, int length)
{
    // Initialize the oscillator
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)wave, length);
    busy |= 1 << 1;
}

int ifBusy()
{
    if (busy == 0)
    {
        HAL_TIM_Base_Stop(&htim6);
        HAL_ADC_Stop_DMA(&hadc1);
        HAL_ADC_Stop_DMA(&hadc2);
    }
    return busy;
}

int waveMax(uint16_t *wave, int length)
{
    int max = wave[0];
    for (int i = 1; i < length; i++)
    {
        if (wave[i] > max)
        {
            max = wave[i];
        }
    }
    return max;
}

int waveMin(uint16_t *wave, int length)
{
    int min = wave[0];
    for (int i = 1; i < length; i++)
    {
        if (wave[i] < min)
        {
            min = wave[i];
        }
    }
    return min;
}

int waveAvg(uint16_t *wave, int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum += wave[i];
    }
    return sum / length;
}

// 触发点检测
int triggerDetect(uint16_t *wave, int length, OscData *data)
{
    if (data->triggerMode == Auto)
    {
        data->trigger = data->avg;
    }
    int trigger = WIDTH / 2;
    for (int i = WIDTH / 2 + constrainMax(data->xBias, 0); i < length - WIDTH / 2 + constrainMin(data->xBias, 0); i++)
    {
        if (wave[i] > data->trigger && data->trigger >= wave[i - 1])
        {
            trigger = i;
            break;
        }
    }
    // data->xBias = trigger - WIDTH/2;
    return constrain(trigger - WIDTH / 2, 0, length - WIDTH);
}

void processWave(OscData *data, uint16_t *wave, int length, int *waveUIlist)
{
    data->max = waveMax(wave, length);
    data->min = waveMin(wave, length);
    data->avg = waveAvg(wave, length);
    data->xBias = triggerDetect(wave, length, data);
    data->length = length;
    for (int i = data->xBias; i < data->xBias + WIDTH; i++)
    {
        waveUIlist[i - data->xBias] = toWaveUI(wave[i]);
    }
}

int getVoltageNum()
{
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_GetValue(&hadc1);
    uint16_t voltage;
    voltage = (HAL_ADC_GetValue(&hadc1) + HAL_ADC_GetValue(&hadc1) + HAL_ADC_GetValue(&hadc1) + HAL_ADC_GetValue(&hadc1)) / 4;
    HAL_ADC_Stop(&hadc1);
    return voltage;
}

void setXbias(OscData *data, int xbias)
{
    data->xBias = xbias;
    // xbias just affect the trigger position, so there is no need to do anything here
}

void setYbias(OscData *data, int ybias)
{
    data->yBias = ybias;
    // todo
}

void setXscale(OscData *data, int xscale)
{

    // todo
    int TimerCounter = adcTIMfreq / xscale;
    // set timer6 auto reload register
    TIM6->ARR = TimerCounter - 1;
    // because TimerCounter is not float, so we need to use integer division to get the proper value of xScale
    //data->xScale = adcTIMfreq / TimerCounter;
    data->xScale = xscale;
}

void setYscale(OscData *data, int yscale)
{
    data->yScale = yscale;
    int index = 0;
    switch (yscale)
    {
    case Amplify0x5:
        index = 0;
        break;
    case Amplify1x:
        index = 1;
        break;
    case Amplify2x5:
        index = 2;
        break;
    case Amplify5x:
        index = 3;
        break;
    case Amplify10x:
        index = 4;
        break;
    case Amplify20x:
        index = 5;
        break;
    case Amplify50x:
        index = 6;
        break;
    case Amplify100x:
        index = 7;
        break;
    default:
        break;
    }
    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, index >> 2); // 1/20x PIN

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, index >> 1 & 0x1); // CD4052 A1 PIN

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, index & 0x1); // CD4052 A0 PIN
}

void setTrigger(OscData *data, int trigger)
{
    data->trigger = trigger;
    // todo
}

void bindOscWaveCh1(OscData *data, int *waveUIlist)
{
    data->waveCh1 = waveUIlist;
}

void bindOscWaveCh2(OscData *data, int *waveUIlist)
{
    data->waveCh2 = waveUIlist;
}

void setInputMode(OscData *data, InputMode mode)
{
    data->inputMode = mode;
    switch (mode)
    {
    case AC:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
        break;
    case DC:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
        break;
    default:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
        break;
    }
}

void setTriggerMode(OscData *data, TriggerMode mode)
{
    data->triggerMode = mode;
}

OscData *initOscData(int xBias, int yBias, int xScale, int yScale, InputMode inputMode, TriggerMode triggermode)
{
    OscData *data = (OscData *)malloc(sizeof(OscData));
    setXbias(data, xBias);
    setYbias(data, yBias);
    setXscale(data, xScale);
    setYscale(data, yScale);
    setTrigger(data, 0);
    setInputMode(data, inputMode);
    setTriggerMode(data, triggermode);
    return data;
}