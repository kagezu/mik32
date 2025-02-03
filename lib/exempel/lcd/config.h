// Цветовая модель

// #define RGB   RGB12   // 4x4x4 bit
// #define RGB   RGB16   // 5x6x5 bit
// #define RGB   RGB18   // 6x6x6 bit
#define RGB   RGB32   // Максимально поддерживаемое драйвером устройства

// Повороты дисплея
//    \   |   FLIP_X  |   FLIP_Y  |   EX_X_Y  |
// ============================================
//   0°   |     -     |     -     |     -     |
//  90°   |     -     |     +     |     +     |
// 180°   |     +     |     +     |     -     |
// 270°   |     +     |     -     |     +     |

// #define LCD_FLIP    0                           //   0°
// #define LCD_FLIP              FLIP_Y | EX_X_Y   //  90°
// #define LCD_FLIP    FLIP_X  | FLIP_Y            // 180°
// #define LCD_FLIP    FLIP_X  |          EX_X_Y   // 270°

// Вариант подключения (только для ST7735_SОFT)

#define PRESET_ST7735_A5
// #define PRESET_ST7735_A4
// #define PRESET_ST7735_SPI

// Драйвер дисплея

// #include "ST7735_SOFT/driver.h"
#include "ST7735_SPI/driver.h"

#include <display.h>
