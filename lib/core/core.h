#pragma once
#include <mik32_memory_map.h>
#include <pad_config.h>
#include <gpio.h>
#include <power_manager.h>
#include <wakeup.h>


// Управление портами

#define GPIO(port, pin)     PAD_CONFIG->PORT_ ## port ## _CFG &= ~(0b11 << ( pin << 1)) 
#define IN(port, pin)       GPIO_ ## port ->DIRECTION_IN |= 1 << pin
#define OUT(port, pin)      GPIO_ ## port ->DIRECTION_OUT |= 1 << pin
#define SET(port, pin)      GPIO_ ## port ->OUTPUT |= 1 << pin
#define RES(port, pin)      GPIO_ ## port ->OUTPUT &= ~(1 << pin)
#define GET(port, pin)      GPIO_ ## port ->STATE & (1 << pin)

#define MASK(port, pin)     (1 << pin)
#define SFR(port, pin)      (GPIO_ ## port ->OUTPUT)


// Доступ к байтам

#define to_byte(w,x)  (((byte *)&w)[x])

union dbyte {
  uint16_t word;
  uint8_t  byte[2];
  struct {
    uint8_t low;
    uint8_t high;
  };
};

union dword {
  uint32_t dword;
  uint16_t word[2];
  uint8_t  byte[4];
  struct {
    uint16_t low;
    uint16_t high;
  };
};

// Функции

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
