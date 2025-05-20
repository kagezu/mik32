// Шаблон конфигурации

// Цветовая модель

// #define RGB   RGB12   // 4x4x4 bit
// #define RGB   RGB16   // 5x6x5 bit
// #define RGB   RGB18   // 6x6x6 bit
// #define RGB   RGB32   // Максимально поддерживаемое драйвером устройства

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

// Либо указать самостоятельно

// #define L_SCK(x)     x ( C, PC0)
// #define L_SDA(x)     x ( C, PC1)
// #define L_RS(x)      x ( C, PC2)
// #define L_RST(x)     x ( C, PC3)
// #define L_CS(x)      x ( C, PC4)
// #define L_PORT(x)    x ( D, PD0)

// Драйвер дисплея

// #include "ST7735_SOFT/driver.h"      // RGB12, RGB16, RGB18
// #include "ST7735_SPI/driver.h"       // RGB12, RGB16, RGB18
// #include "ST7789/driver.h"           // RGB18
// #include "ILI9486_8_BIT/driver.h"    // RGB16, RGB18
// #include "ILI9486_16_BIT/driver.h"   // RGB16, RGB18

// #include <display.h>
