#pragma once
#include <Arduino.h>

void set_isr(uint8_t isr, void handler());

void init_system();


void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
