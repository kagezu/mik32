#pragma once
#include "macros/avr.h"
#include "pins.h"

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

#define SPI_MSB       0
#define SPI_LSB       _BV(DORD)

#define SPI_INTERRUPT _BV(SPIE)

class SPI_Settings {
public:
  SPI_Settings() {}
  SPI_Settings(uint16_t fq, uint8_t mode = SPI_MSB | SPI_MODE0 | SPI_MASTER) { init(fq, mode); }

  void init(uint16_t fq = 0xffff, uint8_t mode = SPI_MSB | SPI_MODE0 | SPI_MASTER)
  {
    uint8_t sck = 0, spi2x = 0;

    if (fq >= F_CPU / 2000) { sck = SPI_DIV_4; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 4000) sck = SPI_DIV_4;
    else if (fq >= F_CPU / 8000) { sck = SPI_DIV_16; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 16000) sck = SPI_DIV_16;
    else if (fq >= F_CPU / 32000) { sck = SPI_DIV_64; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 64000) sck = SPI_DIV_64;
    else sck = SPI_DIV_128;

    spcr = _BV(SPE) | mode | sck;
    spsr = spi2x;
  }

private:
  uint8_t spcr;
  uint8_t spsr;

  friend class SPI_Class;
};

class SPI_Class {
public:
  // Инициализация

  SPI_Class() {}
  void begin()
  {
    SPI_MOSI(OUT);
    SPI_MISO(OUT);
    SPI_SCK(OUT);
    SPI_SS(IN);
    SPI_SS(SET);
  }
  void end() { SPI_MOSI(IN); SPI_SCK(IN); SPI_MISO(IN); SPI_SS(IN); SPCR = 0; }

  void beginTransaction(SPI_Settings settings)
  {
    if (settings.spcr & SPI_MASTER) SPI_SS(OUT);
    if (transaction == 0) { sreg = SREG; cli(); transaction = 1; }
    SPCR = settings.spcr;
    SPSR = settings.spsr;
  }

  void endTransaction(void)
  {
    transaction = 0;
    SREG = sreg;
    SPI_SS(IN);
  }

  // Передача данных

  inline static  void wait() { asm volatile("nop"); while (!(SPSR & _BV(SPIF))); }
  inline static uint8_t transfer(uint8_t data) { SPDR = data; wait(); return SPDR; }
  inline static void send(uint8_t data) { SPDR = data; }
  inline static void send16(uint16_t data) { SPDR = to_byte(data, 1); wait(); SPDR = data; }

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
  void write(uint8_t data);
  void write16(uint16_t data);
  uint16_t read16(uint8_t command);

private:
  uint8_t sreg;
  uint8_t transaction = 0;
};

extern SPI_Class SPI;
