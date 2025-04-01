#pragma once
#include "avr.h"
#include "macros/attribute.h"

void set_isr(uint8_t isr, void handler());

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);


void reset_system() GCC_NAKED;
