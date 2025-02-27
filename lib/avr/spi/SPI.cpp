#include "SPI.h"
// #include "core/type/buffer.h"

#define SPI_BUFFER_SIZE   32
#define SPI_GET_SIZE      0xff
#define SPI_GET_DATA      0x00

SPI_Class SPI;

static uint8_t buffer[16];
static uint8_t *head = buffer;
static uint8_t *tail = buffer;
static volatile uint8_t size = 0;

void SPI_Class::write(uint8_t data)
{
  if (size < SPI_BUFFER_SIZE) {
    *head++ = data;
    size++;
    SPDR = size;
  }
}

void SPI_Class::write16(uint16_t data)
{
  if (size < SPI_BUFFER_SIZE - 1) {
    *head++ = data >> 8;
    *head++ = data;
    size += 2;
    SPDR = size;
  }
}

// ISR(SPI_STC_vect)
// {
//   switch (SPDR) {
//     case SPI_GET_SIZE:
//       SPDR = size;
//       break;

//     case SPI_GET_DATA:
//       if (size) {
//         SPDR = *tail++;
//         size--;
//       }
//       else {
//         tail = head = buffer;
//         SPDR = 0xff;
//       }
//       break;
//   }

uint16_t SPI_Class::read16(uint8_t command)
{
  switch (command) {
    case SPI_GET_SIZE:
      return size;

    case SPI_GET_DATA:
      if (size) {
        size -= 2;
        uint16_t key = *tail++;
        return *tail++ | (key << 8);
      }
  }
  return 0;
}
