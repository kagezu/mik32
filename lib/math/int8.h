// #include "int8.h"
#pragma once
#include "core.h"

constexpr static int32_t _fix16_log2_lut[33] PROGMEM = {
  -1, 0, 65536, 103872,131072,152170,169408,183983,196608,
  207744,217706,226717,234944,242512,249519,256042,262144,
  267876,273280,278392,283242,287855,292253,296456,300480,
  304340,308048,311616,315055,318373,321578,324678,327680
};

/**
 * @brief log2(x)
 * @details x = [0...32]
 * @param x
 * @return const int32_t
 */
constexpr static inline int32_t fix16_log2(uint8_t x) { return _fix16_log2_lut[x]; }

// log2(x!), x = [0...32], log(x!) = Σ log(x)
constexpr static inline int32_t fix16_log2_fact(int8_t x)
{
  int32_t res = 0;
  for (uint8_t i = 2; i <= x; i++) res += _fix16_log2_lut[i];
  return res;
}

// Целочисленный логарифм по основанию 2
template<typename U>
constexpr static inline int8_t ilog2(U x)
{
  uint8_t res = -1;
  while (x) { x >>= 1; res++; }
  return res;
}
