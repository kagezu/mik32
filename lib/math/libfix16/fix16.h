#ifndef __libfixmath_fix16_h__
#define __libfixmath_fix16_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

  typedef int32_t fix16_t;

  static const fix16_t FOUR_DIV_PI = 0x145F3;             /*!< Fix16 value of 4/PI */
  static const fix16_t _FOUR_DIV_PI2 = 0xFFFF9840;        /*!< Fix16 value of -4/PI² */
  static const fix16_t X4_CORRECTION_COMPONENT = 0x399A; 	/*!< Fix16 value of 0.225 */
  static const fix16_t PI_DIV_4 = 0x0000C90F;             /*!< Fix16 value of PI/4 */
  static const fix16_t THREE_PI_DIV_4 = 0x00025B2F;       /*!< Fix16 value of 3PI/4 */

  static const fix16_t fix16_maximum = 0x7FFFFFFF;    /*!< the maximum value of fix16_t */
  static const fix16_t fix16_minimum = 0x80000000;    /*!< the minimum value of fix16_t */
  static const fix16_t fix16_overflow = 0x80000000;   /*!< the value used to indicate overflows when FIXMATH_NO_OVERFLOW is not specified */

  static const fix16_t fix16_pi = 205887;       /*!< fix16_t value of pi */
  static const fix16_t fix16_e = 178145;        /*!< fix16_t value of e */
  static const fix16_t fix16_one = 0x00010000;  /*!< fix16_t value of 1 */
  static const fix16_t fix16_eps = 1;           /*!< fix16_t epsilon */

  /* Функции преобразования между fix16_t и float/integer.
  * Они встроены, чтобы позволить компилятору оптимизировать константные числа
  */
  static inline fix16_t fix16_from_int(int a) { return a * fix16_one; }
  static inline float   fix16_to_float(fix16_t a) { return (float)a / fix16_one; }
  static inline double  fix16_to_dbl(fix16_t a) { return (double)a / fix16_one; }

  static inline int fix16_to_int(fix16_t a)
  {
  #ifdef FIXMATH_NO_ROUNDING
    return (a >> 16);
  #else
    if (a >= 0)
      return (a + (fix16_one >> 1)) / fix16_one;
    return (a - (fix16_one >> 1)) / fix16_one;
  #endif
  }

  static inline fix16_t fix16_from_float(float a)
  {
    float temp = a * fix16_one;
  #ifndef FIXMATH_NO_ROUNDING
    temp += (temp >= 0) ? 0.5f : -0.5f;
  #endif
    return (fix16_t)temp;
  }

  static inline fix16_t fix16_from_dbl(double a)
  {
    double temp = a * fix16_one;
    /* F16() и F16C() всегда округляют, так что и это должно быть так же */
    //#ifndef FIXMATH_NO_ROUNDINGAdd commentMore actions
    temp += (double)((temp >= 0) ? 0.5f : -0.5f);
    //#endif
    return (fix16_t)temp;
  }

  /* Макрос для определения константных значений fix16_t.
  Функции выше не могут быть использованы, например, из инициализаторов глобальных переменных,
  и их имена также довольно длинные. Этот макрос полезен для констант,
  прикрепленных вместе с кодом, например, F16(1.234).

  Обратите внимание, что аргумент вычисляется несколько раз, и также в противном случае
  вы должны использовать это только для константных значений. Для преобразований во время выполнения
  используйте функции выше.
  */
#define F16(x) ((fix16_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))

  static inline fix16_t fix16_abs(fix16_t x)
  {
    return (fix16_t)(x < 0 ? -(uint32_t)x : (uint32_t)x);
  }
  static inline fix16_t fix16_floor(fix16_t x)
  {
    return (x & 0xFFFF0000UL);
  }
  static inline fix16_t fix16_ceil(fix16_t x)
  {
    return (x & 0xFFFF0000UL) + (x & 0x0000FFFFUL ? fix16_one : 0);
  }
  static inline fix16_t fix16_min(fix16_t x, fix16_t y)
  {
    return (x < y ? x : y);
  }
  static inline fix16_t fix16_max(fix16_t x, fix16_t y)
  {
    return (x > y ? x : y);
  }
  static inline fix16_t fix16_clamp(fix16_t x, fix16_t lo, fix16_t hi)
  {
    return fix16_min(fix16_max(x, lo), hi);
  }

  /* Вычитание и сложение с (опциональным) обнаружением переполнения. */
