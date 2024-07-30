#include "VolBiasDac.h"

void setVolBiasDacCh1(uint16_t value)
{
    static uint8_t data[2];
    data[0] = value >> 8;
    data[1] = value & 0xFF;
    HAL_I2C_Master_Transmit(&VolBiasI2CBus, VolBiasDacCh1Addr, data, 2, 1000);
}

void setVolBiasDacCh2(uint16_t value)
{
    static uint8_t data[2]; 
    data[0] = value >> 8;
    data[1] = value & 0xFF;
    HAL_I2C_Master_Transmit(&VolBiasI2CBus, VolBiasDacCh2Addr, data, 2, 1000);
}