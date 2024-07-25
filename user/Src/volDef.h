#ifndef __VOLDEF__
#define __VOLDEF__

#define VREF 2.5f
#define ADC_MAX 4095
#define DAC_MAX 4095

#define adcTIMfreq 10 * 1000 * 1000
#define dacTIMfreq 10 * 1000 * 1000

#define ABSVoltagePixel 40 // 1V 对应像素数
#define toVoltage(x, thisOsc) ((float)(x - ADC_MAX / 2) * VREF / (float)ADC_MAX * ABSVoltagePixel / ((thisOsc)->yScale))
#define toWaveUI(x) (-(((float)(x) * (float)ABSVoltagePixel * VREF)) / ADC_MAX + ABSVoltagePixel * VREF / 2)

#endif // !__VOLDEF__