#ifndef __DDS__
#define __DDS__

#include "ad9959.h"
#include "mathDef.h"

#define MAXCHANNEL 4

typedef struct
{
    uint8_t channel;
    int freq;
    int amp;
    int phase;
} DDS_Config;

extern DDS_Config *get_DDS(uint8_t channel);
extern DDS_Config *DDS_Init(uint8_t channel, int freq, int amp, int phase);
extern void set_freq(DDS_Config *dds, int freq);
extern void set_amp(DDS_Config *dds, int amp);
extern void set_phase(DDS_Config *dds, int phase);


#endif // __DDS__