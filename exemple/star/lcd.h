// Цветовая модель

// #define RGB         RGB12   // 4x4x4 bit
#define RGB         RGB16   // 5x6x5 bit
// #define RGB         RGB32   // 6x6x6 bit


// Поворот дисплея

#define LCD_ROT     R_0
// #define LCD_ROT     R_90
// #define LCD_ROT     R_180
// #define LCD_ROT     R_270
// #define LCD_ROT     R_X
// #define LCD_ROT     R_Y
// #define LCD_ROT     R_EX
// #define LCD_ROT     R_EX_XY

#include "display.h"

// #define LCD Display<ST7735<RGB>, RGB, LCD_ROT>           // RGB12, RGB16, RGB32
#define LCD Display<ST7735<SPI1, RGB>, RGB, LCD_ROT>     // RGB12, RGB16, RGB32
// #define LCD Display<ST7789<RGB>, RGB, LCD_ROT>           // RGB32
// #define LCD Display<ILI9486_8<RGB>, RGB, LCD_ROT>        // RGB16, RGB32
// #define LCD Display<ILI9486_16<RGB>, RGB, LCD_ROT>       // RGB16, RGB32


// Шрифты

extern const struct Font arial_14;
extern const struct Font micro_5x6;
extern const struct Font system_5x7;
extern const struct Font standard_5x8;
extern const struct Font sans_24;
extern const struct Font o_sans_18b;
extern const struct Font serif_18i;
