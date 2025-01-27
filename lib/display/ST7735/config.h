// Цветовая модель

// #define RGB_FORMAT RGB_M    // Монохроматический
// #define RGB_FORMAT RGB_12   // 4x4x4 bit
#define RGB_FORMAT RGB_16   // 5x6x5 bit
// #define RGB_FORMAT RGB_18   // 6x6x6 bit

// Повороты дисплея
//    \   |   FLIP_X  |   FLIP_Y  |   EX_X_Y  |
// ============================================
//   0°   |     -     |     -     |     -     |
//  90°   |     -     |     +     |     +     |
// 180°   |     +     |     +     |     -     |
// 270°   |     +     |     -     |     +     |

// #define FLIP_X
// #define FLIP_Y
// #define EX_X_Y


#ifdef EX_X_Y
#define MAX_X     LCD_MAX_Y
#define MAX_Y     LCD_MAX_X
#else
#define MAX_X     LCD_MAX_X
#define MAX_Y     LCD_MAX_Y
#endif