#ifdef FIXMATH_NO_OVERFLOW

  static inline fix16_t fix16_add(fix16_t inArg0, fix16_t inArg1) { return (inArg0 + inArg1); }
  static inline fix16_t fix16_sub(fix16_t inArg0, fix16_t inArg1) { return (inArg0 - inArg1); }

#else

  extern fix16_t fix16_add(fix16_t a, fix16_t b);
  extern fix16_t fix16_sub(fix16_t a, fix16_t b);

  /* Saturating arithmetic */
  extern fix16_t fix16_sadd(fix16_t a, fix16_t b);
  extern fix16_t fix16_ssub(fix16_t a, fix16_t b);

#endif

  /*! Умножает два заданных fix16_t и возвращает результат.
  */
  extern fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1);

  /*! Делит первый заданный fix16_t на второй и возвращает результат.
  */
  extern fix16_t fix16_div(fix16_t inArg0, fix16_t inArg1);

#ifndef FIXMATH_NO_OVERFLOW
  /*! Выполняет насыщенное умножение (с защитой от переполнения) двух заданных fix16_t и возвращает результат.
  */
  extern fix16_t fix16_smul(fix16_t inArg0, fix16_t inArg1);

  /*! Выполняет насыщенное деление (с защитой от переполнения) первого fix16_t на второй и возвращает результат.
  */
  extern fix16_t fix16_sdiv(fix16_t inArg0, fix16_t inArg1);
#endif

  /*! Делит первый заданный fix16_t на второй и возвращает результат.
  */
  extern fix16_t fix16_mod(fix16_t x, fix16_t y);



  /*! Возвращает линейную интерполяцию: (inArg0 * (1 - inFract)) + (inArg1 * inFract)
  */
  extern fix16_t fix16_lerp8(fix16_t inArg0, fix16_t inArg1, uint8_t inFract);
  extern fix16_t fix16_lerp16(fix16_t inArg0, fix16_t inArg1, uint16_t inFract);
  extern fix16_t fix16_lerp32(fix16_t inArg0, fix16_t inArg1, uint32_t inFract);



  /*! Returns the sine of the given fix16_t.
  */
  extern fix16_t fix16_sin_parabola(fix16_t inAngle);

  /*! Returns the sine of the given fix16_t.
  */
  extern fix16_t fix16_sin(fix16_t inAngle);

  /*! Returns the cosine of the given fix16_t.
  */
  extern fix16_t fix16_cos(fix16_t inAngle);

  /*! Returns the tangent of the given fix16_t.
  */
  extern fix16_t fix16_tan(fix16_t inAngle);

  /*! Returns the arcsine of the given fix16_t.
  */
  extern fix16_t fix16_asin(fix16_t inValue);

  /*! Returns the arccosine of the given fix16_t.
  */
  extern fix16_t fix16_acos(fix16_t inValue);

  /*! Returns the arctangent of the given fix16_t.
  */
  extern fix16_t fix16_atan(fix16_t inValue);

  /*! Returns the arctangent of inY/inX.
  */
  extern fix16_t fix16_atan2(fix16_t inY, fix16_t inX);

  static const fix16_t fix16_rad_to_deg_mult = 3754936;
  static inline fix16_t fix16_rad_to_deg(fix16_t radians)
  {
    return fix16_mul(radians, fix16_rad_to_deg_mult);
  }

  static const fix16_t fix16_deg_to_rad_mult = 1144;
  static inline fix16_t fix16_deg_to_rad(fix16_t degrees)
  {
    return fix16_mul(degrees, fix16_deg_to_rad_mult);
  }



  /*! Returns the square root of the given fix16_t.
  */
  extern fix16_t fix16_sqrt(fix16_t inValue);

  /*! Returns the square of the given fix16_t.
  */
  static inline fix16_t fix16_sq(fix16_t x)
  {
    return fix16_mul(x, x);
  }

  /*! Returns the exponent (e^) of the given fix16_t.
  */
  extern fix16_t fix16_exp(fix16_t inValue);

  /*! Returns the natural logarithm of the given fix16_t.
   */
  extern fix16_t fix16_log(fix16_t inValue);

  /*! Returns the base 2 logarithm of the given fix16_t.
   */
  extern fix16_t fix16_log2(fix16_t x);

  /*! Returns the saturated base 2 logarithm of the given fix16_t.
   */
  extern fix16_t fix16_slog2(fix16_t x);

  /*! Преобразовать значение fix16_t в строку.
  * Требуемая длина буфера для самых больших значений — 13 байт.
  */
  extern void fix16_to_str(fix16_t value, char *buf, int decimals);

  /*! Преобразовать строку в значение fix16_t
  * Игнорирует пробелы в начале и конце. Возвращает fix16_overflow, если
  * значение слишком велико или есть мусорные символы.
  */
  extern fix16_t fix16_from_str(const char *buf);

  static inline uint32_t fix_abs(fix16_t in)
  {
    if (in == fix16_minimum) {
      // минимальное отрицательное число имеет то же представление, что и
      // его абсолютное значение в беззнаковом
      return 0x80000000;
    }
    else {
      return ((in >= 0) ? (in) : (-in));
    }
  }


  /** Вспомогательный макрос для F16C. Заменить токен на его количество символов/цифр. */
