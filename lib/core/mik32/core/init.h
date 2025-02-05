#ifdef MIK32V2
#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <wakeup.h>

volatile void delay_us(uint32_t us);
volatile void delay_ms(uint32_t ms);
void cli();
void sei();

namespace Core {
  void init();
  void init_clock();
}

#endif
