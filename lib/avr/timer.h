#pragma once

#ifdef __AVR_ATmega328P__
#include "timer/timer_m328p.h"
#endif

#ifdef __AVR_ATmega128__
#include "timer/timer_m128.h"
#endif
