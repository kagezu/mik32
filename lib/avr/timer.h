#pragma once

#ifdef __AVR_ATmega328P__
#include "timer/timer_m328p.h"
#endif

#ifdef __AVR_ATmega128__
#include "timer/timer_m128.h"
#endif


//test

#define T32_0_PS
#define T32_0_EN   T1_DIV_1024
#define T32_0_C   TCNT1 = 0
#define T32_0     ((uint32_t)TCNT1 << 10)
