#pragma once
#include "pins.h"

#ifndef SH_RST
#define SH_RST(x)
#endif
#ifndef SH_LD
#define SH_LD(x)
#endif

class Shift {
public:
  void init()
  {
    SH_LD(GPIO); SH_RST(GPIO); SH_SCK(GPIO); SH_SDA(GPIO);
    SH_LD.init(GPO_Max); SH_RST.init(GPO_Max); SH_SCK.init(GPO_Max); SH_SDA(IN);
    SH_LD.set(); SH_RST.set(); SH_SCK.clr();
  }

  inline void reset(bool hold = false) { SH_RST.clr(); if (!hold) SH_RST.set(); }
  inline void load() { SH_LD.clr(); SH_LD.set(); }
  inline void read_bytes(uint8_t *buffer, uint8_t length) { while (length--) *buffer++ = read(); }
  inline void write_bytes(uint8_t *buffer, uint8_t length) { while (length--) write(*buffer++); }

  uint8_t read()
  {
    uint8_t data = 0;
    uint8_t i = 8;
    while (i--) {
      data >>= 1;
      if (SH_SDA(GET)) data |= 0x80;
      SH_SCK.set();
      SH_SCK.clr();
    }
    return data;
  }

  void write(uint8_t data)
  {
    uint8_t i = 8;
    while (i--) {
      if (data & 0x80) SH_SDA.set();
      else  SH_SDA.clr();
      data <<= 1;
      SH_SCK.set();
      SH_SCK.clr();
    }
  }
};
