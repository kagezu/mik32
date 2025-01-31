#pragma once
#ifdef MIK32V2
#include <mik32_memory_map.h>
#include <pad_config.h>
#include <gpio.h>

// Переопределение

#define F_CPU   OSC_SYSTEM_VALUE

// Управление портами

#define GPIO(port, pin)     PAD_CONFIG->PORT_ ## port ## _CFG &= ~(0b11 << ( pin << 1)) 
#define IN(port, pin)       GPIO_ ## port ->DIRECTION_IN |= 1 << pin
#define OUT(port, pin)      GPIO_ ## port ->DIRECTION_OUT |= 1 << pin
#define SET(port, pin)      GPIO_ ## port ->OUTPUT |= 1 << pin
#define RES(port, pin)      GPIO_ ## port ->OUTPUT &= ~(1 << pin)
#define GET(port, pin)      GPIO_ ## port ->STATE & (1 << pin)

#define MASK(port, pin)     (1 << pin)
#define MMO(port, pin)      (GPIO_ ## port ->OUTPUT)
#define MMI(port, pin)      (GPIO_ ## port ->STATE)


// Доступ к байтам

#define to_byte(w,x)  (((uint8_t *)&w)[x])

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

volatile void delay_us(uint32_t us);
volatile void delay_ms(uint32_t ms);


// Типы

typedef uint32_t reg;

// Прочее

#define PROGMEM
#define P(x)  x
#define pgm_read_byte  *(uint8_t *)
#define pgm_read_word  *(uint16_t *)


#endif
