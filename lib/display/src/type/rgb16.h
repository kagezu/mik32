// #include "type/rgb/h"
#pragma once
#include <core.h>

class RGB16 {
public:
  union {
    uint16_t rgb;
    struct {
      uint8_t blue : 5;
      uint8_t green : 6;
      uint8_t red : 5;
    };
  };

public:
  RGB16() {}
  RGB16(uint8_t _red, uint8_t _green, uint8_t _blue) : rgb(((_red & 0xf8) << 8) | ((_green & 0xfc) << 3) | (_blue >> 3)) {}
  RGB16(uint32_t _rgb) : rgb(((_rgb >> 8) & 0xf100) | ((_rgb >> 5) & 0x7e0) | ((_rgb >> 3) & 0x1f)) {}

  void rgb12(uint16_t c) { rgb = ((c << 4) & 0xf000) | ((c << 3) & 0x780) | ((c << 1) & 0xe); }
  void rgb16(uint16_t c) { rgb = c; }
  void rgb32(uint32_t c) { rgb = ((c >> 8) & 0xf100) | ((c >> 5) & 0x7e0) | ((c >> 3) & 0x1f); }

  uint16_t rgb12() { return ((rgb & 0xf000) >> 4) | ((rgb & 0x780) >> 3) | ((rgb & 0x1f) >> 1); }
  uint16_t rgb16() { return rgb; }
  uint32_t rgb32() { return ((rgb & 0xf800) << 8) | ((rgb & 0x7e0) << 5) | ((rgb & 0x1f) << 3); }

  operator uint32_t() { return rgb32(); }
};
