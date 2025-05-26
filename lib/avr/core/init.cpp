#include "avr.h"
#include "macros/attribute.h"

GCC_INIT(0)
{
#ifdef __ATMmega328P__
  TCCR0B |= _BV(CS00); // Включить тактирование
#endif

#ifdef __ATMmega128__
  TCCRB |= _BV(CS00); // Включить тактирование
#endif
}
