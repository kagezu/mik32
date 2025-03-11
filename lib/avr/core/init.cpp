#include "init.h"

void init_system()
{
#ifdef __ATMmega328P__
  TCCR0B |= _BV(CS00); // Включить тактирование
#endif

#ifdef __ATMmega128__
  TCCRB |= _BV(CS00); // Включить тактирование
#endif
}
