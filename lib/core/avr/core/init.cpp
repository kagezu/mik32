#ifdef __AVR_ATmega328P__
#include "init.h"
#include <Arduino.h>

void Core::init_clock()
{
  TCCR0B |= _BV(CS00); // Включить тактирование
}

void Core::init()
{
  init_clock();
}

#endif
