#include "SPI.h"
// #include "core/type/buffer.h"

#define SPI_BUFFER_SIZE   32

SPIClass SPI;

static uint8_t buffer[16];
static uint8_t *head = buffer;
static uint8_t *tail = buffer;
static volatile uint8_t size = 0;

void SPIClass::write(uint8_t data)
{
  // cli();
  size++;
  *head++ = data;
  SPDR = size;
  // sei();
  while (size == SPI_BUFFER_SIZE);
}


ISR(SPI_STC_vect)
{
  if (size) { SPDR = *tail++; size--; }
  else tail = head = buffer;
}
