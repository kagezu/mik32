#pragma once
#include "core.h"
#include "accel.h"

// для mik32 должно быть чётным + 1 байт
#define PRINT_BUFFER_SIZE   33

class PrintF {
private:
  char buffer[PRINT_BUFFER_SIZE];
  static reg strlen(char *);

public:
  void printf(const char *, ...);
  void prints(const char *);
  void prints(char *, reg algin = 0);
  uint16_t h_print(uint8_t);

  virtual void putc(uint8_t) = 0;

  ///////////////////////////////

  template<typename T, typename I>
  char *sprint(char *ptr, T number, I digit, I fix)
  {
    T hi = number >> fix;
    T mod = hi;

    for (auto i = 0; i < digit; i++) {
      number = number - (mod << fix);
      number *= 10;
      mod = number >> fix;
      *--ptr = mod + '0';
    }
    *--ptr = '.';
    return sprint(ptr, hi);
  }

  template<typename T>
  char *sprint(char *ptr, T number)
  {
    bool neg = number < 0;
    number = neg ? -number : number;

    while (number > 9) {
      char mod;
    #ifdef __AVR__
      uint8_t tmp;
      div10_32bit(number, mod, tmp);
    #else
      mod = number % 10;
      number /= 10;
    #endif
      *--ptr = mod + '0';
    }
    *--ptr = number + '0';
    if (neg) *--ptr = '-';

    return ptr;
  }

  template<typename T, typename I>
  char *sprint(char *ptr, T number, I lng)
  {
    while (lng--) {
      ptr -= 2;
      *(uint16_t *)ptr = h_print(number);
      number >>= 8;
    }
    return ptr;
  }
};
