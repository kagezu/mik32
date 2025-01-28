#pragma once
#include <core.h>

template<typename C>
class IDisplay {
public:
  virtual void color(C) = 0;
  virtual void background(C) = 0;
  virtual void clear(C) = 0;
  virtual void clear() = 0;
  virtual void rect(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
  virtual void pixel(uint16_t, uint16_t) = 0;

protected:
  virtual void symbol(uint8_t *, uint16_t, uint16_t, uint8_t, uint8_t) = 0;
};
