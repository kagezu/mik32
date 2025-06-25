#ifndef __libfixmath_fixmath_h__
#define __libfixmath_fixmath_h__

#ifdef __cplusplus
extern "C"
{
#endif

  /*!
  \file fixmath.h
  \brief Функции для выполнения быстрых и точных математических операций с фиксированной точкой.
  */

  // Не использовать: самая точная версия, точность ~2,1%.
  // Использовать: быстрая реализация, работает на 159% быстрее, чем вышеприведенная «точная» версия, с немного меньшей точностью ~2,3%.
  // #define FIXMATH_FAST_SIN

  // Не использовать: Проверка на переполнение и возврат констант переполнения.
  // Использовать: Не проверять на переполнение.
  // #define FIXMATH_NO_OVERFLOW

  // Не использовать: Использовать округление.
  // Использовать: Не использовать округление.
  // #define FIXMATH_NO_ROUNDING


  // #define FIXMATH_SIN_LUT


#include "libfix16/uint32.h"
#include "libfix16/fract32.h"
#include "libfix16/fix16.h"

#ifdef __cplusplus
}
#include "libfix16/fix16.hpp"
#endif

#endif
