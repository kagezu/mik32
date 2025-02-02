#ifdef __AVR__
#include "init.h"
#include <Arduino.h>

void Core::init_clock()
{
#ifdef __ATMmega328P__
  TCCR0B |= _BV(CS00); // Включить тактирование
#endif

#ifdef __ATMmega128__
  TCCRB |= _BV(CS00); // Включить тактирование
#endif
}

void Core::init()
{
  init_clock();
}

#endif
