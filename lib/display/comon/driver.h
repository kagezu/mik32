#pragma once
#include "inttypes.h"

class IDriver {
protected:
  void send_config(const uint8_t *config, uint8_t size)
  {
    while (size) {
      uint8_t count = pgm_read_byte(config++);
      uint8_t comand = pgm_read_byte(config++);
      size -= 2 + count;
      send_command(comand);
      while (count--) send_byte(pgm_read_byte(config++));
    }
  }

  virtual void send_command(uint8_t command) = 0;
  virtual void send_byte(uint8_t data) = 0;
};
