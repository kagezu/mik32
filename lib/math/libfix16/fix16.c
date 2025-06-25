#include "fix16.h"

/* Вычитание и сложение с обнаружением переполнения.
* Версии без обнаружения переполнения inline.
*/
#ifndef FIXMATH_NO_OVERFLOW
fix16_t fix16_add(fix16_t a, fix16_t b)
{
  // Используйте беззнаковое целые числа, поскольку переполнение со знаковыми целыми числами является
  // неопределенной операцией (http://www.airs.com/blog/archives/120).
  uint32_t _a = a;
  uint32_t _b = b;
  uint32_t sum = _a + _b;

  // Переполнение может произойти только если знак a == знак b, и тогда
  // это вызывает знак суммы != знак a.
  if (!((_a ^ _b) & 0x80000000) && ((_a ^ sum) & 0x80000000))
    return fix16_overflow;

  return sum;
}

fix16_t fix16_sub(fix16_t a, fix16_t b)
{
  uint32_t _a = a;
  uint32_t _b = b;
  uint32_t diff = _a - _b;

  // Переполнение может произойти только если знак a != знак b, и тогда
  // это вызывает знак diff != знак a.
  if (((_a ^ _b) & 0x80000000) && ((_a ^ diff) & 0x80000000))
    return fix16_overflow;

  return diff;
}

/* Насыщающая арифметика */
fix16_t fix16_sadd(fix16_t a, fix16_t b)
{
  fix16_t result = fix16_add(a, b);

  if (result == fix16_overflow)
    return (a >= 0) ? fix16_maximum : fix16_minimum;

  return result;
}

fix16_t fix16_ssub(fix16_t a, fix16_t b)
{
  fix16_t result = fix16_sub(a, b);

  if (result == fix16_overflow)
    return (a >= 0) ? fix16_maximum : fix16_minimum;

  return result;
}
#endif



/* 64-битная реализация для fix16_mul. Самая быстрая версия.
* Выполняет умножение 32*32 -> 64 бит. Средние 32 бита являются результатом,
* нижние 16 бит используются для округления, а верхние 16 бит используются для обнаружения переполнения.
*/

fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1)
{
  int64_t product = (int64_t)inArg0 * inArg1;

#ifndef FIXMATH_NO_OVERFLOW
  // Все старшие 17 бит должны быть одинаковыми (знак).
  uint32_t upper = (product >> 47);
#endif

  if (product < 0) {
  #ifndef FIXMATH_NO_OVERFLOW
    if (~upper)
      return fix16_overflow;
  #endif
  }
  else {
  #ifndef FIXMATH_NO_OVERFLOW
    if (upper)
      return fix16_overflow;
  #endif
  }
  return product >> 16;
}

/* Обертка вокруг fix16_mul для добавления насыщающей арифметики. */
fix16_t fix16_smul(fix16_t inArg0, fix16_t inArg1)
{
  fix16_t result = fix16_mul(inArg0, inArg1);

  if (result == fix16_overflow) {
    if ((inArg0 >= 0) == (inArg1 >= 0))
      return fix16_maximum;
    else
      return fix16_minimum;
  }

  return result;
}

/* 32-битная реализация fix16_div. Самая быстрая версия, например, для ARM Cortex M3.
* Выполняет 32-битные деления повторно, чтобы уменьшить остаток. Чтобы это
* было эффективно, процессор должен иметь 32-битное аппаратное деление.
*/

static uint8_t clz(uint32_t x)
{
  uint8_t result = 0;
  if (x == 0) return 32;
  while (!(x & 0xF0000000)) { result += 4; x <<= 4; }
  while (!(x & 0x80000000)) { result += 1; x <<= 1; }
  return result;
}

