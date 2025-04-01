#include "init.h"

 GCC_INIT(0)
{
#ifdef __ATMmega328P__
  TCCR0B |= _BV(CS00); // Включить тактирование
#endif

#ifdef __ATMmega128__
  TCCRB |= _BV(CS00); // Включить тактирование
#endif
}

void reset_system()
{
  asm volatile("jmp 0");
}
