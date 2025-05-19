#pragma once
#include <core.h>

class IDisplay {
public:
  virtual void putc(char) = 0;
  virtual void rect(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
  virtual void pixel(uint16_t, uint16_t) = 0;
};
