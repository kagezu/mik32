#pragma once
#include "mik32.h"

// ISR

#define ISR extern "C" __attribute__((used, interrupt, section(".trap_text"))) void trap_handler()

void sei();
void cli();

// Delay

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

// String

char *sjoin(char *, const char *);
