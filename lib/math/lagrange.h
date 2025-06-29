/* https://ru.wikipedia.org/wiki/%D0%98%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%87%D0%BB%D0%B5%D0%BD_%D0%9B%D0%B0%D0%B3%D1%80%D0%B0%D0%BD%D0%B6%D0%B0#%D0%A1%D0%BB%D1%83%D1%87%D0%B0%D0%B9_%D1%80%D0%B0%D0%B2%D0%BD%D0%BE%D0%BE%D1%82%D1%81%D1%82%D0%BE%D1%8F%D1%89%D0%B8%D1%85_%D1%83%D0%B7%D0%BB%D0%BE%D0%B2_%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D0%BE%D0%BB%D1%8F%D1%86%D0%B8%D0%B8
*/

// Интерполяция Лагранжа (с равными интервалами)
// #include "lagrange.h"

#pragma once
#include "int8.h"
#include "int128.h"

// #define USE_INT128

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
  // Дробная точка Q L.dig
  uint8_t dig;

public:
  Lagrange(L(&X)[H][N], D &max_input) :li(X)
  {
    // Определим возможность вычислений с заданными параметрами
    // Максимальная разрядность числителя
    constexpr uint8_t max_num = sizeof(int64_t) << 3;
    // Разрядность числителя log2((h^(n-1))*((n/2)!)^2/(n/2))
    const uint8_t l_num = ((fix16_log2_fact(N >> 1) << 1) + fix16_log2(H) * (N - 1) - fix16_log2(N >> 1)) >> 16;
    if (l_num > max_num)
      return; // Overflow 

    // Установим фиксированную точку в коэффициентах Лагранжа
    // Максимальная разрядность коэффициентов Лагранжа, 1 бит оставлен под знак
    constexpr uint8_t max_dig = (sizeof(L) << 3) - 1;
    // Максимальная разрядность целочисленного типа минус знак
    constexpr uint8_t max_int = (sizeof(D) << 3) - 1;
    // Сдвигаем точку вправо, чтобы не вызвать переполнения
    dig = max_int - max_input > max_dig ? max_dig : max_int - max_input;
    // Теперь нужно умножить числитель и разделить делитель, так чтобы не потерять точность
    // Увеличить числитель, избежав переполнение
    const uint8_t dig_num = (max_num - l_num) > dig ? dig : max_num - l_num;
    // Уменьшить знаменатель, если уже нельзя увеличивать числитель
    // dig_denum != 0 только если числитель уже полный, эти биты в любом случае будут отброшены при делении
    const uint8_t dig_denum = dig - dig_num;

    // Вычисление коэффициентов Лагранжа
    // N - количество узловых точек точек (степень полинома :p = n-1) 
    // H - Шаг узлов интерполяции

    // Узловая точка с которой начинается интерполяция
    constexpr uint8_t k = (N - 1) >> 1;

  #ifdef USE_INT128  // Вариант с числами 128 bit

    int128_t pow = int128_const(0, 1); // 1
    for (int8_t i = 0; i < N; i++) pow = int128_mul_i128_i64(pow, i); // pow = h^p
    pow = int128_shift(pow, -dig_denum); // Q128.-denum

    // Переберем все узловые точки
    for (int8_t j = 0; j < N; j++) {
      int128_t d = int128_mul_i128_i64(pow, fact(N - j - 1));
      d = int128_mul_i128_i64(d, fact(j)); // h^p * (p-j)! * j!
      int8_t reduce = 1 + int8_log2(int128_hi(d)); // если reduce > 0, нужно сдвинуть до int64
      int64_t div = int128_lo(int128_shift(d, -reduce)); // Q64.-(denum + reduce)
      if (div < 0) { reduce++; div = ((uint64_t)div >> 1); }  // ещё 1 бит под знак
      if ((j & 1) == 0) div = -div; // (-1)^(p-j)

      // Переберем все промежуточные значения, необходимые интерполировать
      for (int8_t x = 0; x < H; x++) {

        // Перемножаем все отклонения от искомой точки
        int128_t num = int128_const(0, 1);
        for (int8_t i = 0; i < N; i++)
          if (i != j) num = int128_mul_i128_i64(num, (int64_t)H * (k - i)); // П(x - x0 - ih), i = 0...p

        // Q128.( dig_num - reduce)
        num = int128_shift(num, dig_num - reduce);

        // Заполним таблицу коэффициентов 
        int128_t l = int128_div_i128_i64(num, div);
        li[x][j] = (L)(int128_hi(l) > 0 ? int128_lo(l) : -int128_lo(l)); // Q L.dig
      }
    }

  #else

    int64_t pow = 1;
    for (int8_t i = 0; i < N; i++) // pow = h^p
      pow *= H;
    pow >>= dig_denum; // Q X.-denum

    // Переберем все узловые точки
    for (int8_t j = 0; j < N; j++) {
      int64_t div = pow * fact(N - j - 1) * fact(j); // h^p * (p-j)! * j!
      if ((j & 1) == 0) div = -div; // (-1)^(p-j)

      // Переберем все промежуточные значения, необходимые интерполировать
      for (int8_t x = 0; x < H; x++) {

        // Перемножаем все отклонения от искомой точки П(x - x0 - ih), i = 0...p
        int64_t num = 1;
        for (int8_t i = 0; i < N; i++)
          if (i != j) num *= x + H * (k - i);

        // Заполним таблицу коэффициентов Q L.dig
        li[x][j] = (num << dig_num) / div;
      }
    }

  #endif
  }

  L f(L *y, int8_t x)
  {
    D res = 0;
    for (int8_t i = 0; i < N; i++) res += (D)li[x][i] * y[i];
    return res >> dig;
  }

  /**
   * @brief Интерполяция массива данных
   *
   * @param out выходной массив
   * @param in массив узловых точек
   * @param size количество промежутков интерполяции
   */
  void interpolate(L *out, L *in, int16_t size)
  {
    for (int16_t i = 0; i < size; i++) {
      D sum = 0;
      for (int8_t x = 0; x < N; x++)
        sum += (D)li[x][i] * in[i]; // Q D.dig
      *out = sum >> dig; // Q L.0
    }
  }

private:
  static const uint64_t fact(int8_t x)
  {
    uint64_t result = 1;
    for (int8_t i = 1; i <= x; i++) result *= i;
    return result;
  }
};
