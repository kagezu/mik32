#ifndef __libfixmath_fract32_h__
#define __libfixmath_fract32_h__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

  typedef uint32_t fract32_t;

  /*! Создает дробь, используя беззнаковые целые числа.
  \param inNumerator беззнаковый целочисленный числитель
  \param inDenominator беззнаковый целочисленный знаменатель
  \return дробь, используя заданные числитель и знаменатель
  */
  extern fract32_t fract32_create(uint32_t inNumerator, uint32_t inDenominator);

  /*! Инвертирует заданную дробь, меняя местами числитель и знаменатель.
  */
  extern fract32_t fract32_invert(fract32_t inFract);

#ifndef FIXMATH_NO_64BIT
  /*! Выполняет беззнаковое насыщенное (защищенное от переполнения) умножение с двумя заданными дробями и возвращает результат как беззнаковое целое число.
  */
  extern uint32_t  fract32_usmul(uint32_t inVal, fract32_t inFract);

  /*! Выполняет насыщенное (защищенное от переполнения) умножение двух заданных дробей и возвращает результат как целое число со знаком.
  */
  extern int32_t   fract32_smul(int32_t inVal, fract32_t inFract);
#endif

#ifdef __cplusplus
}
#endif

#endif
