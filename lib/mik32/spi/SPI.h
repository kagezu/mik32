#ifdef MIK32V2
#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <gpio.h>
#include "spi.h"
#include "pins.h"

#define SPI_MODE0       0x00
#define SPI_MODE1       0x01
#define SPI_MODE2       0x02
#define SPI_MODE3       0x03

#define SPI_TX_THR      0x07
#define SPI_DELAY       0x00

#define SPI_N           SPI_1

class SPI_Settings {
public:
  void init()
  {
    config =
      SPI_CONFIG_MANUAL_CS_M                          // Ручной режим
      | SPI_CONFIG_CS_NONE_M                          // Устройства не выбраны
      | SPI_CONFIG_MASTER_M;                          // Мастер

    delay_ext_clk = SPI_DELAY;
    tx_thr = SPI_TX_THR;
  }

  void mode(uint8_t m = SPI_MODE0) { config = (config & ~(SPI_CONFIG_CLK_PH_M | SPI_CONFIG_CLK_POL_M)) | (m << SPI_CONFIG_CLK_POL_S); }
  void master() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_MASTER_M; }
  void slave() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_SLAVE_M; }
  void manual_cs(uint32_t *addr, uint32_t mask)
  {
    config |= SPI_CONFIG_MANUAL_CS_M | SPI_CONFIG_CS_NONE_M;
    addr_cs = addr;
    mask_cs = mask;
  }
  void select_cs(uint8_t ss_n)
  {
    config =
      (config & ~(SPI_CONFIG_CS_NONE_M | SPI_CONFIG_MANUAL_CS_M))
      | (SPI_CONFIG_CS_NONE_M ^ (1 << (SPI_CONFIG_CS_S + ss_n)));
  }

  void fq(uint16_t fq = 0xffff)
  {
    uint8_t baud_rate_div = 0;
    uint16_t max_fq = OSC_SYSTEM_VALUE / 2000;

    while (baud_rate_div < 0x07) {
      if (fq >= max_fq) break;
      max_fq >>= 1;
      baud_rate_div++;
    }

    config = (config & ~SPI_CONFIG_BAUD_RATE_DIV_M)
      | (baud_rate_div << SPI_CONFIG_BAUD_RATE_DIV_S); // Делитель частоты
  }
  void delay(uint32_t btwn = 0, uint32_t after = 0, uint32_t ini = 0)
  {
    delay_ext_clk =
      (ini << SPI_DELAY_INIT_S)
      | (after << SPI_DELAY_AFTER_S)
      | (btwn << SPI_DELAY_BTWN_S);
  }

private:
  uint32_t *addr_cs;
  uint32_t mask_cs;
  uint32_t config;
  uint32_t delay_ext_clk;
  uint32_t tx_thr;

  friend class SPI_Class;
};

class SPI_Class {
public:
  void init(uint8_t spi_n);

  void begin(SPI_Settings settings);
  void end(void);

  uint8_t transfer(uint8_t);
  uint16_t transfer16(uint16_t);

  inline void clear_rx() { SPI_N->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M; }
  inline void clear_tx() { SPI_N->ENABLE = SPI_ENABLE_CLEAR_TX_FIFO_M; }
  inline void wait_thr() { while (!(SPI_N->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M)); }
  inline void wait_full() { while (SPI_N->INT_STATUS & SPI_INT_STATUS_TX_FIFO_FULL_M); }
  inline void wait_clr() { while (SPI_N->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M); }
  inline void send(uint8_t data) { wait_full(); SPI_N->TXDATA = data; }
  inline void send16(uint16_t data) { wait_thr(); SPI_N->TXDATA = data >> 8; SPI_N->TXDATA = data; }

  // без проверок
  inline  uint8_t read() { return SPI_N->RXDATA; }
  inline  uint16_t read16() { uint16_t rx = SPI_N->RXDATA << 8; return SPI_N->RXDATA | rx; }
  inline  uint32_t read32();
  inline  void  write(uint8_t data) { SPI_N->TXDATA = data; }
  inline  void  write16(uint16_t data) { SPI_N->TXDATA = data >> 8; SPI_N->TXDATA = data; }
  inline  void  write32(uint32_t data);

private:
  // SPI_TypeDef *SPI_N;
};

extern SPI_Class SPI;

#endif
