#pragma once
#include <core.h>

struct Font {
  uint8_t count_char;
  uint8_t first_char;
  uint8_t weight;
  uint8_t height;
  const uint8_t *offset;
  const uint8_t *data;
};
