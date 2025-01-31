#ifdef MIK32V2
#pragma once
#include <core.h>

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03


class SPI_Master {
public:
  SPI_Master() {}
  void wait();
  void init(uint16_t fq = 4000, uint8_t mode = SPI_MODE0);
  void end();
  void send(uint8_t);
  void send12(uint16_t);
  void send16(uint16_t);
  uint8_t transfer(uint8_t);
  uint16_t transfer16(uint16_t);
};

// extern SPI_Master SPI;

#endif
