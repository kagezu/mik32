// #include "rgb/rgb/h"
#pragma once
#include <inttypes.h>

class RGB {
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
  RGB() {}
  RGB(uint8_t _red, uint8_t _green, uint8_t _blue) : blue(_blue), green(_green), red(_red) {}
  RGB(uint32_t _rgb) : rgb(_rgb) {}

  operator uint32_t() { return *(uint32_t *)this; }

};
