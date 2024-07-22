#include "ITHandler.h"

void AdcCH1Finish(DMA_HandleTypeDef *hdma)
{
    HAL_TIM_Base_Stop(&htim6);
    HAL_ADC_Stop_DMA(&hadc1);
}