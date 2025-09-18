#pragma once
#include "convert.h"
#include "inttypes.h"

class RGB24 {
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
  RGB24() {}
  RGB24(uint8_t r, uint8_t g, uint8_t b) : blue(b), green(g), red(r) {}
  RGB24(uint32_t c) : rgb(c) {}

  INLINE void rgb12(uint16_t c) { rgb = RGB_24_TO_12(c); }
  INLINE void rgb16(uint16_t c) { rgb = RGB_24_TO_16(c); }
  INLINE void rgb24(uint32_t c) { rgb = RGB_24_TO_24(c); }

  INLINE uint16_t rgb12() { return RGB_24_TO_12(rgb); }
  INLINE uint16_t rgb16() { return RGB_24_TO_16(rgb); }
  INLINE uint32_t rgb24() { return RGB_24_TO_24(rgb); }

  operator uint32_t() { return *(uint32_t *)this; }

  static constexpr int len() { return 24; }
};
