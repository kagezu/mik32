#pragma once
#include "convert.h"
#include "inttypes.h"

class RGB12 {
public:
  uint16_t rgb;

public:
  RGB12() {}
  RGB12(uint8_t r, uint8_t g, uint8_t b) : rgb(RGB_TO_12(r, g, b)) {}
  RGB12(uint32_t c) : rgb(RGB_24_TO_12(c)) {}

  void rgb12(uint16_t c) { rgb = RGB_12_TO_12(c); }
  void rgb16(uint16_t c) { rgb = RGB_12_TO_16(c); }
  void rgb24(uint32_t c) { rgb = RGB_12_TO_24(c); }

  uint16_t rgb12() { return RGB_12_TO_12(rgb); }
  uint16_t rgb16() { return RGB_12_TO_16(rgb); }
  uint32_t rgb24() { return RGB_12_TO_24(rgb); }

  operator uint32_t() { return rgb24(); }
};