fix16_t fix16_div(fix16_t a, fix16_t b)
{
  // Это использует аппаратное 32/32-битное деление несколько раз, пока мы не
  // вычислим все биты в (a<<17)/b. Обычно это занимает 1-3 итерации.

  if (b == 0)
    return fix16_minimum;

  uint32_t remainder = fix_abs(a);
  uint32_t divider = fix_abs(b);
  uint64_t quotient = 0;
  int bit_pos = 17;

  // Немного ускорить деление.
  // Это повышает скорость в худших сценариях, где N и D велики
  // Это дает более низкую оценку результата на N/(D >> 17 + 1).
  if (divider & 0xFFF00000) {
    uint32_t shifted_div = ((divider >> 17) + 1);
    quotient = remainder / shifted_div;
    uint64_t tmp = ((uint64_t)quotient * (uint64_t)divider) >> 17;
    remainder -= (uint32_t)(tmp);
  }

  // Если делитель делится на 2^n, воспользуйтесь этим.
  while (!(divider & 0xF) && bit_pos >= 4) {
    divider >>= 4;
    bit_pos -= 4;
  }

  while (remainder && bit_pos >= 0) {
    // Сдвигаем остаток настолько, насколько это возможно без переполнения
    int shift = clz(remainder);
    if (shift > bit_pos) shift = bit_pos;
    remainder <<= shift;
    bit_pos -= shift;

    uint32_t div = remainder / divider;
    remainder = remainder % divider;
    quotient += (uint64_t)div << bit_pos;

  #ifndef FIXMATH_NO_OVERFLOW
    if (div & ~(0xFFFFFFFF >> bit_pos))
      return fix16_overflow;
  #endif

    remainder <<= 1;
    bit_pos--;
  }

  fix16_t result = quotient >> 1;

  // Figure out the sign of the result
  if ((a ^ b) & 0x80000000) {
  #ifndef FIXMATH_NO_OVERFLOW
    if (result == fix16_minimum)
      return fix16_overflow;
  #endif

    result = -result;
  }

  return result;
}


#ifndef FIXMATH_NO_OVERFLOW
/* Обертка вокруг fix16_div для добавления насыщающей арифметики. */
fix16_t fix16_sdiv(fix16_t inArg0, fix16_t inArg1)
{
  fix16_t result = fix16_div(inArg0, inArg1);

  if (result == fix16_overflow) {
    if ((inArg0 >= 0) == (inArg1 >= 0))
      return fix16_maximum;
    else
      return fix16_minimum;
  }

  return result;
}
#endif

fix16_t fix16_mod(fix16_t x, fix16_t y)
{
  x %= y;
  return x;
}

fix16_t fix16_lerp8(fix16_t inArg0, fix16_t inArg1, uint8_t inFract)
{
  int64_t tempOut = int64_mul_i32_i32(inArg0, (((int32_t)1 << 8) - inFract));
  tempOut = int64_add(tempOut, int64_mul_i32_i32(inArg1, inFract));
  tempOut = int64_shift(tempOut, -8);
  return (fix16_t)int64_lo(tempOut);
}

fix16_t fix16_lerp16(fix16_t inArg0, fix16_t inArg1, uint16_t inFract)
{
  int64_t tempOut = int64_mul_i32_i32(inArg0, (((int32_t)1 << 16) - inFract));
  tempOut = int64_add(tempOut, int64_mul_i32_i32(inArg1, inFract));
  tempOut = int64_shift(tempOut, -16);
  return (fix16_t)int64_lo(tempOut);
}

fix16_t fix16_lerp32(fix16_t inArg0, fix16_t inArg1, uint32_t inFract)
{
  if (inFract == 0)
    return inArg0;
  int64_t inFract64 = int64_const(0, inFract);
  int64_t subbed = int64_sub(int64_const(1, 0), inFract64);
  int64_t tempOut = int64_mul_i64_i32(subbed, inArg0);
  tempOut = int64_add(tempOut, int64_mul_i64_i32(inFract64, inArg1));
  return int64_hi(tempOut);
}
