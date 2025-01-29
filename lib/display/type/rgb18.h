#pragma once
#include <core.h>

class RGB18 {
public:
  uint8_t blue;
  uint8_t green;
  uint8_t red;

public:
  RGB18() {}
  RGB18(uint8_t _red, uint8_t _green, uint8_t _blue) : blue(_blue), green(_green), red(_red) {}
  RGB18(uint32_t _rgb) : blue(_rgb), green(_rgb >> 8), red(_rgb >> 16) {}

  void rgb12(uint16_t c) { red = (c >> 4) & 0xf0; green = c & 0xf0; blue = c << 4; }
  void rgb16(uint16_t c) { red = (c & 0xf800) >> 8; green = (c & 0x07e0) >> 3; blue = (c & 0x1f) << 3; }
  void rgb32(uint32_t c) { *this = c; }

  uint16_t rgb12() { return (red << 4) | (green & 0xf0) | (blue >> 4); }
  uint16_t rgb16() { return ((red & 0xf8) << 8) | (green & 0xfc) | (blue >> 3); }
  uint32_t rgb32() { return *this; }

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
