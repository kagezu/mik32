#pragma once
#include "mik32.h"

// init

void _init();
extern "C" void SmallSystemInit() { _init(); }

// ISR

void set_isr(void(*)());
void sei();
void cli();

// Delay

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
