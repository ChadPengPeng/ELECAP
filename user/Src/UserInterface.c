#include "UserInterface.h"
void userInterface() {
    flotingUI(WIDTH / 2, HEIGHT / 4, 100, 20, MacaronORANGE, BLACK, 254);
    // your init object
    // recUI(120, 160, 100, 100, BLUE, 2);
    // recUI(130, 160, 100, 100, PURPLE, 2);
    // recUI(140, 160, 100, 100, GREEN, 2);
    // recUI(150, 160, 100, 100, YELLOW, 2);
    // recUI(160, 160, 100, 100, ORANGE, 2);
    UIobject *drawer = drawerUI(WIDTH-1, HEIGHT / 2, 50, HEIGHT * 3 / 4, MacaronGREEN, BLACK, 4);
    // drawerUI(100, 100, 100, 100, RED, GRAY, 4);
    buttonUI(20, -30, 40, 20, MacaronBLUE, BLUE, 5, drawer);
    buttonUI(20, 0, 40, 20, MacaronBLUE, BLUE, 5, drawer);
    buttonUI(20, 30, 40, 20, MacaronBLUE, BLUE, 5, drawer);
    messageUI(WIDTH / 2, HEIGHT * 3 / 4, WIDTH * 3 / 4, HEIGHT / 4, MacaronWHITE, BLACK, 6);

    extern OscData *thisOsc;
    waveUI(MacaronYELLOW, MacaronPINK, 1, thisOsc);
}