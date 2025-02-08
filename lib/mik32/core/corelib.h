#pragma once
#include <inttypes.h> 

// Init

void  init_system();


// ISR

void set_isr(void(*)());
void sei();
void cli();

// Delay

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
