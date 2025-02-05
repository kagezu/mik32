#ifdef __AVR__
#pragma once
#include "pins.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C


class SPI_Master {
public:
  SPI_Master() {}
  void wait() { while (!(SPSR & _BV(SPIF))); }
  void init(uint16_t fq = 0xffff, uint8_t mode = SPI_MODE0);
  void end() { SPI_MOSI(IN); SPI_SCK(IN); SPI_MISO(IN); SPCR = 0; }
  void send(uint8_t data) { wait(); SPDR = data; }
  void send16(uint16_t);
  uint8_t transfer(uint8_t);
  uint16_t transfer16(uint16_t);

  // для буферизации
  inline  uint8_t read() { return SPDR; }
  inline  uint16_t read16() { return SPDR; }
  inline  uint32_t read32() { return SPDR; }
  inline  void  write(uint8_t data) { SPDR = data; }
  inline  void  write16(uint16_t data) { SPDR = data >> 8; __asm__ __volatile__("nop"::); wait(); SPDR = data; }
  inline  void  write32(uint32_t data) {}
};

// extern SPI_Master SPI;

#endif
