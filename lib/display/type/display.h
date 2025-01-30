#pragma once
#include <core.h>

class IDisplay {
public:
  virtual const uint16_t max_x() = 0;
  virtual const uint16_t max_y() = 0;

  virtual void clear() = 0;
  virtual void rect(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
  virtual void pixel(uint16_t, uint16_t) = 0;
  virtual void symbol(uint8_t *, uint16_t, uint16_t, uint8_t, uint8_t) = 0;
};
