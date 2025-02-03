// #include "type/rgb/h"
#pragma once
#include <core.h>

class RGB32 {
public:
  union {
    uint32_t rgb;
    struct {
      uint8_t blue;
      uint8_t green;
      uint8_t red;
      uint8_t alpha;
    };
  };

public:
  RGB32() {}
  RGB32(uint8_t _red, uint8_t _green, uint8_t _blue) : blue(_blue), green(_green), red(_red) {}
  RGB32(uint32_t _rgb) : rgb(_rgb) {}

  void rgb12(uint16_t c) { red = (c >> 4) & 0xf0; green = c & 0xf0; blue = c << 4; }
  void rgb16(uint16_t c) { red = (c >> 8) & 0xf8; green = (c >> 3) & 0xfc; blue = c << 3; }
  void rgb32(uint32_t c) { rgb = c; }

  uint16_t rgb12() { return (red << 4) | (green & 0xf0) | (blue >> 4); }
  uint16_t rgb16() { return ((red & 0xf8) << 8) | (green & 0xfc) | (blue >> 3); }
  uint32_t rgb32() { return rgb; }

  operator uint32_t() { return *(uint32_t *)this; }
};

















/*
#if RGB_FORMAT == RGB_M
#include "rgbm.h"
#elif RGB_FORMAT == RGB_12
#include "rgb12.h"
#elif RGB_FORMAT == RGB_16
#include "rgb16.h"
#elif RGB_FORMAT == RGB_18
#include "rgb18.h"
#endif
*/
