// Цветовая модель

// #define RGB         RGB12   // 4x4x4 bit
#define RGB         RGB16   // 5x6x5 bit
// #define RGB         RGB32  // 6x6x6 bit


// Поворот дисплея

// #define LCD_ROT     ROT_0
#define LCD_ROT     ROT_90
// #define LCD_ROT     ROT_180
// #define LCD_ROT     ROT_270
// #define LCD_ROT     ROT_X
// #define LCD_ROT     ROT_Y
// #define LCD_ROT     ROT_EX
// #define LCD_ROT     ROT_EX_XY


// Драйвер дисплея

#include "ST7735_SOFT/driver.h"  
#include "ST7735_SPI/driver.h"    
#include "ST7789/driver.h"         
#include "ILI9486_8_BIT/driver.h" 
#include "ILI9486_16_BIT/driver.h"  

#include "display.h"

// #define LCD Display<ST7735_SOFT<RGB>, RGB, LCD_ROT>
#define LCD Display<ST7735_SPI<RGB>, RGB, LCD_ROT>
// #define LCD Display<ST7789<RGB>, RGB, LCD_ROT>
// #define LCD Display<ILI9486_8<RGB>, RGB, LCD_ROT>
// #define LCD Display<ILI9486_16<RGB>, RGB, LCD_ROT>


// Шрифты

extern const struct Font arial_14;
extern const struct Font micro_5x6;
extern const struct Font system_5x7;
extern const struct Font standard_5x8;
extern const struct Font sans_24;
extern const struct Font o_sans_18b;
extern const struct Font serif_18i;
