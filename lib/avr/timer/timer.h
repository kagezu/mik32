#pragma once

#ifdef __AVR_ATmega328P__
#include "timer_m328p.h"

#define OCR0                OCR0A
#define T0_COMP_ON          T0_COMPA_ON
#define TIMER0_COMP_vect    TIMER0_COMPA_vect
#endif

#ifdef __AVR_ATmega128__
#include "timer_m128.h"

#define OCR0A               OCR0
#define T0_COMPA_ON         T0_COMP_ON
#define TIMER0_COMPA_vect   TIMER0_COMP_vect
#endif
