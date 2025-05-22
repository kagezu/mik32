#pragma once
#include "core.h"

// Повороты дисплея
//    \   |   FLIP_X  |   FLIP_Y  |   EX_X_Y  |
// ============================================
//   0°   |     -     |     -     |     -     |
//  90°   |     -     |     +     |     +     |
// 180°   |     +     |     +     |     -     |
// 270°   |     +     |     -     |     +     |

#define  EX_X_Y   0x20
#define  FLIP_X   0x40
#define  FLIP_Y   0x80

class ROT_0 { public: GCC_INLINE constexpr uint8_t state() { return 0; } };
class ROT_90 { public: GCC_INLINE constexpr uint8_t state() { return FLIP_Y | EX_X_Y; } };
class ROT_180 { public: GCC_INLINE constexpr uint8_t state() { return FLIP_X | FLIP_Y; } };
class ROT_270 { public: GCC_INLINE constexpr uint8_t state() { return FLIP_X | EX_X_Y; } };
class ROT_X { public: GCC_INLINE constexpr uint8_t state() { return FLIP_X; } };
class ROT_Y { public: GCC_INLINE constexpr uint8_t state() { return FLIP_X; } };
class ROT_EX { public: GCC_INLINE constexpr uint8_t state() { return EX_X_Y; } };
class ROT_EX_XY { public: GCC_INLINE constexpr uint8_t state() { return EX_X_Y | FLIP_X | FLIP_Y; } };
