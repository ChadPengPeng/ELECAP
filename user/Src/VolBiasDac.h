#ifndef __VOLBIASDAC__
#define __VOLBIASDAC__

#include "i2c.h"

#define VolBiasI2CBus hi2c1

#define VolBiasDacCh1Addr 0xc0
#define VolBiasDacCh2Addr 0xc2

#define BiasDacMax 4095

extern void setVolBiasDacCh1(uint16_t value);
extern void setVolBiasDacCh2(uint16_t value);

#endif // !__VOLBIASDAC__