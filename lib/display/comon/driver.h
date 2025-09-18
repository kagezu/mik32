#pragma once
#include "inttypes.h"
#include "const.h"
#include "pgmspace.h"

class IDriver {
protected:
  void send_config(const uint8_t *config, int16_t size)
  {
    while (size > 0) {
      uint8_t count = pgm_read_byte(config++);
      uint8_t comand = pgm_read_byte(config++);
      size -= 2 + count;
      send_command(comand);
      while (count--) send_byte(pgm_read_byte(config++));
    }
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); send_word(x0); send_word(x1);
    send_command(RASET); send_word(y0); send_word(y1);
    send_command(RAMWR);
  }

  virtual void send_command(uint8_t command) = 0;
  virtual void send_byte(uint8_t data) = 0;
  virtual void send_word(uint16_t data) = 0;
};
