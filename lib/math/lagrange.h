/* https://ru.wikipedia.org/wiki/%D0%98%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%87%D0%BB%D0%B5%D0%BD_%D0%9B%D0%B0%D0%B3%D1%80%D0%B0%D0%BD%D0%B6%D0%B0#%D0%A1%D0%BB%D1%83%D1%87%D0%B0%D0%B9_%D1%80%D0%B0%D0%B2%D0%BD%D0%BE%D0%BE%D1%82%D1%81%D1%82%D0%BE%D1%8F%D1%89%D0%B8%D1%85_%D1%83%D0%B7%D0%BB%D0%BE%D0%B2_%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%B8
*/

// Интерполяция Лагранжа (с равными интервалами)
// #include "lagrange.h"

#include "int8.h"

#define USE_INT128
#ifdef USE_INT128
#include "int128.h"
#define TYPE_INT  int128_t
#else
#define TYPE_INT  int64_t
#endif

static const int32_t fact(int8_t x)
{
  int32_t result = 1;
  for (int8_t i = 1; i <= x; i++) result *= i;
  return result;
}

/**
 * @brief Интерполяция Лагранжа (с равными интервалами)
 * @details Интерполяция выполняется между узлами k и (k + 1), при числе узлов n = 2(k + 1) и (n - 1) степень полинома
 * @tparam H Шаг узлов интерполяции
 * @tparam NODE Количество узлов интерполяции
 */
template<typename L, const uint8_t H, const uint8_t NODE>
class Lagrange {
private:
  L l[H][NODE];

  int32_t factor;
  L *y;

public:
  Lagrange()
  {
    // Проверки, чтобы избежать переполнения
    uint8_t h_pow = uint8_log2_pow(H, NODE - 1);// h^pow
    uint8_t n2f2 = uint8_log2_l(NODE);          // ((n/2)!)^2/(n/2)
    // uint8_t powf = uint8_log2_fact(NODE - 1);   // n!
    uint8_t max = h_pow + n2f2;

    if (max >= sizeof(TYPE_INT) << 3) return; // Overflow
    if (n2f2 >= sizeof(L) << 3) return;       // Overflow Lagrange coefficients
  }

  void init(uint8_t pow, uint8_t h)
  {
    // Узловая точка с которой начинается аппроксимация
    const int8_t k = pow >> 1;

    int32_t factor = 1;
    for (int8_t i = 0; i < k; i++) factor *= h; // f = h^k

    // Вычисление коэффициентов Лагранжа
    for (int8_t j = 0; j < NODE; j++) {
      int64_t d = factor * fact(pow - j) * fact(j);
      if ((j & 1) == 0) d = -d;
      for (int8_t x = 1; x < h; x++) {
        int64_t ls = 1;
        for (int8_t i = 0; i < NODE; i++)
          if (i != j) ls *= x + h * (k - i);
        l[j][x] = ls / d / h;
      }
    }
  }

  void Ly(uint16_t *in) { y = in; }
  uint32_t f(int8_t x)
  {
    int32_t res = 0;
    for (int8_t i = 0; i < NODE; i++) res += l[x][i] * y[i];
    res /= factor;
    return (uint32_t)res;
  }

};
