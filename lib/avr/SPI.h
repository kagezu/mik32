#pragma once
#include "macros/common.h"
#include "macros/attribute.h"
#include "macros/gpio.h"

#ifdef __AVR_ATmega328P__
#define SPI_SS(x)     x (B, PB2)
#define SPI_MOSI(x)   x (B, PB3)
#define SPI_MISO(x)   x (B, PB4)
#define SPI_SCK(x)    x (B, PB5)
#endif

#ifdef __AVR_ATmega128__
#define SPI_SS(x)     x (B, PB0)
#define SPI_SCK(x)    x (B, PB1)
#define SPI_MOSI(x)   x (B, PB2)
#define SPI_MISO(x)   x (B, PB3)
#endif


#define SPI_MASTER    _BV(MSTR)
#define SPI_SLAVE     0x00


#define SPI_MODE0     0x00
#define SPI_MODE1     _BV(CPHA)
#define SPI_MODE2     _BV(CPOL)
#define SPI_MODE3     (_BV(CPHA) | _BV(CPOL))

#define SPI_DIV_4     0x00
#define SPI_DIV_16    0x01
#define SPI_DIV_64    0x02
#define SPI_DIV_128   0x03
#define SPI_DIV_M     0x03

#define SPI_MSB       0
#define SPI_LSB       _BV(DORD)

#define SPI_INTERRUPT _BV(SPIE)

class SPI_Settings {
public:
  void init()
  {
    spcr = _BV(SPE) | SPI_MSB | SPI_MODE0 | SPI_MASTER;
    fq();
  }

  void fq(uint16_t fq = 0xffff)
  {
    uint8_t sck = 0, spi2x = 0;

    if (fq >= F_CPU / 2000) { sck = SPI_DIV_4; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 4000) sck = SPI_DIV_4;
    else if (fq >= F_CPU / 8000) { sck = SPI_DIV_16; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 16000) sck = SPI_DIV_16;
    else if (fq >= F_CPU / 32000) { sck = SPI_DIV_64; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 64000) sck = SPI_DIV_64;
    else sck = SPI_DIV_128;

    spcr = (spcr & ~SPI_DIV_M) | sck;
    spsr = spi2x;
  }

  // заглушки
  void thr(uint8_t x) {}

private:
  uint8_t spcr;
  uint8_t spsr;

  friend class CSPI;
};

class CSPI {
public:
  void init()
  {
    SPI_MOSI(OUT);
    SPI_MISO(OUT);
    SPI_SCK(OUT);
    SPI_SS(IN);
    SPI_SS(SET);
  }

  void begin(SPI_Settings settings)
  {
    if (!transaction) { sreg = SREG; cli(); transaction = 1; }
    SPCR = settings.spcr;
    SPSR = settings.spsr;
  }

  void end(void)
  {
    wait();
    transaction = 0;
    SREG = sreg;
  }

  // Передача данных

  GCC_INLINE void wait() { asm volatile("nop"); while (!(SPSR & _BV(SPIF))); }
  GCC_INLINE void wait_clr() { wait(); }
  GCC_INLINE void wait_thr() { wait(); }

  GCC_INLINE void send(uint8_t data) { SPDR = data; }
  GCC_INLINE void send16(uint16_t data) { SPDR = to_byte(data, 1); wait(); SPDR = data; }
  GCC_INLINE uint8_t transfer(uint8_t data) { SPDR = data; wait(); return SPDR; }

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

private:
  uint8_t sreg;
  uint8_t transaction = 0;
};
