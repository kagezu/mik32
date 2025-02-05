#pragma once
#ifdef __AVR__
#include <Arduino.h>

namespace Core {
  void init();
  void init_clock();
}

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif
