// Интерполяция Лагранжа (с равными интервалами)
// #include "lagrange.h"

#pragma once
#include "int8.h"
#include "int128.h"

// #define USE_INT128

// Максимальная разрядность числителя
#ifdef USE_INT128 // Вариант с числами 128 bit
typedef int128_t num_t;// Тип для расчёта коэффициентов
typedef int64_t int_t; // Аккумулятор
typedef int64_t lag_t; // Коэффициенты Лагранжа
#else
typedef int64_t num_t; // Тип для расчёта коэффициентов
typedef int32_t int_t; // Аккумулятор
typedef int32_t lag_t; // Коэффициенты Лагранжа
#endif

// Разрядность типа для расчёта коэффициентов
constexpr int max_num = (sizeof(num_t) << 3) - 1;
// Максимальная разрядность аккумулятора минус знак
constexpr uint8_t max_int = (sizeof(int_t) << 3) - 1;
// Максимальная разрядность коэффициентов Лагранжа, 1 бит оставлен под знак
constexpr uint8_t max_dig = (sizeof(lag_t) << 3) - 2;


// Интерполяция Лагранжа (с равными интервалами)
// Интерполяция выполняется между узлами x0 и (x0 + 1), при числе узлов n = 2·(x0 + 1), и (n - 1) степень полинома
// Используется математика с фиксированной точкой, но положение точки выбирается при инициализации
// NODE Количество узлов интерполяции
// STEP Шаг узлов интерполяции
// DEEP Разрядность входных значений
template<const int NODE, const int STEP, const int DEEP>
class Lagrange {
private:
  lag_t li[STEP][NODE];
  int dig;  // Дробная точка Q lag_t.dig
  int step; // Шаг узлов интерполяции

public:
  void init(int s = STEP)
  {
    if (step == s) return;
    step = s; // Запоминаем новые параметры

    // Разрядность числителя log2((h^(n-1))·((n/2)!)^2/(n/2))
    const int32_t l_num = ((fix16_log2_fact(NODE >> 1) << 1) + fix16_log2(step) * (NODE - 1) - fix16_log2(NODE >> 1)) >> 16;

    // Установим фиксированную точку в коэффициентах Лагранжа
    // Сдвигаем точку вправо, чтобы не вызвать переполнения
    dig = max_int - DEEP > max_dig ? max_dig : max_int - DEEP;

    // Теперь нужно умножить числитель и разделить делитель, так чтобы не потерять точность
    // Увеличить числитель, избежав переполнение
    const int dig_num = (max_num - l_num) > dig ? dig : max_num - l_num;

    // Уменьшить знаменатель, если уже нельзя увеличивать числитель
    // dig_denum != 0 только если числитель уже полный, эти биты в любом случае будут отброшены при делении
    const int dig_denum = dig - dig_num;

    // Вычисление коэффициентов Лагранжа
    // NODE - количество узловых точек точек (степень полинома :p = n-1) 
    // step - Шаг узлов интерполяции

    // Узловая точка с которой начинается интерполяция
    const uint8_t x0 = (NODE - 1) >> 1;

  #ifdef USE_INT128  // Вариант с числами 128 bit

    int128_t pow = int128_const(0, 1); // 1
    for (int8_t i = 0; i < NODE - 1; i++) pow = int128_mul_i128_i64(pow, step); // pow = h^p
    pow = int128_shift(pow, -dig_denum); // Q128.-denum

    // Переберем все узловые точки
    for (int8_t j = 0; j < NODE; j++) {
      int128_t d = int128_mul_i128_i64(pow, fact(NODE - j - 1));
      d = int128_mul_i128_i64(d, fact(j)); // h^p · (p-j)! · j!
      int8_t reduce = 1 + ilog2(int128_hi(d)); // если reduce > 0, нужно сдвинуть до int64
      int64_t div = int128_lo(int128_shift(d, -reduce)); // Q64.-(denum + reduce)
      if (div < 0) { reduce++; div = ((uint64_t)div >> 1); }  // ещё 1 бит под знак
      if ((j & 1) == 0) div = -div; // (-1)^(p-j)

      // Переберем все промежуточные значения, необходимые интерполировать
      for (int8_t x = 0; x < step; x++) {

        // Перемножаем все отклонения от искомой точки
        int128_t num = int128_const(0, 1);
        for (int8_t i = 0; i < NODE; i++)
          if (i != j) num = int128_mul_i128_i64(num, x + (int64_t)step * (x0 - i)); // П(x - x0 - ih), i = 0...p

        // Q128.( dig_num - reduce)
        num = int128_shift(num, dig_num - reduce);

        // Заполним таблицу коэффициентов 
        int128_t l = int128_div_i128_i64(num, div);
        li[x][j] = (lag_t)(int128_hi(l) > 0 ? int128_lo(l) : -int128_lo(l)); // Q lag_t.dig
      }
    }

  #else

    num_t pow = 1;
    for (int i = 0; i < NODE - 1; i++)
      pow *= step; // pow = h^p
    pow >>= dig_denum; // Q X.-denum

    // Переберем все узловые точки
    for (int j = 0; j < NODE; j++) {
      num_t div = pow * fact(NODE - j - 1) * fact(j); // h^p · (p-j)! · j!
      if ((j & 1) == 0) div = -div; // (-1)^(p-j)

      // Переберем все промежуточные значения, необходимые интерполировать
      for (int x = 0; x < step; x++) {

        // Перемножаем все отклонения от искомой точки П(x - x0 - ih), i = 0...p
        num_t num = 1;
        for (int i = 0; i < NODE; i++)
          if (i != j) num *= x + step * (x0 - i);

        // Заполним таблицу коэффициентов Q lag_t.dig
        li[x][j] = (num << dig_num) / div;
      }
    }

  #endif
  }

  // Интерполяция массива данных
  template<typename I, typename O>
  void interpolate(I *in, O *out, int size)
  {
    const int segments = size / step; // Количество отрезков для интерполяции
    for (int i = 0; i < segments; i++) {
      for (int x = 0; x < step; x++) {
        int_t sum = 0;
        for (int j = 0; j < NODE; j++)
          sum += (int_t)li[x][j] * in[j]; // Q int_t.dig
        *out++ = sum >> dig; // Q lag_t.0
      }
      in++; // Следующий отрезок
    }
  }

private:
  static const uint64_t fact(int x)
  {
    uint64_t result = 1;
    for (int8_t i = 1; i <= x; i++) result *= i;
    return result;
  }
};
