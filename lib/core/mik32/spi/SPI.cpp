#ifdef MIK32V2
#include "SPI.h"
#include "spi.h"


SPI_Master SPI;

// Частота в кГц
void SPI_Master::init(uint16_t fq, uint8_t mode)
{
  uint8_t spi_div = 0;
  uint16_t max_fq = F_CPU / 4000;

  while (++spi_div < 7)
    if (fq >= max_fq) break;
    else max_fq >>= 1;



}

void SPI_Master::end()
{}

void SPI_Master::send(uint8_t data)
{

}

uint8_t SPI_Master::transfer(uint8_t data)
{
  uint8_t rxByte = 0;

  return rxByte;
}


void SPI_Master::send16(uint16_t data)
{

}

uint16_t SPI_Master::transfer16(uint16_t data)
{
  uint8_t buf[2];
  uint16_t rxVal = 0;

  return rxVal;
}

#endif
