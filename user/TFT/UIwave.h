#ifndef __UIWAVE__
#define __UIWAVE__

#include "UIobject.h"
#include "interference.h"
#include "stdio.h"
#include "osc.h"

typedef OscData UIwaveStruct;

extern int* waveInt;
extern void waveUI(u16 color, int priority, UIwaveStruct *selfStruct);

// typedef struct UIwave_struct
// {
//     int xBias;        //延迟像素数
//     int yBias;        //外界偏执电压像素数
//     int xScale;         //频率
//     int yScale;         //1v含像素数
//     int wave[WIDTH];
// } UIwaveStruct;



#endif // !__UIWAVE__