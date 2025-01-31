#ifdef __AVR_ATmega328P__
#include "spi.h"

#define SPI_DIV_4   0x00
#define SPI_DIV_16  0x01
#define SPI_DIV_64  0x02
#define SPI_DIV_128 0x03

SPI_Master SPI;

// Частота в кГц
void SPI_Master::init(uint16_t fq, uint8_t mode)
{
  uint8_t sck = 0, spi2x = 0;

  if (fq >= F_CPU / 2000) { sck = SPI_DIV_4; spi2x = _BV(SPI2X); }
  else if (fq >= F_CPU / 4000) sck = SPI_DIV_4;
  else if (fq >= F_CPU / 8000) { sck = SPI_DIV_16; spi2x = _BV(SPI2X); }
  else if (fq >= F_CPU / 16000) sck = SPI_DIV_16;
  else if (fq >= F_CPU / 32000) { sck = SPI_DIV_64; spi2x = _BV(SPI2X); }
  else if (fq >= F_CPU / 64000) sck = SPI_DIV_64;
  else sck = SPI_DIV_128;

  SPI_MASTER_PIN;
  SPCR = _BV(SPE) | _BV(MSTR) | mode | sck;
  SPSR = spi2x;
  SPDR = 0;   // Способ установить флаг SPIF
  wait();
}

void SPI_Master::send(uint8_t data)
{
  wait();
  SPDR = data;
}

void SPI_Master::send12(uint16_t data12)
{
  static uint8_t half, flag = 0;
  uint8_t data;

  if (flag) {
    data = half | (data12 >> 8);
    wait();
    SPDR = data;
    flag = 0;
    wait();
    SPDR = (uint8_t)data12;
  }
  else {
    data = data12 >> 4;
    wait();
    SPDR = data;
    half = data12 << 4;
    flag = 1;
  }
}

void SPI_Master::send16(uint16_t data)
{
  dbyte buf;
  buf.word = data;
  wait();
  SPDR = buf.byte[1];
  wait();
  SPDR = buf.byte[0];
}

uint8_t SPI_Master::transfer(uint8_t data)
{
  wait();
  SPDR = data;
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

uint16_t SPI_Master::transfer16(uint16_t data)
{
  dbyte buf;
  buf.word = data;
  wait();
  SPDR = buf.byte[1];
  wait();
  buf.byte[1] = SPDR;
  SPDR = buf.byte[0];
  while (!(SPSR & _BV(SPIF)));
  buf.byte[0] = SPDR;
  return buf.word;
}

#endif
