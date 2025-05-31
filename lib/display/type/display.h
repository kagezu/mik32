#pragma once
#include <core.h>
// #include "rect.h"

class IDisplay {
public:
  virtual void putc(uint8_t) = 0;
  // virtual void fill(Rect r) = 0;
  virtual void fill(int16_t, int16_t, int16_t, int16_t) = 0;
  virtual void pixel(int16_t, int16_t) = 0;
};
