#include "DDS.h"
#include "stdlib.h"

DDS_Config *ddsList[MAXCHANNEL];
int ddsCount = 0;

DDS_Config *get_DDS(uint8_t channel)
{
    for (int i = 0; i < ddsCount; i++)
    {
        if (ddsList[i]->channel == channel)
        {
            return ddsList[i];
        }
    }
    return NULL;
}

void set_freq(DDS_Config *dds, int freq)
{
    if (dds != NULL)
    {
        dds->freq = freq;
    }
    ad9959_write_frequency(dds->channel, freq);
    ad9959_io_update();
}

void set_amp(DDS_Config *dds, int amp)
{
    if (dds != NULL)
    {
        dds->amp = amp;
    }
    ad9959_write_amplitude(dds->channel, amp);
    ad9959_io_update();
}

void set_phase(DDS_Config *dds, int phase)
{
    if (dds != NULL)
    {
        dds->phase = phase;
    }
    ad9959_write_phase(dds->channel, phase);
    ad9959_io_update();
}

DDS_Config *DDS_Init(uint8_t channel, int freq, int amp, int phase)
{
    DDS_Config *dds = (DDS_Config *)malloc(sizeof(DDS_Config));
    ddsList[ddsCount] = dds;
    ddsCount++;
    dds->channel = channel;
    set_amp(dds, amp);
    set_freq(dds, freq);
    set_phase(dds, phase);
    return dds;
}
