#pragma once
#include "mik32.h"

#define SPI_MODE0         0x00
#define SPI_MODE1         0x01
#define SPI_MODE2         0x02
#define SPI_MODE3         0x03

#define SPI_TX_THR        0x07
#define SPI_DELAY_DEF     0x00
#define SPI_INT_DISABLE   0x3F

class SPIConf {
public:
  uint32_t config;
  uint32_t delay_clk;
  uint32_t tx_thr;

  SPIConf()
  {
    config = 0
      | SPI_CONFIG_MANUAL_CS_M    // Ручной режим
      | SPI_CONFIG_CS_NONE_M      // Устройства не выбраны
      | SPI_CONFIG_MASTER_M;      // Мастер

    delay_clk = SPI_DELAY_DEF;
    tx_thr = SPI_TX_THR;
    fq(0x4000);
  }

  void thr(uint8_t t) { tx_thr = t; }
  void mode(uint8_t m = SPI_MODE0) { config = (config & ~(SPI_CONFIG_CLK_PH_M | SPI_CONFIG_CLK_POL_M)) | (m << SPI_CONFIG_CLK_POL_S); }
  void master() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_MASTER_M; }
  void slave() { config &= ~SPI_CONFIG_MODE_SEL_M; }
  void select_cs(uint8_t ss_n)
  {
    config =
      (config & ~(SPI_CONFIG_CS_NONE_M | SPI_CONFIG_MANUAL_CS_M))
      | (SPI_CONFIG_CS_NONE_M ^ (1 << (SPI_CONFIG_CS_S + ss_n)));
  }

  void fq(uint16_t f)
  {
    uint8_t baud_rate_div = 0;
    uint16_t max_fq = OSC_SYSTEM_VALUE / 2000;
    while (f < max_fq && baud_rate_div++ < 0x07)
      max_fq >>= 1;

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
};

#define SPI_N   ((SPI_TypeDef *)N)

template<auto N>
class SPI {
public:
  SPI()
  {
    // Настройка порта ввода/вывода
    if (N == SPI_0_BASE_ADDRESS) {
      SPI0_MISO(SERIAL); SPI0_MOSI(SERIAL); SPI0_SCK(SERIAL); SPI0_NSS_IN(SERIAL);
      SPI0_MISO(P_NC); SPI0_MOSI(P_NC); SPI0_SCK(P_NC); SPI0_NSS_IN(P_VCC);
    }
    else {
      SPI1_MISO(SERIAL); SPI1_MOSI(SERIAL); SPI1_SCK(SERIAL); SPI1_NSS_IN(SERIAL);
      SPI1_MISO(P_NC); SPI1_MOSI(P_NC); SPI1_SCK(P_NC); SPI1_NSS_IN(P_VCC);
    }

    SPI_N->ENABLE = 0;                      // Отключение модуля
    SPI_N->INT_DISABLE = SPI_INT_DISABLE;   // Сброс маски прерываний
    SPI_N->DELAY = SPI_DELAY_DEF;           // Регистр задержек
    SPI_N->TX_THR = SPI_TX_THR;             // Установка порога по умолчанию
  }

  void wait() {}
  // Очистить FIFO
  ATTR_INLINE  void clear_fifo() { SPI_N->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M | SPI_ENABLE_CLEAR_TX_FIFO_M; }
  // Очистить чтением RX_FIFO
  ATTR_INLINE  void clear_rx() { while ((SPI_N->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M)) SPI_N->RXDATA; }
  // Ждать TX_FIFO < TX_THR
  ATTR_INLINE  void wait_thr() { while (!(SPI_N->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M)); }
  // Ждать TX_FIFO < 8
  ATTR_INLINE  void wait_full() { while (SPI_N->INT_STATUS & SPI_INT_STATUS_TX_FIFO_FULL_M); }
  // Ждать TX_FIFO = 0
  ATTR_INLINE  void wait_idle() { while (SPI_N->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M); }
  ATTR_INLINE  void send(uint8_t data) { SPI_N->TXDATA = data; }
  ATTR_INLINE  void send16(uint16_t data) { SPI_N->TXDATA = data >> 8; SPI_N->TXDATA = data; }

  void begin(SPIConf settings)
  {
    clear_fifo();
    SPI_N->CONFIG = settings.config;
    // SPI_N->TX_THR = settings.tx_thr;
    SPI_N->ENABLE = SPI_ENABLE_M;           // Включение модуля
  }

  ATTR_INLINE void end() { wait_idle(); SPI_N->ENABLE = 0; }

  uint8_t transfer(uint8_t data)
  {
    SPI_N->TXDATA = data;
    wait_idle();
    return  SPI_N->RXDATA;
  }

  uint16_t transfer16(uint16_t data)
  {
    uint16_t rx_dbyte;
    SPI_N->TXDATA = data >> 8;
    SPI_N->TXDATA = data;
    wait_idle();
    rx_dbyte = SPI_N->RXDATA << 8;
    rx_dbyte |= SPI_N->RXDATA;
    return rx_dbyte;
  }
};

#define SPI0 SPI<SPI_0_BASE_ADDRESS>
#define SPI1 SPI<SPI_1_BASE_ADDRESS>
