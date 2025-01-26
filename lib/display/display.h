#pragma once

#define _ST7735_
// #define _ST7789_
// #define _SSD1306_

// Выбор интерфейса

// #define LCD_SPI         // Подключает через SPI
// #define LCD_I2C         // Подключает через I2C
#define LCD_SOFT        // Программно

// Цветовая модель

// #define RGB_FORMAT RGB_M    // Монохроматический
#define RGB_FORMAT RGB_12   // 4x4x4 bit
// #define RGB_FORMAT RGB_16   // 5x6x5 bit
// #define RGB_FORMAT RGB_18   // 6x6x6 bit

// Повороты дисплея
//    \   |   FLIP_X  |   FLIP_Y  |   EX_X_Y  |
// ============================================
//   0°   |     -     |     -     |     -     |
//  90°   |     -     |     +     |     +     |
// 180°   |     +     |     +     |     -     |
// 270°   |     +     |     -     |     +     |

// #define FLIP_X
#define FLIP_Y
#define EX_X_Y


#ifdef EX_X_Y
#define MAX_X     LCD_MAX_Y
#define MAX_Y     LCD_MAX_X
#else
#define MAX_X     LCD_MAX_X
#define MAX_Y     LCD_MAX_Y
#endif


#ifdef _ST7735_
#define DISPLAY_MODEL ST7735
#include <ST7735/ST7735.h>
#elif defined(_ST7789_)
#define DISPLAY_MODEL ST7789
#include <ST7789/ST7789.h>
#elif defined(_SSD1306_)
#define DISPLAY_MODEL SSD1306
#include <SSD1306/SSD1306.h>
#endif

#define Display DISPLAY_MODEL
