#pragma once
#include "convert.h"
#include "inttypes.h"

class RGB16 {
public:
  uint16_t rgb;

public:
  RGB16() {}
  RGB16(uint8_t r, uint8_t g, uint8_t b) : rgb(RGB_TO_16(r, g, b)) {}
  RGB16(uint32_t c) : rgb(RGB_24_TO_16(c)) {}

  ATTR_INLINE void rgb12(uint16_t c) { rgb = RGB_12_TO_16(c); }
  ATTR_INLINE void rgb16(uint16_t c) { rgb = RGB_16_TO_16(c); }
  ATTR_INLINE void rgb24(uint32_t c) { rgb = RGB_24_TO_16(c); }

  ATTR_INLINE uint16_t rgb12() { return RGB_16_TO_12(rgb); }
  ATTR_INLINE uint16_t rgb16() { return RGB_16_TO_16(rgb); }
  ATTR_INLINE uint32_t rgb24() { return RGB_16_TO_24(rgb); }

  operator uint16_t() { return rgb24(); }
};
