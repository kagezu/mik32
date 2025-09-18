#pragma once
#include "mik32.h"

constexpr int DMA_USART0 = 0;
constexpr int DMA_USART1 = 1;
constexpr int DMA_CRYPT = 2;
constexpr int DMA_SPI0 = 3;
constexpr int DMA_SPI1 = 4;
constexpr int DMA_I2C0 = 5;
constexpr int DMA_I2C = 6;
constexpr int DMA_SPIFI = 7;
constexpr int DMA_TIMER1 = 8;
constexpr int DMA_TIMER2 = 9;
constexpr int DMA_DAC0 = 10;
constexpr int DMA_DAC1 = 11;
constexpr int DMA_TIMER0 = 12;
constexpr int DMA_MEM = 13;

constexpr int DMA_L = 0;
constexpr int DMA_M = 1;
constexpr int DMA_H = 2;
constexpr int DMA_VH = 3;


template<const int N, const int P>
class DMA {
protected:
  uint32_t config;

  enum :int { BYTE, HALF, WORD };
  enum :int { IMM, INC };
  enum :int { NONE, ACK };

public:
  constexpr DMA_CHANNEL_TypeDef *CH() { return &DMA_CONFIG->CHANNELS[N]; }
  constexpr uint32_t DMA_READY() { return DMA_STATUS_READY(N); }

  constexpr static uint32_t read(int perf, int brust = 0, int even = BYTE, int inc = IMM, int ack = NONE)
  {
    return (perf == DMA_MEM
      ? DMA_CH_CFG_READ_MODE_MEMORY_M         // Установить режим памяти
      : (perf << DMA_CH_CFG_READ_REQUEST_S))  // Установить периферийную линию
      | (even << DMA_CH_CFG_READ_SIZE_S)        // Разрядность данных
      | (brust << DMA_CH_CFG_READ_BURST_SIZE_S) // Установить размер пакета
      | (inc << DMA_CH_CFG_READ_INCREMENT_S)    // Установить инкремент
      | (ack << DMA_CH_CFG_READ_ACK_EN_S);      // Установить подтверждение
  }

  constexpr static uint32_t write(int perf, int brust = 0, int even = BYTE, int inc = IMM, int ack = NONE)
  {
    return  (perf == DMA_MEM
      ? DMA_CH_CFG_WRITE_MODE_MEMORY_M         // Установить режим памяти
      : (perf << DMA_CH_CFG_WRITE_REQUEST_S))  // Установить периферийную линию 
      | (even << DMA_CH_CFG_WRITE_SIZE_S)        // Разрядность данных
      | (brust << DMA_CH_CFG_WRITE_BURST_SIZE_S) // Установить размер пакета
      | (inc << DMA_CH_CFG_WRITE_INCREMENT_S)    // Установить инкремент
      | (ack << DMA_CH_CFG_WRITE_ACK_EN_S);      // Установить подтверждение
  }

  // Начать работу
  INLINE void start() { CH()->CFG = config; }
  // Сбросить канал
  INLINE void reset() { CH()->CFG = 0; }

  // Ожидание завершения работы канала
  INLINE void wait() { while (!(DMA_CONFIG->CONFIG_STATUS & DMA_READY())) delay_us(10); }
  INLINE bool is_active() { return !(DMA_CONFIG->CONFIG_STATUS & DMA_READY()); }

  // Установка данных
  template<typename D, typename S>
  INLINE void setup(D dst, S src, uint32_t len)
  {
    CH()->DST = (uint32_t)dst;
    CH()->SRC = (uint32_t)src;
    CH()->LEN = len - 1;
  }

  // mem -> gpio
  // void memout(uint32_t dst, uint32_t src, uint32_t len)
  // {
    // setup(dst, src, len);
    // read(DMA::MEM, WORD, HALF, INC, NONE);
    // write(DMA::MEM, HALF, HALF, IMM, NONE);
    // DMA_CH->CFG = config;
  // }

  // adc -> mem
  void adc(const int line, void *dst, uint32_t len)
  {
    setup(dst, (uint32_t)&ANALOG_REG->ADC_VALUE, (len >> 2) << 2);
    config = DMA_CH_CFG_ENABLE_M
      | (P << DMA_CH_CFG_PRIOR_S)
      | read(line, HALF, HALF, IMM, ACK)
      | write(DMA_MEM, WORD, WORD, INC);
  }
};
