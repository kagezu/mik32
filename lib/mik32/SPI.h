#pragma once
#include "mik32.h"

#define SPI_MISO_0(x)   x (0, 0)
#define SPI_MOSI_0(x)   x (0, 1)
#define SPI_SCK_0(x)    x (0, 2)
#define SPI_SS_0(x)     x (0, 3)

#define SPI_MISO_1(x)   x (1, 0)
#define SPI_MOSI_1(x)   x (1, 1)
#define SPI_SCK_1(x)    x (1, 2)
#define SPI_SS_1(x)     x (1, 3)

#define SPI_MODE0       0x00
#define SPI_MODE1       0x01
#define SPI_MODE2       0x02
#define SPI_MODE3       0x03

#define SPI_TX_THR      0x07
#define SPI_DELAY_DIV   0x00    // Минимальная задержка 3 такта (2 + [D_BTWN])

class SPI_Settings {
public:
  void init()
  {
    config =
      SPI_CONFIG_MANUAL_CS_M                          // Ручной режим
      | SPI_CONFIG_CS_NONE_M                          // Устройства не выбраны
      | SPI_CONFIG_MASTER_M;                          // Мастер

    delay_clk = SPI_DELAY_DIV;
    tx_thr = SPI_TX_THR;
  }
  void thr(uint8_t t) { tx_thr = t; }
  void mode(uint8_t m = SPI_MODE0) { config = (config & ~(SPI_CONFIG_CLK_PH_M | SPI_CONFIG_CLK_POL_M)) | (m << SPI_CONFIG_CLK_POL_S); }
  void master() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_MASTER_M; }
  void slave() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_SLAVE_M; }
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
    delay_clk =
      (ini << SPI_DELAY_INIT_S)
      | (after << SPI_DELAY_AFTER_S)
      | (btwn << SPI_DELAY_BTWN_S);
  }

  // private:
  uint32_t config;
  uint32_t delay_clk;
  uint32_t tx_thr;

  // friend class SPI<>;
};

// typedef SPI_TypeDef *SPI_Type;

// template<SPI_TypeDef *SPI_N = SPI_1>
class SPI {
public:
  void init(uint8_t spi_n = 1);
  void begin(SPI_Settings settings);
  void end();
  //Заглушка
  void wait() {}

  uint8_t transfer(uint8_t);
  uint16_t transfer16(uint16_t);

  // Очистить чтением RX_FIFO
  void clear_rx();
  // Ждать TX_FIFO < TX_THR
  GCC_INLINE void wait_thr() { while (!(SPI_N->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M)); }
  // Ждать TX_FIFO < 8
  GCC_INLINE void wait_full() { while (SPI_N->INT_STATUS & SPI_INT_STATUS_TX_FIFO_FULL_M); }
  // Ждать TX_FIFO = 0
  GCC_INLINE void wait_clr() { while (SPI_N->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M); }

  GCC_INLINE void send(uint8_t data) { SPI_N->TXDATA = data; }
  GCC_INLINE void send16(uint16_t data) { SPI_N->TXDATA = data >> 8; SPI_N->TXDATA = data; }

  // без проверок

  // GCC_INLINE void  write(uint8_t data) { SPI_N->TXDATA = data; }
  // GCC_INLINE void  write16(uint16_t data) { SPI_N->TXDATA = data >> 8; SPI_N->TXDATA = data; }
  // GCC_INLINE uint8_t read() { return SPI_N->RXDATA; }
  // GCC_INLINE uint16_t read16() { uint16_t rx = SPI_N->RXDATA << 8; return SPI_N->RXDATA | rx; }

private:
  SPI_TypeDef *SPI_N;
};
