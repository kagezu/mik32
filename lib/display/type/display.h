#pragma once
#include <core.h>

class IDisplay {
public:
  virtual void putc(uint8_t) = 0;
  virtual void rect(int16_t, int16_t, int16_t, int16_t) = 0;
  virtual void pixel(int16_t, int16_t) = 0;
};
