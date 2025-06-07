/* https://ru.wikipedia.org/wiki/%D0%98%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%87%D0%BB%D0%B5%D0%BD_%D0%9B%D0%B0%D0%B3%D1%80%D0%B0%D0%BD%D0%B6%D0%B0#%D0%A1%D0%BB%D1%83%D1%87%D0%B0%D0%B9_%D1%80%D0%B0%D0%B2%D0%BD%D0%BE%D0%BE%D1%82%D1%81%D1%82%D0%BE%D1%8F%D1%89%D0%B8%D1%85_%D1%83%D0%B7%D0%BB%D0%BE%D0%B2_%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%B8
*/

// #include "lagrange.h"
#include <inttypes.h>

// Интерполяция Лагранжа (с равными интервалами)

static constexpr int max_dx = 10;
static constexpr int max_pow = 11;

static uint16_t *yn;
static int32_t l[max_pow + 1][max_dx]; // Коэффициенты Лагранжа
static int32_t factor;
static int32_t ln;

static int32_t fact(int32_t x)
{
  if (x) return x * fact(x - 1);
  return 1;
}

// Вычисление коэффициентов Лагранжа
void L_init(uint8_t pow, uint8_t dx)
{
  if (pow > max_pow || dx > max_dx) return;
  ln = pow;
  factor = 1;
  for (int32_t i = 0; i < ln / 2; i++) factor *= dx;
  for (int32_t n = 0; n <= ln; n++) {
    int64_t d = factor * fact(ln - n) * fact(n) * (((n & 1) << 1) - 1);
    for (int32_t x = 0; x < dx; x++) {
      int64_t ls = 1;
      for (int32_t i = 0; i <= ln; i++)
        if (i != n) ls *= x + dx * ((ln >> 1) - i);
      l[n][x] = ls / d / dx;
    }
  }
}

void Ly(uint16_t *y) { yn = y; }
uint32_t L(int32_t x)
{
  int32_t res = 0;
  for (int32_t i = 0; i <= ln; i++) res += l[i][x] * yn[i];
  res /= factor;
  return (uint32_t)res;
}
