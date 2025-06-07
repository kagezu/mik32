#ifndef __libfixmath_uint32_h__
#define __libfixmath_uint32_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

  /*! Выполняет беззнаковый лог-base2 для указанного беззнаковое целого числа и возвращает результат.
  */
  extern uint32_t uint32_log2(uint32_t inVal);

#ifdef __cplusplus
}
#endif

#endif
