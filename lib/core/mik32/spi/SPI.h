#ifdef MIK32V2
#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <pad_config.h>
#include <gpio.h>
#include "spi.h"
#include "pins.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03


class SPI_Master {
public:
  SPI_Master() {}
  void init(uint16_t fq = 0xffff, uint8_t mode = SPI_MODE0);
  void end();
  uint8_t transfer(uint8_t);
  uint16_t transfer16(uint16_t);

  inline void wait() { while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M); }
  inline void send(uint8_t data)
  {
    SPI_1->TXDATA = data;
    while (!(SPI_1->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M));
  }
  inline void send16(uint16_t data)
  {
    while (!(SPI_1->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M));
    SPI_1->TXDATA = data >> 8;
    SPI_1->TXDATA = data;
  }
};

// extern SPI_Master SPI;

#endif
