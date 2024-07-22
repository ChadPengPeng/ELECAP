#include "keyEvent.h"

void keyEvent()
{
    static State state[keyNum];
    static uint8_t lastClick[keyNum] = {0};
    for (Event e = KEY1; e <= KEY4; e++)
    {
        lastClick[e] = (HAL_GPIO_ReadPin(GPIOE, (uint16_t)1 << (e - KEY1)) == RESET) | lastClick[e] << 1;
        if (state[e] == OnClick)
            state[e] = Hold;
        if (state[e] == HoldEnd)
            state[e] = NoneState;
        // warning: Only when key is PE0-PE3, this code can work.
        if (lastClick[e] == 0b11111111 && state[e] == NoneState)
        {
            state[e] = OnClick;
        }
        if (lastClick[e] == 0b00000000 && state[e] == Hold)
        {
            state[e] = HoldEnd;
        }
        if (state[e] != NoneState)
        {
            addEvent(getEvent(e, state[e]));
        }
    }
}