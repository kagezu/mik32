#pragma once
#ifdef __AVR__
#include "avr.h"
#else
#include <stdint.h>
#define PROGMEM
#endif

struct Font {
  uint8_t count_char;
  uint8_t first_char;
  uint8_t weight;
  uint8_t height;
  const uint16_t *offset;
  const uint8_t *w;
  const uint8_t *data;
  // uint8_t byte_per_char;
};

// struct Font {
//   uint8_t count_char;
//   uint8_t first_char;
//   uint8_t weight;
//   uint8_t height;
//   const uint8_t *offset;
//   const uint8_t *data;
// };
