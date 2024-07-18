#ifndef __INTERFERENCE__
#define __INTERFERENCE__

#include "bsp_system.h"
#include "dev_lcd.h"
#include "fontInclude.h"

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define ORANGE 0xF830  // Standard Orange
#define PURPLE 0x7750 // Standard Purple
// Additional colors
#define PINK 0xFF10    // Pink
#define DARKPINK 0xE720 // Dark Pink
#define LIGHTPINK 0xFF80 // Light Pink
#define TURQUOISE 0x40E0 // Turquoise
#define SKYBLUE 0x80D0 // Sky Blue
#define SAND 0xDCA0   // Sand
#define OLIVE 0x7F00   // Olive Green
#define MAROON 0x7F00 // Maroon
#define PEACH 0xFFA0  // Peach
#define LAVENDER 0xE070 // Lavender
#define BEIGE 0xE0E0  // Beige
#define MUSTARD 0xE080 // Mustard Yellow
#define TEAL 0x00C0   // Teal
#define INDIGO 0x4400 // Indigo
#define CORAL 0xFF40  // Coral
#define PLUM 0x8040   // Plum
#define KHAKI 0xE0A0  // Khaki
#define LIME 0x00E0   // Lime Green
#define VIOLET 0x7050 // Violet
#define PERIWINKLE 0xA090 // Periwinkle
#define CREAM 0xFFF0  // Cream
#define ROSE 0xFF40   // Rose
#define RUST 0xB440   // Rust
#define SAPPHIRE 0x0080 // Sapphire
#define IVORY 0xFFFF  // Ivory (very close to white)
#define AMBER 0xFFC0  // Amber
#define SAPGREEN 0x4070 // Sap Green
#define GOLDENROD 0xE0A0 // Goldenrod
#define BURNTSIENNA 0xA040 // Burnt Sienna
#define SAGE 0x80A0   // Sage Green
#define ALMOND 0xE0D0  // Almond
#define BRASS 0xE0C0   // Brass
#define CHOCOLATE 0xA040 // Chocolate
#define SALMON 0xFF80  // Salmon
#define ASHGRAY 0x7070 // Ash Gray
#define COPPER 0xA060  // Copper
#define FUCHSIA 0xF030 // Fuchsia
#define SANDSTONE 0xD0A0 // Sandstone
#define CHARCOAL 0x4040 // Charcoal
#define PEPPERMINT 0x30E0 // Peppermint
#define BRONZE 0xA060   // Bronze
#define JADE 0x00A0   // Jade
#define RASPBERRY 0xE030 // Raspberry
#define MINT 0x30E0   // Mint
#define CINNABAR 0xE020 // Cinnabar
#define ECRU 0xE0D0   // Ecru
#define TAUPE 0x8060  // Taupe
#define CERULEAN 0x30D0 // Cerulean
#define SCARLET 0xF010 // Scarlet
#define BISTRE 0x6030  // Bistre
#define LEMON 0xFFE0   // Lemon
#define LIMEGREEN 0x30E0 // Lime Green
#define TAN 0xD0A0   // Tan
#define AQUAMARINE 0x20E0 // Aquamarine
#define MANGENTA 0xF81F // Magenta (corrected typo)
#define INDIGOPURPLE 0x4410 // Indigo Purple
#define BRIGHTVIOLET 0x7010 // Bright Violet
#define PEACOCK 0x00A0 // Peacock
#define LILAC 0xA070   // Lilac
#define LAVENDERBLUSH 0xFFF0 // Lavender Blush
#define WHEAT 0xE0A0   // Wheat
#define CREAMPUFF 0xFFF0 // Cream Puff
#define ROSEWOOD 0xA040 // Rosewood
#define MINTCREAM 0xFFF0 // Mint Cream
#define SMOKEGRAY 0x9090 // Smoke Gray
#define PERSIMMON 0xE030 // Persimmon
#define MISTYROSE 0xFFF0 // Misty Rose
#define MOSSGREEN 0x40A0 // Moss Green
#define CREAMYSAGE 0xE0A0 // Creamy Sage
#define OYSTER 0xE0D0   // Oyster
#define PASTELPINK 0xFFA0 // Pastel Pink
#define PASTELBLUE 0x80D0 // Pastel Blue
#define PASTELGREEN 0x80A0 // Pastel Green
#define PASTELYELLOW 0xFFD0 // Pastel Yellow
#define PASTELORANGE 0xFF80 // Pastel Orange
#define PASTELPURPLE 0xD080 // Pastel Purple
#define PASTELBROWN 0xC080 // Pastel Brown
#define PASTELRED 0xFF60   // Pastel Red
#define PASTELMAGENTA 0xFF90 // Pastel Magenta
#define PASTELCYAN 0x80FF   // Pastel Cyan

#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458

#define LIGHTGREEN 0X841F
#define LGRAY 0XC618

#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12

#define WIDTH 320
#define HEIGHT 240

// RGB565颜色格式定义
#define MacaronPINK   0xF81F // 淡粉色
#define MacaronBLUE   0x54EA // 浅蓝色
#define MacaronGREEN  0x37E3 // 淡绿色
#define MacaronYELLOW 0xFFE0 // 柠檬黄色
#define MacaronPURPLE 0xD477 // 紫罗兰色
#define MacaronORANGE 0xF840 // 橙色
#define MacaronBROWN  0xC628 // 褐色
#define MacaronWHITE  0xFFFF // 白色
#define MacaronBLACK  0x0000 // 黑色

#define GRAPHICSIZE (WIDTH * HEIGHT)

#define SCREEN_Init() LCD_Init()
#define DrawPoint(x, y, color) LCD_Fast_DrawPoint(x, y, color)
#define SetWindow(sx, sy, width, height)   \
    LCD_Set_Window(sx, sy, width, height); \
    LCD_WriteRAM_Prepare()
#define WriteColor(color) LCD_WriteRAM(color)
#define ReadColor(x, y) LCD_ReadPoint(x, y)

#define absM(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)
#define getR(color) ((color) >> 11)
#define getG(color) (((color) & 0b0000011111100000) >> 5)
#define getB(color) ((color) & 0b0000000000011111)
#define Migrate(R, G, B) (((R) << 11) + ((G) << 5) + (B))
#define to565(color) (( (Color & 0X00FCFCFC ) >>2 )|((Color & 0X0000FC00 ) >>4)|((Color & 0X00FC0000 ) >>6))

extern u16 frameCache[HEIGHT][WIDTH];
#define cachePoint(x, y, color) frameCache[y][x] = (color)
#define getPoint(x, y) (frameCache[y][x])

extern u16 FadeColor(u16 color, u16 weight);
extern void DrawTransparentPoint(int x, int y, u16 color, u16 weight);
extern void cacheLine(int x1, int y1, int x2, int y2, u16 color);
extern void cacheString(int x, int y, int width, int height, u8 size, char *p, u16 color);
extern void cacheOneCenter(int x, int y, u8 size, char *p, u16 color);
extern void cacheCircle(int x0, int y0, int r, u16 color);
extern void cacheRoundedRec(int x, int y, int width, int height, int r, u16 color);
extern void cacheVLine(int x0, int x1, int y, u16 color);
extern void cacheCenterRec(int centerX, int centerY, int halfWidth, int halfHeight, u16 color);
extern void cacheCenterBlock(int centerX, int centerY, int halfWidth, int halfHeight, u16 color);
#endif // !__INTERFERENCE__
