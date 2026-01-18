// #include "average.h"
#pragma once
#include <inttypes.h>

template<typename T, const uint8_t F = 0>
class Average {
public:
  T value;

  Average() { value = 0; }
  Average(T &arg) { value = arg << F; }

  Average &operator=(uint8_t arg) { value = arg << F; return *this; }
  Average &operator=(uint16_t arg) { value = arg << F; return *this; }
  Average &operator=(uint32_t arg) { value = arg << F; return *this; }

  Average &operator+=(int8_t arg) { value = value - (value >> F) + arg; return *this; }
  Average &operator+=(uint8_t arg) { value = value - (value >> F) + arg; return *this; }
  Average &operator+=(int16_t arg) { value = value - (value >> F) + arg; return *this; }
  Average &operator+=(uint16_t arg) { value = value - (value >> F) + arg; return *this; }
  Average &operator+=(int32_t arg) { value = value - (value >> F) + arg; return *this; }
  Average &operator+=(uint32_t arg) { value = value - (value >> F) + arg; return *this; }

  operator T() const { return (T)(value >> F); }
  operator uint8_t() const { return (uint8_t)(value >> F); }
  operator uint16_t() const { return (uint16_t)(value >> F); }
  operator uint32_t() const { return (uint32_t)(value >> F); }
};
