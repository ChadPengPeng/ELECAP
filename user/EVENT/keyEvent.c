#include "keyEvent.h"

void keyEvent()
{
    static State state[keyNum];
    static uint8_t lastClick[keyNum] = {0, 0, 0, 0};
    for (EventCode e = KEY1; e <= KEY4; e++)
    {
        int KeyIndex = e - KEY1;
        int click = (HAL_GPIO_ReadPin(GPIOE, (uint16_t)1 << (KeyIndex)) == RESET);
        if (state[KeyIndex] == OnClick)
            state[KeyIndex] = Hold;
        if (state[KeyIndex] == HoldEnd)
            state[KeyIndex] = NoneState;
        // warning: Only when key is PE0-PE3, this code can work.
        if (lastClick[KeyIndex] == 0b00000111 && click != 0)
        {
            state[KeyIndex] = OnClick;
        }
        if (lastClick[KeyIndex] == 0b00001000 && click == 0 && state[KeyIndex] == Hold)
        {
            state[KeyIndex] = HoldEnd;
        }

        lastClick[KeyIndex] = click | ((lastClick[KeyIndex] << 1) & 0b00001111);

        if (state[KeyIndex] != NoneState)
        {
            addEvent(getEvent(e, state[KeyIndex]));
        }
    }
}