/* https://ru.wikipedia.org/wiki/%D0%98%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%87%D0%BB%D0%B5%D0%BD_%D0%9B%D0%B0%D0%B3%D1%80%D0%B0%D0%BD%D0%B6%D0%B0#%D0%A1%D0%BB%D1%83%D1%87%D0%B0%D0%B9_%D1%80%D0%B0%D0%B2%D0%BD%D0%BE%D0%BE%D1%82%D1%81%D1%82%D0%BE%D1%8F%D1%89%D0%B8%D1%85_%D1%83%D0%B7%D0%BB%D0%BE%D0%B2_%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%B8
*/

// Интерполяция Лагранжа (с равными интервалами)
// #include "lagrange.h"

#pragma once
#include "int8.h"

// #define USE_INT128
#ifdef USE_INT128
#include "int128.h"
#define LONG_INT  int128_t
#else
#define LONG_INT  int64_t
#endif


#define LAGRANGE_OVERFLOW   0x01
#define LAGRANGE_OVERFLOW_N 0x02
#define LAGRANGE_OVERFLOW_H 0x04


/**
 * @brief Интерполяция Лагранжа (с равными интервалами)
 * @details Интерполяция выполняется между узлами k и (k + 1), при числе узлов n = 2(k + 1), и (n - 1) степень полинома
 * Используется математика с фиксированной точкой, но положение точки выбирается при инициализации
 * @tparam T Тип для промежуточных операций преобразования
 * @tparam L Тип коэффициентов Лагранжа
 * @tparam H Шаг узлов интерполяции
 * @tparam N Количество узлов интерполяции
 */
template<typename D, typename L, const uint8_t H, const uint8_t N>
class Lagrange {
private:
  // массив коэффициентов объявляется вне класса
  L(&li)[H][N];

public:
  uint8_t dig;
  Lagrange(L(&X)[H][N], D &max_data) :li(X)
  {
    // Максимальная разрядность числителя
    constexpr uint8_t max_num = (sizeof(LONG_INT) << 3) - 1;
    // Максимальная разрядность коэффициентов Лагранжа, 1 бит оставлен под знак
    constexpr uint8_t max_dig = (sizeof(L) << 3) - 1;
    // Максимальная разрядность целочисленного типа
    constexpr uint8_t max_trans = (sizeof(D) << 3) - 1;
    // Узловая точка с которой начинается интерполяция
    constexpr uint8_t k = (N - 1) >> 1;
    // Разрядность числителя log2((h^(n-1))*((n/2)!)^2/(n/2))
    const uint8_t l_num = ((fix16_log2_fact(N >> 1) << 1) + fix16_log2(H) * (N - 1) - fix16_log2(N >> 1)) >> 16;

    // Overflow
    if (l_num > max_num)  return;

    // Фиксированная точка в коэффициентах Лагранжа
    dig = max_dig > l_num ? l_num : max_dig;
    // Нужно умножить числитель и разделить делитель, так чтобы не потерять точность
    // Увеличить числитель, избежав переполнение
    const uint8_t s_num = (max_num - l_num) > dig ? dig : max_num - l_num;

    D pow = 1;
    // h^(n-1)
    for (int8_t i = 0; i < N; i++) pow *= H;
    // Уменьшить знаменатель, если уже нельзя увеличивать числитель
    // dig != s_num только если числитель уже полный, эти биты в любом случае будут отброшены при делении
    pow >>= dig - s_num;

    // Вычисление коэффициентов Лагранжа
    for (int8_t j = 0; j < N; j++) {
      LONG_INT div = pow * fact(N - j - 1) * fact(j);
      if ((j & 1) == 0) div = -div;
      for (int8_t x = 0; x < H; x++) {
        LONG_INT num = 1;
        for (int8_t i = 0; i < N; i++)
          if (i != j) num *= x + H * (k - i);
        li[x][j] = (num << s_num) / div; // Q L.dig
      }
    }
  }

  L f(L *y, int8_t x)
  {
    D res = 0;
    for (int8_t i = 0; i < N; i++) res += (D)li[x][i] * y[i];
    res >>= dig;
    return res >> dig;
  }

private:
  static const uint64_t fact(int8_t x)
  {
    uint64_t result = 1;
    for (int8_t i = 1; i <= x; i++) result *= i;
    return result;
  }
};
