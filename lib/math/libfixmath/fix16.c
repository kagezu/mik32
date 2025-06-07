#include "fix16.h"
#include "int64.h"


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

#if !defined(FIXMATH_NO_64BIT) && !defined(FIXMATH_OPTIMIZE_8BIT)
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

  #ifndef FIXMATH_NO_ROUNDING
    // Эта корректировка необходима для правильного округления -1/2
    product--;
  #endif
  }
  else {
  #ifndef FIXMATH_NO_OVERFLOW
    if (upper)
      return fix16_overflow;
  #endif
  }

#ifdef FIXMATH_NO_ROUNDING
  return product >> 16;
#else
  fix16_t result = product >> 16;
  result += (product & 0x8000) >> 15;

  return result;
#endif
}
#endif

/* 32-битная реализация fix16_mul. Потенциально быстрая на 16-битных процессорах,
* и это относительно хороший компромисс для компиляторов, которые не поддерживают
* uint64_t. Использует 16*16->32-битные умножения.
*/
#if defined(FIXMATH_NO_64BIT) && !defined(FIXMATH_OPTIMIZE_8BIT)
fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1)
{
  // Each argument is divided to 16-bit parts.
  //					AB
  //			*	 CD
  // -----------
  //					BD	16 * 16 -> 32 bit products
  //				 CB
  //				 AD
  //				AC
  //			 |----| 64 bit product
  int32_t A = (inArg0 >> 16), C = (inArg1 >> 16);
  uint32_t B = (inArg0 & 0xFFFF), D = (inArg1 & 0xFFFF);

  int32_t AC = A * C;
  int32_t AD_CB = A * D + C * B;
  uint32_t BD = B * D;

  int32_t product_hi = AC + (AD_CB >> 16);

  // Handle carry from lower 32 bits to upper part of result.
  uint32_t ad_cb_temp = AD_CB << 16;
  uint32_t product_lo = BD + ad_cb_temp;
  if (product_lo < BD)
    product_hi++;

#ifndef FIXMATH_NO_OVERFLOW
  // The upper 17 bits should all be the same (the sign).
  if (product_hi >> 31 != product_hi >> 15)
    return fix16_overflow;
#endif

#ifdef FIXMATH_NO_ROUNDING
  return (product_hi << 16) | (product_lo >> 16);
#else
  // Вычитание 0x8000 (= 0,5) и последующее использование знакового сдвига вправо
  // обеспечивает правильное округление до результата-1, за исключением угла
  // случай отрицательных чисел и наименьшего слова = 0x8000.
  // Чтобы справиться с этим, нам также нужно вычесть 1 для отрицательных чисел.
  uint32_t product_lo_tmp = product_lo;
  product_lo -= 0x8000;
  product_lo -= (uint32_t)product_hi >> 31;
  if (product_lo > product_lo_tmp)
    product_hi--;

  // Отбрасываем самые младшие 16 бит. Обратите внимание, что это не совсем то же самое,
  // что и деление на 0x10000. Например, если произведение = -1, результат будет
  // также -1, а не 0. Это компенсируется добавлением +1 к результату
  // и компенсацией этого в свою очередь при округлении выше.
  fix16_t result = (product_hi << 16) | (product_lo >> 16);
  result += 1;
  return result;
#endif
}
#endif

/* 8-битная реализация fix16_mul. Самая быстрая, например, на Atmel AVR.
* Использует 8*8->16-битные умножения, а также пропускает любые байты, которые
* равны нулю.
*/
#if defined(FIXMATH_OPTIMIZE_8BIT)
fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1)
{
  uint32_t _a = fix_abs(inArg0);
  uint32_t _b = fix_abs(inArg1);

  uint8_t va[4] = { _a, (_a >> 8), (_a >> 16), (_a >> 24) };
  uint8_t vb[4] = { _b, (_b >> 8), (_b >> 16), (_b >> 24) };

  uint32_t low = 0;
  uint32_t mid = 0;

  // Столбец результата i зависит от va[0..i] и vb[i..0]

#ifndef FIXMATH_NO_OVERFLOW
// i = 6
  if (va[3] && vb[3]) return fix16_overflow;
#endif

  // i = 5
  if (va[2] && vb[3]) mid += (uint16_t)va[2] * vb[3];
  if (va[3] && vb[2]) mid += (uint16_t)va[3] * vb[2];
  mid <<= 8;

  // i = 4
  if (va[1] && vb[3]) mid += (uint16_t)va[1] * vb[3];
  if (va[2] && vb[2]) mid += (uint16_t)va[2] * vb[2];
  if (va[3] && vb[1]) mid += (uint16_t)va[3] * vb[1];

#ifndef FIXMATH_NO_OVERFLOW
  if (mid & 0xFF000000) return fix16_overflow;
#endif
  mid <<= 8;

  // i = 3
  if (va[0] && vb[3]) mid += (uint16_t)va[0] * vb[3];
  if (va[1] && vb[2]) mid += (uint16_t)va[1] * vb[2];
  if (va[2] && vb[1]) mid += (uint16_t)va[2] * vb[1];
  if (va[3] && vb[0]) mid += (uint16_t)va[3] * vb[0];

#ifndef FIXMATH_NO_OVERFLOW
  if (mid & 0xFF000000) return fix16_overflow;
#endif
  mid <<= 8;

  // i = 2
  if (va[0] && vb[2]) mid += (uint16_t)va[0] * vb[2];
  if (va[1] && vb[1]) mid += (uint16_t)va[1] * vb[1];
  if (va[2] && vb[0]) mid += (uint16_t)va[2] * vb[0];

  // i = 1
  if (va[0] && vb[1]) low += (uint16_t)va[0] * vb[1];
  if (va[1] && vb[0]) low += (uint16_t)va[1] * vb[0];
  low <<= 8;

  // i = 0
  if (va[0] && vb[0]) low += (uint16_t)va[0] * vb[0];

#ifndef FIXMATH_NO_ROUNDING
  low += 0x8000;
#endif
  mid += (low >> 16);

#ifndef FIXMATH_NO_OVERFLOW
  if (mid & 0x80000000)
    return fix16_overflow;
#endif

  fix16_t result = mid;

  /* Figure out the sign of result */
  if ((inArg0 >= 0) != (inArg1 >= 0)) {
    result = -result;
  }

  return result;
}
#endif

