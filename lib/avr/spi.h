#pragma once
#include "pinout.h"

#define SPI_MASTER    _BV(MSTR)
#define SPI_SLAVE     0x00


#define SPI_MODE0     0x00
#define SPI_MODE1     _BV(CPHA)
#define SPI_MODE2     _BV(CPOL)
#define SPI_MODE3     (_BV(CPHA) | _BV(CPOL))

#define SPI_DIV_4     0x00
#define SPI_DIV_16    0x01
#define SPI_DIV_64    0x02
#define SPI_DIV_128   0x03
#define SPI_DIV_M     0x03

#define SPI_MSB       0
#define SPI_LSB       _BV(DORD)

#define SPI_INTERRUPT _BV(SPIE)

class SPI {
protected:
  uint8_t spcr;
  uint8_t spsr;

public:
  SPI()
  {
    SPI1_MISO.init(GP_Float);
    SPI1_MOSI.init(GPO_Max);
    SPI1_SCK.init(GPO_Max);
    SPI1_NSS_IN.init(GP_VCC);

    fq(0x4000);

  }

  void fq(uint16_t fq)
  {
    uint8_t sck = 0, spi2x = 0;

    if (fq >= F_CPU / 2000) { sck = SPI_DIV_4; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 4000) sck = SPI_DIV_4;
    else if (fq >= F_CPU / 8000) { sck = SPI_DIV_16; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 16000) sck = SPI_DIV_16;
    else if (fq >= F_CPU / 32000) { sck = SPI_DIV_64; spi2x = _BV(SPI2X); }
    else if (fq >= F_CPU / 64000) sck = SPI_DIV_64;
    else sck = SPI_DIV_128;

    spcr = (spcr & ~SPI_DIV_M) | sck;
    spsr = spi2x;
  }

  void mode(uint8_t m) { spcr = (spcr & ~SPI_MODE3) | m; }
  void master() { spcr |= SPI_MASTER; }
  void slave() { spcr &= ~SPI_MASTER; }

  // заглушки
  void thr(uint8_t) {}
  void select_cs(uint8_t) {}
  void delay(uint32_t) {}


  void begin()
  {
    if (!transaction) { sreg = SREG; cli(); transaction = 1; }
    SPCR = spcr;
    SPSR = spsr;
  }

  void end(void)
  {
    wait();
    transaction = 0;
    SREG = sreg;
  }

  // Передача данных

  INLINE void wait() { asm volatile("nop"); while (!(SPSR & _BV(SPIF))); }
  INLINE void wait_idle() { wait(); }
  INLINE void wait_thr() { wait(); }

  INLINE void send(uint8_t data) { SPDR = data; }
  INLINE void send16(uint16_t data) { SPDR = to_byte(data, 1); wait(); SPDR = data; }
  INLINE uint8_t transfer(uint8_t data) { SPDR = data; wait(); return SPDR; }

  uint16_t transfer16(uint16_t data)
  {
    SPDR = to_byte(data, 1);
    wait();
    to_byte(data, 1) = SPDR;
    SPDR = to_byte(data, 0);
    wait();
    to_byte(data, 0) = SPDR;
    return data;
  }

  void transfer(void *buf, size_t count)
  {
    uint8_t *ptr = (uint8_t *)buf;
    while (count--) {
      SPDR = *ptr;
      wait();
      *ptr++ = SPDR;
    }
  }

private:
  uint8_t sreg;
  uint8_t transaction = 0;
};

class SPI0 : public SPI {};
#define SPI1  SPI




