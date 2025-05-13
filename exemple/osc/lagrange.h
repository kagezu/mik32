// #include "lagrange.h"
#include <inttypes.h>

// Интерполяция Лагранжа

#ifndef Ln
#define Ln  9// Степень полинома Лагранжа (нечётная)
#endif

#ifndef Lx
#define Lx  10 // Интервал между значениями
#endif

static uint16_t *yn;
static int32_t l[Ln + 1][Lx];
static int32_t factor;

static int32_t fact(int32_t x)
{
  if (x) return x * fact(x - 1);
  return 1;
}

void L_init()
{
  factor = 1;
  for (int32_t i = 0; i < Ln / 2; i++) factor *= Lx;
  for (int32_t n = 0; n <= Ln; n++) {
    int64_t d = factor * fact(Ln - n) * fact(n) * (((n & 1) << 1) - 1);
    for (int32_t x = 0; x < Lx; x++) {
      int64_t ls = 1;
      for (int32_t i = 0; i <= Ln; i++)
        if (i != n) ls *= x + Lx * ((Ln >> 1) - i);
      l[n][x] = ls / d / Lx;
    }
  }
}

void Ly(uint16_t *y) { yn = y; }
uint32_t L(int32_t x)
{
  int32_t res = 0;
  for (uint32_t i = 0; i <= Ln; i++) res += l[i][x] * yn[i];
  res /= factor;
  return (uint32_t)res;
}