#ifndef FIXMATH_NO_OVERFLOW
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
#endif

/* 32-битная реализация fix16_div. Самая быстрая версия, например, для ARM Cortex M3.
* Выполняет 32-битные деления повторно, чтобы уменьшить остаток. Чтобы это
* было эффективно, процессор должен иметь 32-битное аппаратное деление.
*/
#if !defined(FIXMATH_NO_HARD_DIVISION)
#ifdef __GNUC__
// Подсчитаем начальные нули, используя специфичную для процессора инструкцию, если она доступна.
#define clz(x) (__builtin_clzl(x) - (8 * sizeof(long) - 32))
#else
static uint8_t clz(uint32_t x)
{
  uint8_t result = 0;
  if (x == 0) return 32;
  while (!(x & 0xF0000000)) { result += 4; x <<= 4; }
  while (!(x & 0x80000000)) { result += 1; x <<= 1; }
  return result;
}
#endif

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

#ifndef FIXMATH_NO_ROUNDING
  // Частное всегда положительно, поэтому округление выполняется легко
  quotient++;
#endif

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
#endif /* !defined(FIXMATH_NO_HARD_DIVISION) */

/* Альтернативная 32-битная реализация fix16_div. Самая быстрая, например, на Atmel AVR.
* Это делает деление вручную, и поэтому подходит для процессоров, которые
* не имеют аппаратного деления.
*/
#if defined(FIXMATH_NO_HARD_DIVISION)
fix16_t fix16_div(fix16_t a, fix16_t b)
{
  // Здесь используется базовый алгоритм восстанавливающего двоичного деления.
  // Кажется, быстрее выполнить все деление вручную, чем
  // пытаться составить 64-битное деление из 32-битных делений на
  // платформах без аппаратного деления.

  if (b == 0)
    return fix16_minimum;

  uint32_t remainder = fix_abs(a);
  uint32_t divider = fix_abs(b);

  uint32_t quotient = 0;
  uint32_t bit = 0x10000;

  /* Алгоритм требует D >= R */
  while (divider < remainder) {
    divider <<= 1;
    bit <<= 1;
  }

#ifndef FIXMATH_NO_OVERFLOW
  if (!bit)
    return fix16_overflow;
#endif

  if (divider & 0x80000000) {
    // Выполните один шаг вручную, чтобы избежать переполнения позже.
    // Мы знаем, что нижний бит делителя здесь равен 0.
    if (remainder >= divider) {
      quotient |= bit;
      remainder -= divider;
    }
    divider >>= 1;
    bit >>= 1;
  }

  /* Основной цикл деления */
  while (bit && remainder) {
    if (remainder >= divider) {
      quotient |= bit;
      remainder -= divider;
    }

    remainder <<= 1;
    bit >>= 1;
  }

#ifndef FIXMATH_NO_ROUNDING
  if (remainder >= divider) {
    quotient++;
  }
#endif

  fix16_t result = quotient;

  /* Выясняем знак результата */
  if ((a ^ b) & 0x80000000) {
  #ifndef FIXMATH_NO_OVERFLOW
    if (result == fix16_minimum)
      return fix16_overflow;
  #endif

    result = -result;
  }

  return result;
}
#endif /* defined(FIXMATH_NO_HARD_DIVISION) */

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
#ifdef FIXMATH_NO_HARD_DIVISION
  /* Причина, по которой мы делаем это, а не используем оператор по модулю,
  * заключается в том, что если у вас нет аппаратного делителя, это приведет к
  * более быстрым операциям, когда углы близки к границам.
  */
  while (x >= y) x -= y;
  while (x <= -y) x += y;
#else
  /* Обратите внимание, что в C90 знак результата операции по модулю
  * не определен. В C99 он такой же, как делимое (он же числитель).
  */
  x %= y;
#endif

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
