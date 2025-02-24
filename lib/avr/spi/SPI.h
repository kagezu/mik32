#pragma once
#include "macros/avr.h"
#include "pins.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_DIV_4   0x00
#define SPI_DIV_16  0x01
#define SPI_DIV_64  0x02
#define SPI_DIV_128 0x03

#define SPI_MSBFIRST    0
#define SPI_LSBFIRST    _BV(DORD)

class SPI_Settings {
public:
  SPI_Settings() {}
  SPI_Settings(uint16_t fq, uint8_t bit = MSBFIRST, uint8_t mode = SPI_MODE0) { init(fq, bit, mode); }

  void init(uint16_t fq = 0xffff, uint8_t bit = MSBFIRST, uint8_t mode = SPI_MODE0)
  {
    uint8_t sck = 0, spi2x = 0;

    if (fq >= F_CPU / 2000) { sck = SPI_DIV_4; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 4000) sck = SPI_DIV_4;
    else if (fq >= F_CPU / 8000) { sck = SPI_DIV_16; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 16000) sck = SPI_DIV_16;
    else if (fq >= F_CPU / 32000) { sck = SPI_DIV_64; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 64000) sck = SPI_DIV_64;
    else sck = SPI_DIV_128;

    spcr = _BV(SPE) | _BV(MSTR) | mode | sck | bit;
    spsr = spi2x;
  }

private:
  uint8_t spcr;
  uint8_t spsr;

  friend class SPI_Master;
};

class SPI_Master {
public:
  // Инициализация

  SPI_Master() {}
  void begin() { SPI_SS(OUT); SPI_MOSI(OUT); SPI_SCK(OUT); SPI_MISO(IN); }
  void end() { SPI_MOSI(IN); SPI_SCK(IN); SPI_MISO(IN); SPCR = 0; }

  void beginTransaction(SPI_Settings settings)
  {
    if (!transaction) { sreg = SREG; cli(); transaction = 1; }
    SPCR = settings.spcr;
    SPSR = settings.spsr;
  }

  void endTransaction(void)
  {
    transaction = 0;
    SREG = sreg;
  }

  // Передача данных

  inline static  void wait() { asm volatile("nop"); while (!(SPSR & _BV(SPIF))); }
  inline static uint8_t transfer(uint8_t data) { SPDR = data; wait(); return SPDR; }
  inline static void send(uint8_t data) { wait(); SPDR = data; }
  inline static void send16(uint16_t data) { wait(); SPDR = to_byte(data, 1); wait(); SPDR = data; }

  uint16_t transfer16(uint16_t data)
  {
    SPDR = to_byte(data, 1);
    wait();
    to_byte(data, 1) = SPDR;
    SPDR = to_byte(data, 0);
    wait();
    to_byte(data, 0) = SPDR;
    return data;
  }

  void transfer(void *buf, size_t count)
  {
    uint8_t *ptr = (uint8_t *)buf;
    while (count--) {
      SPDR = *ptr;
      wait();
      *ptr++ = SPDR;
    }
  }

  // для буферизации
  inline  uint8_t read() { return SPDR; }
  inline  uint16_t read16() { return SPDR; }
  inline  uint32_t read32() { return SPDR; }
  inline  void  write(uint8_t data) { SPDR = data; }
  inline  void  write16(uint16_t data) { SPDR = data >> 8; __asm__ __volatile__("nop"::); wait(); SPDR = data; }
  inline  void  write32(uint32_t data) {}

private:
  uint8_t sreg;
  uint8_t transaction = 0;
};

extern SPI_Master SPI;
