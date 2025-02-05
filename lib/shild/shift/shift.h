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
    SH_LD(OUT); SH_RST(OUT); SH_SCK(OUT); SH_SDA(IN);
    SH_LD(SET); SH_RST(SET); SH_SCK(CLR);
  }

  inline void reset(bool hold = false) { SH_RST(CLR); if (!hold) SH_RST(SET); }
  inline void load() { SH_LD(CLR); SH_LD(SET); }
  inline void read_bytes(uint8_t *buffer, uint8_t length) { while (length--) *buffer++ = read(); }
  inline void write_bytes(uint8_t *buffer, uint8_t length) { while (length--) write(*buffer++); }

  uint8_t read()
  {
    uint8_t data = 0;
    uint8_t i = 8;
    while (i--) {
      data >>= 1;
      if (SH_SDA(GET)) data |= 0x80;
      SH_SCK(SET);
      SH_SCK(CLR);
    }
    return data;
  }

  void write(uint8_t data)
  {
    uint8_t i = 8;
    while (i--) {
      if (data & 0x80) SH_SDA(SET);
      else  SH_SDA(CLR);
      data <<= 1;
      SH_SCK(SET);
      SH_SCK(CLR);
    }
  }
};
