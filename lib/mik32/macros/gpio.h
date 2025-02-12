#pragma once
#include <mik32_memory_map.h>
#include <pad_config.h>
#include <gpio.h>

#define GPIO(port, pin)     PAD_CONFIG->PORT_ ## port ## _CFG &= ~(0b11 << ( pin << 1))
#define SER(port, pin)      PAD_CONFIG->PORT_ ## port ## _CFG |= (0b01 << ( pin << 1))
#define TIM(port, pin)      PAD_CONFIG->PORT_ ## port ## _CFG |= (0b10 << ( pin << 1))
#define AN(port, pin)       PAD_CONFIG->PORT_ ## port ## _CFG |= (0b11 << ( pin << 1))

#define DS_2MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b00 << ( pin << 1))
#define DS_4MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b01 << ( pin << 1))
#define DS_8MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b10 << ( pin << 1))

#define NC(port, pin)       PAD_CONFIG->PORT_ ## port ## _PUPD &= ~(0b11 << ( pin << 1))
#define VCC(port, pin)      PAD_CONFIG->PORT_ ## port ## _PUPD |= (0b01 << ( pin << 1))
#define GND(port, pin)      PAD_CONFIG->PORT_ ## port ## _PUPD |= (0b10 << ( pin << 1))

#define IN(port, pin)       GPIO_ ## port ->DIRECTION_IN |= 1 << pin
#define OUT(port, pin)      GPIO_ ## port ->DIRECTION_OUT |= 1 << pin

#define SET(port, pin)      GPIO_ ## port ->OUTPUT |= (1 << pin)
#define CLR(port, pin)      GPIO_ ## port ->OUTPUT &= ~(1 << pin)
#define INV(port, pin)      GPIO_ ## port ->OUTPUT ^= (1 << pin)
#define GET(port, pin)      (GPIO_ ## port ->STATE & (1 << pin))

#define MASK(port, pin)     (1 << pin)
#define MMO(port, pin)      (GPIO_ ## port ->OUTPUT)
#define MMI(port, pin)      (GPIO_ ## port ->STATE)
