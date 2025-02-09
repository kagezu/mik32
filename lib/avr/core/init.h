#pragma once
#ifdef __AVR__
#include <Arduino.h>

void init_system();


void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif
