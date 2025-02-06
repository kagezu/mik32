#include "SPI.h"
#include "macros/avr.h"

#define SPI_DIV_4   0x00
#define SPI_DIV_16  0x01
#define SPI_DIV_64  0x02
#define SPI_DIV_128 0x03

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

  SPI_SS(OUT); SPI_MOSI(OUT); SPI_SCK(OUT); SPI_MISO(IN);

  SPCR = _BV(SPE) | _BV(MSTR) | mode | sck;
  SPSR = spi2x;
  SPDR = 0;   // Способ установить флаг SPIF
  wait();
}

void SPI_Master::send16(uint16_t data)
{
  wait();
  SPDR = to_byte(data, 1);
  __asm__ __volatile__("nop"::);
  wait();
  SPDR = to_byte(data, 0);
}

uint8_t SPI_Master::transfer(uint8_t data)
{
  wait();
  SPDR = data;
  __asm__ __volatile__("nop"::);
  wait();
  return SPDR;
}

uint16_t SPI_Master::transfer16(uint16_t data)
{
  wait();
  SPDR = to_byte(data, 1);
  __asm__ __volatile__("nop"::);
  wait();
  to_byte(data, 1) = SPDR;
  SPDR = to_byte(data, 0);
  __asm__ __volatile__("nop"::);
  wait();
  to_byte(data, 0) = SPDR;
  return data;
}