/*

template<const int N>
class SPI {
protected:
  uint32_t config;
  uint32_t delay_clk;
  uint32_t tx_thr;

public:
  SPI()
  {
    // Настройка порта ввода/вывода
    // if (N == SPI_0_BASE_ADDRESS) {
    if (N) {
      SPI1_MISO(SERIAL); SPI1_MOSI(SERIAL); SPI1_SCK(SERIAL); SPI1_NSS_IN(SERIAL);
      SPI1_MISO(P_NC); SPI1_MOSI(P_NC); SPI1_SCK(P_NC); SPI1_NSS_IN(P_VCC);
    }
    else {
      SPI0_MISO(SERIAL); SPI0_MOSI(SERIAL); SPI0_SCK(SERIAL); SPI0_NSS_IN(SERIAL);
      SPI0_MISO(P_NC); SPI0_MOSI(P_NC); SPI0_SCK(P_NC); SPI0_NSS_IN(P_VCC);
    }

    config = 0
      | SPI_CONFIG_MANUAL_CS_M    // Ручной режим
      | SPI_CONFIG_CS_NONE_M      // Устройства не выбраны
      | SPI_CONFIG_MASTER_M;      // Мастер

    fq(0x4000); // 16 MHz

    delay_clk = SPI_DELAY_DEF;
    tx_thr = SPI_TX_THR;

    SPIx->ENABLE = 0;                     // Отключение модуля
    SPIx->INT_DISABLE = SPI_INT_DISABLE;  // Сброс маски прерываний
    SPIx->DELAY = delay_clk;              // Регистр задержек
    SPIx->TX_THR = tx_thr;                // Установка порога по умолчанию
  }

  // Частота в килогерцах
  void fq(uint16_t f)
  {
    uint8_t baud_rate_div = 0;
    uint16_t max_fq = OSC_SYSTEM_VALUE / 2000;
    while (f < max_fq && baud_rate_div++ < 0x07)
      max_fq >>= 1;

    config = (config & ~SPI_CONFIG_BAUD_RATE_DIV_M)
      | (baud_rate_div << SPI_CONFIG_BAUD_RATE_DIV_S); // Делитель частоты
  }
  void thr(uint8_t t) { tx_thr = 9 - t; }
  void mode(uint8_t m = SPI_MODE0) { config = (config & ~(SPI_CONFIG_CLK_PH_M | SPI_CONFIG_CLK_POL_M)) | (m << SPI_CONFIG_CLK_POL_S); }
  void master() { config = (config & ~SPI_CONFIG_MODE_SEL_M) | SPI_CONFIG_MASTER_M; }
  void slave() { config &= ~SPI_CONFIG_MODE_SEL_M; }
  void select_cs(uint8_t ss_n)
  {
    config =
      (config & ~(SPI_CONFIG_CS_NONE_M | SPI_CONFIG_MANUAL_CS_M))
      | (SPI_CONFIG_CS_NONE_M ^ (1 << (SPI_CONFIG_CS_S + ss_n)));
  }
  void delay(uint32_t btwn = 0, uint32_t after = 0, uint32_t ini = 0)
  {
    delay_clk =
      (ini << SPI_DELAY_INIT_S)
      | (after << SPI_DELAY_AFTER_S)
      | (btwn << SPI_DELAY_BTWN_S);
  }

  static void wait() {}
  // Очистить FIFO
  INLINE static void clear_fifo() { SPIx->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M | SPI_ENABLE_CLEAR_TX_FIFO_M; }
  // Очистить чтением RX_FIFO
  INLINE static void clear_rx() { while ((SPIx->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M)) SPIx->RXDATA; }
  // Ждать TX_FIFO < TX_THR
  INLINE static void wait_thr() { while (!(SPIx->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M)); }
  // Ждать TX_FIFO < 8
  INLINE static void wait_full() { while (SPIx->INT_STATUS & SPI_INT_STATUS_TX_FIFO_FULL_M); }
  // Ждать TX_FIFO = 0
  INLINE static void wait_idle() { while (SPIx->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M); }
  INLINE static void send(uint8_t data) { SPIx->TXDATA = data; }
  INLINE static void send16(uint16_t data) { SPIx->TXDATA = data >> 8; SPIx->TXDATA = data; }

  INLINE void begin()
  {
    clear_fifo();
    SPIx->CONFIG = config;
    // SPIx->TX_THR = tx_thr;
    // SPIx->DELAY = delay_clk;
    SPIx->ENABLE = SPI_ENABLE_M;           // Включение модуля
  }

  INLINE static void end() { wait_idle(); SPIx->ENABLE = 0; }

  static uint8_t transfer(uint8_t data)
  {
    SPIx->TXDATA = data;
    wait_idle();
    return  SPIx->RXDATA;
  }

  static uint16_t transfer16(uint16_t data)
  {
    uint16_t rx_dbyte;
    SPIx->TXDATA = data >> 8;
    SPIx->TXDATA = data;
    wait_idle();
    rx_dbyte = SPIx->RXDATA << 8;
    rx_dbyte |= SPIx->RXDATA;
    return rx_dbyte;
  }
};

*/
