#pragma once
#include "mik32.h"

// ISR

void sei();
void cli();

// Delay

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
