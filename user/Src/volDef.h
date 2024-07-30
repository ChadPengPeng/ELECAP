#ifndef __VOLDEF__
#define __VOLDEF__

#define VREF 2.5f
#define ADC_MAX 4095
#define DAC_MAX 4095

#define adcTIMfreq 10 * 1000 * 1000
#define dacTIMfreq 10 * 1000 * 1000

#define ABSVoltagePixel 50 // 1V 对应像素数
#define toVoltage(x, thisOsc) ((float)(x - ADC_MAX / 2) * VREF / (float)ADC_MAX * ABSVoltagePixel / ((thisOsc)->yScale))
#define toWaveUI(x) (-(((float)(x) * (float)ABSVoltagePixel * VREF)) / ADC_MAX + ABSVoltagePixel * VREF / 2)

#define adcTimer htim6
#define dacTimer htim7
#define adc1 hadc1
#define adc2 hadc2
#define dac1 hdac1
#define dac1Channel1 DAC_CHANNEL_1
#define GPIO_ACDC GPIOC
#define PIN_ACDC GPIO_PIN_0
#define GPIO_BY20 GPIOC
#define PIN_BY20 GPIO_PIN_1
#define GPIO_CD4052_A0 GPIOC
#define PIN_CD4052_A0 GPIO_PIN_2
#define GPIO_CD4052_A1 GPIOC
#define PIN_CD4052_A1 GPIO_PIN_3
#endif // !__VOLDEF__