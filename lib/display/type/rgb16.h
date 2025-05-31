#pragma once
#include "convert.h"

class RGB16 {
public:
  uint16_t rgb;

public:
  RGB16() {}
  RGB16(uint8_t r, uint8_t g, uint8_t b) : rgb(RGB_TO_16(r, g, b)) {}
  RGB16(uint32_t c) : rgb(RGB_32_TO_16(c)) {}

  void rgb12(uint16_t c) { rgb = RGB_12_TO_16(c); }
  void rgb16(uint16_t c) { rgb = RGB_16_TO_16(c); }
  void rgb32(uint32_t c) { rgb = RGB_32_TO_16(c); }

  uint16_t rgb12() { return RGB_16_TO_12(rgb); }
  uint16_t rgb16() { return RGB_16_TO_16(rgb); }
  uint32_t rgb32() { return RGB_16_TO_32(rgb); }

  operator uint32_t() { return rgb32(); }
};
