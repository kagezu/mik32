#pragma once
#include <inttypes.h>

#define F_CPU   OSC_SYSTEM_VALUE

// Доступ к байтам

#define to_byte(w,x)  (((uint8_t *)&w)[x])

// Типы

typedef uint32_t reg;
typedef uint32_t adr;

// Прочее

#define PROGMEM
#define P(x)  x
#define pgm_read_byte  *(uint8_t *)
#define pgm_read_word  *(uint16_t *)
