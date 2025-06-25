#include "fix16.h"
/* Алгоритм квадратного корня взят прямо из
* http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
* Важное отличие в том, что он разделен на две части
* чтобы использовать только 32-битные операции.
*
* Обратите внимание, что для отрицательных чисел мы возвращаем -sqrt(-inValue).
*/
fix16_t fix16_sqrt(fix16_t inValue)
{
  uint8_t neg = (inValue < 0);
  uint32_t num = fix_abs(inValue);
  uint32_t result = 0;
  uint32_t bit;
  uint8_t n;

  // Многие числа будут меньше 15, поэтому
  // это дает хороший баланс между временем, затраченным
  // в if, и временем, затраченным в цикле while
  // при поиске начального значения.

  if (num & 0xFFF00000)
    bit = (uint32_t)1 << 30;
  else
    bit = (uint32_t)1 << 18;

  while (bit > num)
    bit >>= 2;

  // Основная часть выполняется дважды, чтобы избежать
  // использования 64-битных значений в вычислениях.

  for (n = 0; n < 2; n++) {
    // Сначала получаем верхние 24 бита ответа.
    while (bit) {
      if (num >= result + bit) {
        num -= result + bit;
        result = (result >> 1) + bit;
      }
      else {
        result = (result >> 1);
      }
      bit >>= 2;
    }

    if (n == 0) {
      // Затем обрабатываем его снова, чтобы получить младшие 8 бит.
      if (num > 65535) {
        // Остаток 'num' слишком велик для сдвига влево
        // на 16, поэтому нам придется вручную добавить 1 к результату и
        // соответствующим образом скорректировать 'num'.
        // num = a - (result + 0.5)^2
        //	 = num + result^2 - (result + 0.5)^2
        //	 = num - result - 0.5
        num -= result;
        num = (num << 16) - 0x8000;
        result = (result << 16) + 0x8000;
      }
      else {
        num <<= 16;
        result <<= 16;
      }

      bit = 1 << 14;
    }
  }

#ifndef FIXMATH_NO_ROUNDING
  // Наконец, если бы следующий бит был 1, округлить результат в большую сторону.
  if (num > result) {
    result++;
  }
#endif

  return (neg ? -(fix16_t)result : (fix16_t)result);
}
