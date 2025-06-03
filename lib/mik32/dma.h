#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <dma_config.h>

class DMA {
public:
  enum DMA_REQUEST : uint8_t {
    USART0, USART1, CRYPT, SPI0, SPI1, I2C0, I2C1,
    SPIFI, TIMER1, TIMER2, DAC0, DAC1, TIMER0, MEM
  };
  enum DMA_SIZE : uint8_t { BYTE, HALF, WORD };
  enum DMA_PRIOR : uint8_t { LOW, MEDIUM, HIGH, VERY };
  enum DMA_INCREMENT : uint8_t { IMM, INC };
  enum DMA_ACK : uint8_t { NONE, ACK };

public:
  DMA(uint8_t ch, DMA_PRIOR prior = LOW)
  {
    PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M;   // Включить тактирование модуля
    DMA_CH = &DMA_CONFIG->CHANNELS[ch];     // Адрес регистров управления каналом
    config = DMA_CH_CFG_ENABLE_M
      | (prior << DMA_CH_CFG_PRIOR_S);      // Установка приоритета
    dma_ready = DMA_STATUS_READY(ch);        // Признак, что канал свободен.
  }

  // Сбросить канал
  void reset() { DMA_CH->CFG = 0; }

  // Ожидание завершения работы канала
  void GCC_INLINE wait() { while (!(DMA_CONFIG->CONFIG_STATUS & dma_ready)) for (uint32_t i = 0; i < 100; i++); }
  bool GCC_INLINE is_active() { return !(DMA_CONFIG->CONFIG_STATUS & dma_ready); }
  // Начать работу
  void GCC_INLINE start() { DMA_CH->CFG = config; }

  // Установка данных
  void setup(void *dst, void *src, uint32_t len)
  {
    DMA_CH->DST = (uint32_t)dst;
    DMA_CH->SRC = (uint32_t)src;
    DMA_CH->LEN = len - 1;
  }
  void setup(void *dst, uint32_t src, uint32_t len)
  {
    DMA_CH->DST = (uint32_t)dst;
    DMA_CH->SRC = src;
    DMA_CH->LEN = len - 1;
  }
  void setup(uint32_t dst, void *src, uint32_t len)
  {
    DMA_CH->DST = dst;
    DMA_CH->SRC = (uint32_t)src;
    DMA_CH->LEN = len - 1;
  }
  void setup(uint32_t dst, uint32_t src, uint32_t len)
  {
    DMA_CH->DST = dst;
    DMA_CH->SRC = src;
    DMA_CH->LEN = len - 1;
  }

  /*
  + perf - Источник запроса
  + brust - Размер пакета = 2^brust
  + even - Разрядность [DMA::BYTE, DMA::HALF, DMA::WORD]
  + inc - Инкремент [DMA::IMM, DMA::INC]
  + ack - Подтверждение [DMA::NONE, DMA::ACK]
   */
  void read(uint8_t  perf, uint8_t brust = 0, uint8_t  even = BYTE, uint8_t  inc = IMM, uint8_t  ack = NONE)
  {
    config = (config & ~(
      DMA_CH_CFG_READ_MODE_MEMORY_M             // Сбросить режим памяти
      | DMA_CH_CFG_READ_NO_INCREMENT_M          // Очистить инкремент
      | DMA_CH_CFG_READ_SIZE_REZ_M              // Очистить разрядность
      | (0b111 << DMA_CH_CFG_READ_BURST_SIZE_S) // Сбросить размер пакета
      | DMA_CH_CFG_READ_REQUEST_M               // Очистить периферийную линию
      | DMA_CH_CFG_READ_ACK_EN_M))              // Очистить подтверждение
      | (perf == MEM
        ? DMA_CH_CFG_READ_MODE_MEMORY_M         // Установить режим памяти
        : (perf << DMA_CH_CFG_READ_REQUEST_S))  // Установить периферийную линию
      | (even << DMA_CH_CFG_READ_SIZE_S)        // Разрядность данных
      | (brust << DMA_CH_CFG_READ_BURST_SIZE_S) // Установить размер пакета
      | (inc << DMA_CH_CFG_READ_INCREMENT_S)    // Установить инкремент
      | (ack << DMA_CH_CFG_READ_ACK_EN_S);      // Установить подтверждение
  }

  /*
  + perf - Источник запроса
  + brust - Размер пакета = 2^brust
  + even - Разрядность [DMA::BYTE, DMA::HALF, DMA::WORD]
  + inc - Инкремент [DMA::IMM, DMA::INC]
  + ack - Подтверждение [DMA::NONE, DMA::ACK]
   */
  void write(uint8_t  perf, uint8_t brust = 0, uint8_t  even = BYTE, uint8_t  inc = IMM, uint8_t  ack = NONE)
  {
    config = (config & ~(
      DMA_CH_CFG_WRITE_MODE_MEMORY_M             // Сбросить режим памяти
      | DMA_CH_CFG_WRITE_NO_INCREMENT_M          // Очистить инкремент
      | DMA_CH_CFG_WRITE_SIZE_REZ_M              // Очистить разрядность
      | (0b111 << DMA_CH_CFG_WRITE_BURST_SIZE_S) // Сбросить размер пакета
      | DMA_CH_CFG_WRITE_REQUEST_M               // Очистить периферийную линию
      | DMA_CH_CFG_WRITE_ACK_EN_M))              // Очистить подтверждение
      | (perf == MEM
        ? DMA_CH_CFG_WRITE_MODE_MEMORY_M         // Установить режим памяти
        : (perf << DMA_CH_CFG_WRITE_REQUEST_S))  // Установить периферийную линию 
      | (even << DMA_CH_CFG_WRITE_SIZE_S)        // Разрядность данных
      | (brust << DMA_CH_CFG_WRITE_BURST_SIZE_S) // Установить размер пакета
      | (inc << DMA_CH_CFG_WRITE_INCREMENT_S)    // Установить инкремент
      | (ack << DMA_CH_CFG_WRITE_ACK_EN_S);      // Установить подтверждение
  }


  // mem -> gpio
  void memout(uint32_t dst, uint32_t src, uint32_t len)
  {
    DMA_CH->DST = dst;
    DMA_CH->SRC = src;
    DMA_CH->LEN = len - 1;
    read(DMA::MEM, WORD, HALF, INC, NONE);
    write(DMA::MEM, HALF, HALF, IMM, NONE);
    DMA_CH->CFG = config;
  }

  // adc -> mem
  void adc(DMA_REQUEST line, void *dst, uint32_t len)
  {
    setup(dst, (uint32_t)&ANALOG_REG->ADC_VALUE, (len >> 2) << 2);
    read(line, HALF, HALF, IMM, ACK);
    write(MEM, WORD, WORD, INC);
  }

protected:
  DMA_CHANNEL_TypeDef *DMA_CH;
  uint32_t config;
  uint32_t dma_ready;
};
