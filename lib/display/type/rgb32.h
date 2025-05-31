#pragma once
#include "convert.h"

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
  RGB32(uint8_t r, uint8_t g, uint8_t b) : blue(b), green(g), red(r) {}
  RGB32(uint32_t c) : rgb(c) {}

  void rgb12(uint16_t c) { rgb = RGB_32_TO_12(c); }
  void rgb16(uint16_t c) { rgb = RGB_32_TO_16(c); }
  void rgb32(uint32_t c) { rgb = RGB_32_TO_32(c); }

  uint16_t rgb12() { return RGB_32_TO_12(rgb); }
  uint16_t rgb16() { return RGB_32_TO_16(rgb); }
  uint32_t rgb32() { return RGB_32_TO_32(rgb); }

  operator uint32_t() { return *(uint32_t *)this; }
};
