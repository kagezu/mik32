#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <dma_config.h>

class DMA {
public:
  enum DMA_REQUEST : uint8_t {
    USART0,
    USART1,
    CRYPT,
    SPI0,
    SPI1,
    I2C0,
    I2C1,
    SPIFI,
    TIMER1,
    TIMER2,
    DAC0,
    DAC1,
    TIMER0
  };

  enum DMA_SIZE : uint8_t {
    BYTE, HALF, WORD
  };

  enum DMA_PRIOR : uint8_t {
    LOW, MEDIUM, HIGH, VERY
  };

  enum DMA_INCREMENT : uint8_t {
    INC_OFF, INC_ON
  };

public:
  DMA(uint8_t ch, DMA_PRIOR prior = LOW)
  {
    PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M;   // Включить тактирование модуля
    DMA_CONFIG->CONFIG_STATUS = 0;          // Значения DMA по умолчанию
    DMA_CH = &DMA_CONFIG->CHANNELS[ch];     // Адрес регистров управления каналом
    config = prior << DMA_CH_CFG_PRIOR_S;   // Установка приоритета
  }

  // Запуск канала
  void start() { DMA_CH->CFG = config | DMA_CH_CFG_ENABLE_M; }

  void read(DMA_REQUEST perf, uint8_t brust = 0)
  {
    static uint32_t src[] = {};
    DMA_CH->SRC = src[perf];
    config = (config & ~(
      DMA_CH_CFG_READ_REQUEST_M                     // Очистить периферийную линию
      | DMA_CH_CFG_READ_SIZE_REZ_M                  // Очистить разрядность
      | DMA_CH_CFG_READ_NO_INCREMENT_M              // Очистить инкремент
      | (0b111 << DMA_CH_CFG_READ_BURST_SIZE_S)))   // Сбросить размер пакета
      | (perf << DMA_CH_CFG_READ_REQUEST_S)         // Установить периферийную линию
      | (size(perf) << DMA_CH_CFG_READ_SIZE_S)      // Разрядность данных
      | (brust << DMA_CH_CFG_READ_BURST_SIZE_S);    // Установить размер пакета
  }

  void write(DMA_REQUEST perf, uint8_t brust = 0)
  {
    static uint32_t dst[] = {};
    DMA_CH->DST = dst[perf];
    config = (config & ~(
      DMA_CH_CFG_WRITE_REQUEST_M                    // Очистить периферийную линию
      | DMA_CH_CFG_WRITE_SIZE_REZ_M                 // Очистить разрядность
      | DMA_CH_CFG_WRITE_NO_INCREMENT_M             // Очистить инкремент
      | (0b111 << DMA_CH_CFG_WRITE_BURST_SIZE_S)))  // Сбросить размер пакета
      | (perf << DMA_CH_CFG_WRITE_REQUEST_S)        // Установить периферийную линию
      | (size(perf) << DMA_CH_CFG_WRITE_SIZE_S)     // Разрядность данных
      | (brust << DMA_CH_CFG_WRITE_BURST_SIZE_S);   // Установить размер пакета
  }

protected:
  DMA_CHANNEL_TypeDef *DMA_CH;
  uint32_t config;

  static uint8_t size(DMA_REQUEST perf)
  {
    static uint8_t size_a[] = {};
    return size_a[perf];
  }
};