#define FIXMATH_TOKLEN(token) ( sizeof( #token ) - 1 )

/** Helper macro for F16C. Handles pow(10, n) for n from 0 to 8. */
#define FIXMATH_CONSTANT_POW10(times) ( \
  (times == 0) ? 1ULL \
        : (times == 1) ? 10ULL \
            : (times == 2) ? 100ULL \
                : (times == 3) ? 1000ULL \
                    : (times == 4) ? 10000ULL \
                        : (times == 5) ? 100000ULL \
                            : (times == 6) ? 1000000ULL \
                                : (times == 7) ? 10000000ULL \
                                    : 100000000ULL \
)


/** Вспомогательный макрос для F16C, тип uint64_t используется только во время компиляции и
* не должен быть виден в сгенерированном коде.
*
* @note Мы не используем fix16_one вместо 65536ULL, потому что
* "использование константной переменной в константном выражении нестандартно в C".
*/
#define FIXMATH_CONVERT_MANTISSA(m) \
( (unsigned) \
    ( \
        ( \
            ( \
                (uint64_t)( ( ( 1 ## m ## ULL ) - FIXMATH_CONSTANT_POW10(FIXMATH_TOKLEN(m)) ) * FIXMATH_CONSTANT_POW10(5 - FIXMATH_TOKLEN(m)) ) \
                * 100000ULL * 65536ULL \
            ) \
            + 5000000000ULL /* rounding: + 0.5 */ \
        ) \
        / \
        10000000000LL \
    ) \
)


#define FIXMATH_COMBINE_I_M(i, m) \
( \
    ( \
        (    i ) \
        << 16 \
    ) \
    | \
    ( \
        FIXMATH_CONVERT_MANTISSA(m) \
        & 0xFFFF \
    ) \
)


/** Создать константу int16_t (Q16.16) из отдельных частей целого числа и мантиссы.
*
* Протестировано только на 32-битном ARM Cortex-M0 / x86 Intel.
*
* Этот макрос необходим при компиляции с параметрами типа "--fpu=none",
* которые запрещают любое использование float и связанных типов и
* таким образом сделают невозможным использование констант fix16_t.
*
* Просто замените использование F16() на F16C() следующим образом:
* F16(123.1234) становится F16C(123,1234)
*
* @warning Указание любого значения за пределами указанных интервалов
* ПРИВЕДЕТ к неопределенному поведению!
*
* @note Независимо от указанных ниже минимальных и максимальных значений для i и m,
* общее значение числа, представленного i и m, ДОЛЖНО находиться в интервале
* ]-32768.00000:32767.99999[, в противном случае использование с этим макросом приведет к неопределенному поведению.
*
* @param i Целочисленная константа со знаком со значением в интервале ]-32768:32767[.
* @param m Положительная целочисленная константа в интервале ]0:99999[ (дробная часть/мантисса).
*/
#define F16C(i, m) \
( (fix16_t) \
    ( \
      (( #i[0] ) == '-') \
        ? -FIXMATH_COMBINE_I_M((unsigned)( ( (i) * -1) ), m) \
        : FIXMATH_COMBINE_I_M((unsigned)i, m) \
    ) \
)

#ifdef __cplusplus
}
#endif

#endif
