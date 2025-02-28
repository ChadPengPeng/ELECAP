#ifndef __UIWAVE__
#define __UIWAVE__

#include "UIobject.h"
#include "interface.h"
#include "stdio.h"
#include "osc.h"
#include "UIfloatingWindow.h"
#include "touchEvent.h"

typedef OscData UIwaveStruct;

extern int *waveInt;
extern void waveUI(u16 colorCh1, u16 colorCh2, int priority, UIwaveStruct *selfStruct);

// typedef struct UIwave_struct
// {
//     int xBias;        //延迟像素数
//     int yBias;        //外界偏执电压像素数
//     int xScale;         //频率
//     int yScale;         //1v含像素数
//     int wave[WIDTH];
// } UIwaveStruct;

#endif // !__UIWAVE__