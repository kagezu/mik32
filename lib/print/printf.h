// #include "printf.h"
#pragma once
#include "core.h"
#ifdef __AVR__
#include "accel.h"
#endif


class PrintF {
private:
  static char buffer[];
  static char strlen(char *);

public:
  void printf(const char *, ...);
  void prints(const char *);
  void prints(char *, char algin = 0);
  uint16_t h_print(uint8_t);

  virtual void putc(uint8_t) = 0;

  ///////////////////////////////

  template <typename T, typename I>
  char *sprint(char *ptr, T number, I digit, I fix) {
    bool neg = number < 0;
    if (neg) number = -number;
    T hi = number >> fix;
    T mod = hi;
    ptr -= digit;

    for (auto i = 0; i < digit; i++) {
      number = number - (mod << fix);
      number *= 10;
      mod = number >> fix;
      ptr[i] = mod + '0';
    }
    *--ptr = '.';
    while (hi > 9) {
      mod = hi % 10;
      hi /= 10;
      *--ptr = mod + '0';
    }
    *--ptr = hi + '0';
    if (neg) *--ptr = '-';
    return ptr;
  }

  template <typename T>
  char *sprint(char *ptr, T number) {
    bool neg = number < 0;
    number = neg ? -number : number;

    while (number > 9) {
      char mod;
#ifdef __AVR__
      uint8_t tmp;
      div10_16bit(number, mod, tmp);
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

  template <typename T, typename I>
  char *sprint(char *ptr, T number, I lng) {
    while (lng--) {
      ptr -= 2;
      *(uint16_t *)ptr = h_print(number);
      number >>= 8;
    }
    return ptr;
  }
};
