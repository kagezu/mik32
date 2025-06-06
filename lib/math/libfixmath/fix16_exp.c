#include "fix16.h"
#include <stdbool.h>

#ifndef FIXMATH_NO_CACHE
static fix16_t _fix16_exp_cache_index[4096] = { 0 };
static fix16_t _fix16_exp_cache_value[4096] = { 0 };
#endif

fix16_t fix16_exp(fix16_t inValue)
{
  if (inValue == 0) return fix16_one;
  if (inValue == fix16_one) return fix16_e;
  if (inValue >= 681391) return fix16_maximum;
  if (inValue <= -772243) return 0;

#ifndef FIXMATH_NO_CACHE
  fix16_t tempIndex = (inValue ^ (inValue >> 4)) & 0x0FFF;
  if (_fix16_exp_cache_index[tempIndex] == inValue)
    return _fix16_exp_cache_value[tempIndex];
#endif

  /* Алгоритм основан на степенном ряде для exp(x):
 * http://en.wikipedia.org/wiki/Exponential_function#Formal_definition
 *
 * Из члена n мы получаем член n+1, умножая на x/n.
 * Когда член суммы падает до нуля, мы можем прекратить суммирование.
 */

 // Степенной ряд сходится гораздо быстрее на положительных значениях
 // и exp(-x) = 1/exp(x).
  bool neg = (inValue < 0);
  if (neg) inValue = -inValue;

  fix16_t result = inValue + fix16_one;
  fix16_t term = inValue;

  uint_fast8_t i;
  for (i = 2; i < 30; i++) {
    term = fix16_mul(term, fix16_div(inValue, fix16_from_int(i)));
    result += term;

    if ((term < 500) && ((i > 15) || (term < 20)))
      break;
  }

  if (neg) result = fix16_div(fix16_one, result);

#ifndef FIXMATH_NO_CACHE
  _fix16_exp_cache_index[tempIndex] = inValue;
  _fix16_exp_cache_value[tempIndex] = result;
#endif

  return result;
}



fix16_t fix16_log(fix16_t inValue)
{
  fix16_t guess = fix16_from_int(2);
  fix16_t delta;
  int scaling = 0;
  int count = 0;

  if (inValue <= 0)
    return fix16_minimum;

  // Bring the value to the most accurate range (1 < x < 100)
  const fix16_t e_to_fourth = 3578144;
  while (inValue > fix16_from_int(100)) {
    inValue = fix16_div(inValue, e_to_fourth);
    scaling += 4;
  }

  while (inValue < fix16_one) {
    inValue = fix16_mul(inValue, e_to_fourth);
    scaling -= 4;
  }

  do {
    // Solving e(x) = y using Newton's method
    // f(x) = e(x) - y
    // f'(x) = e(x)
    fix16_t e = fix16_exp(guess);
    delta = fix16_div(inValue - e, e);

    // It's unlikely that logarithm is very large, so avoid overshooting.
    if (delta > fix16_from_int(3))
      delta = fix16_from_int(3);

    guess += delta;
  } while ((count++ < 10)
    && ((delta > 1) || (delta < -1)));

  return guess + fix16_from_int(scaling);
}



static inline fix16_t fix16_rs(fix16_t x)
{
#ifdef FIXMATH_NO_ROUNDING
  return (x >> 1);
#else
  fix16_t y = (x >> 1) + (x & 1);
  return y;
#endif
}

/**
* Предполагается, что входное значение >= 1.
*
* Обратите внимание, что это всегда вызывается только с inValue >= 1 (потому что у него есть оболочка для проверки.
* Таким образом, результат всегда меньше входного значения.
*/
static fix16_t fix16__log2_inner(fix16_t x)
{
  fix16_t result = 0;

  while (x >= fix16_from_int(2)) {
    result++;
    x = fix16_rs(x);
  }

  if (x == 0) return (result << 16);

  uint_fast8_t i;
  for (i = 16; i > 0; i--) {
    x = fix16_mul(x, x);
    result <<= 1;
    if (x >= fix16_from_int(2)) {
      result |= 1;
      x = fix16_rs(x);
    }
  }
#ifndef FIXMATH_NO_ROUNDING
  x = fix16_mul(x, x);
  if (x >= fix16_from_int(2)) result++;
#endif

  return result;
}



/**
* вычисляет логарифм по основанию 2 входных данных.
* Обратите внимание, что отрицательные входные данные недопустимы! (возвращает fix16_overflow, поскольку исключений нет)
*
* т. е.  2 ^ output = input
* Это эквивалентно функциям log или ln, за исключением того, что вместо основания 10 или основания e используется основание 2.
* Это полезно, поскольку двоичные данные, подобные этому, легко вычисляются двоичными устройствами, такими как современные микропроцессоры.
*
* Это можно использовать как вспомогательную функцию для вычисления степеней с нецелыми степенями и/или основаниями.
*/
fix16_t fix16_log2(fix16_t x)
{
  // Обратите внимание, что отрицательный x дает недействительный результат.
  // Если x == 0, предел log2(x) при x -> 0 = -бесконечность.
  // log2(-ve) дает комплексный результат.
  if (x <= 0) return fix16_overflow;

  // Если входное значение меньше единицы, результат равен -log2(1.0 / in)
  if (x < fix16_one) {
    // Обратите внимание, что обратное значение приведет к переполнению.
    // Это точный ответ для log2(1.0 / 65536)
    if (x == 1) return fix16_from_int(-16);

    fix16_t inverse = fix16_div(fix16_one, x);
    return -fix16__log2_inner(inverse);
  }

  // Если ввод >= 1, просто продолжайте как обычно.
  // Обратите внимание, что x == fix16_one — это особый случай, где ответ равен 0.
  return fix16__log2_inner(x);
}

/**
* Это оболочка для fix16_log2, которая реализует арифметику насыщения.
*/
fix16_t fix16_slog2(fix16_t x)
{
  fix16_t retval = fix16_log2(x);
  // Переполнение возможно только в случае отрицательного входного значения.
  if (retval == fix16_overflow)
    return fix16_minimum;
  return retval;
}
