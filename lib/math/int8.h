#pragma once
#include <core.h>

static const int32_t _log2_lut[33] PROGMEM = {
  -1, 0, 1, 103872, 131072,152170,169408,183983,196608,
  207744,217706,226717,234944,242512,249519,256042,262144,
  267876,273280,278392,283242,287855,292253,296456,300480,
  304340,308048,311616,315055,318373,321578,324678,327680
};

/**
 * @brief log2(n!)
 *
 * @param n
 * @return uint8_t
 */
static inline uint8_t uint8_log2_fact(uint8_t n)
{
  int32_t collect = 0;

  // log(n!) = Σ log(n)
  for (uint8_t i = 2; i <= n; i++)
    collect += _log2_lut[i];
  return collect >> 16;
}

/**
 * @brief log2(n^pow)
 *
 * @param n > 0
 * @param pow
 * @return uint8_t
 */
static inline uint8_t uint8_log2_pow(uint8_t n, uint8_t pow)
{
  return (_log2_lut[n] * pow) >> 16;
}

/**
 * @brief log2(((n/2)!)^2/(n/2))
 * @details Максимальная разрядность коэффициента лагранжа
 * @param n
 * @return uint8_t
 */
static inline uint8_t uint8_log2_l(uint8_t n)
{
  uint8_t n2 = n >> 1;
  int32_t result = 0;

  // log(n!) = Σ log(n) Вычисляем факториал
  for (uint8_t i = 2; i <= n2; i++)
    result += _log2_lut[i];

  // log(n^2) = 2 log(n) Возводим в квадрат
  result <<= 1;

  // Делим на n2
  result -= _log2_lut[n2];

  return result >> 16;
}
