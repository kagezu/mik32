#pragma once
#include "pins.h"

#define SPI_MODE_0 0x00
#define SPI_MODE_1 0x04
#define SPI_MODE_2 0x08
#define SPI_MODE_3 0x0C


class SPI_Master {
public:
  SPI_Master() { TCCR0B |= _BV(CS00); } // Включить тактирование
  void init(uint16_t fq = 0xffff, uint8_t mode = SPI_MODE_0);
  void end() { SPI_STOP; }
  void send(uint8_t);
  uint8_t read(uint8_t);
  uint16_t transfer(uint16_t);
};

extern SPI_Master spi;
