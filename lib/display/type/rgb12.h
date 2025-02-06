// #include "type/rgb/h"
#pragma once
#include <core.h>

class RGB12 {
public:
  union {
    uint16_t rgb;
    struct {
      uint8_t blue : 4;
      uint8_t green : 4;
      uint8_t red : 4;
      uint8_t alpha : 4;
    };
  };

public:
  RGB12() {}
  RGB12(uint8_t _red, uint8_t _green, uint8_t _blue) : rgb(((_red & 0xf0) << 4) | (_green & 0xf0) | (_blue >> 4)) {}
  RGB12(uint32_t _rgb) : rgb(((_rgb >> 12) & 0xf00) | ((_rgb >> 8) & 0xf0) | ((_rgb >> 4) & 0xf)) {}

  void rgb12(uint16_t c) { rgb = c; }
  void rgb16(uint16_t c) { rgb = ((c >> 4) & 0xf00) | ((c >> 3) & 0xf0) | ((c >> 1) & 0xf); }
  void rgb32(uint32_t c) { rgb = ((c >> 12) & 0xf00) | ((c >> 8) & 0xf0) | ((c >> 4) & 0xf); }

  uint16_t rgb12() { return rgb; }
  uint16_t rgb16() { return ((rgb & 0xf00) << 4) | ((rgb & 0xf0) << 3) | ((rgb & 0xf) << 1); }
  uint32_t rgb32() { return ((rgb & 0xf00) << 12) | ((rgb & 0xf0) << 8) | ((rgb & 0xf) << 4); }

  operator uint32_t() { return rgb32(); }
};
