#pragma once
#include <Arduino.h>

// Доступ к байтам

#define to_byte(w,x)  (((uint8_t *)&w)[x])

// Типы

typedef uint8_t reg;
typedef uint16_t adr;

// Прочее

#define P(x) PSTR(x)

#define I_SAVE uint8_t _sreg = SREG; __asm__ __volatile__ ("cli" :: )
#define I_REST SREG = _sreg
