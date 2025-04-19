#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <dma_config.h>

class DMA {
public:
  enum DMA_REQUEST : uint8_t {
    USART0, USART1, CRYPT, SPI0, SPI1, I2C0, I2C1,
    SPIFI, TIMER1, TIMER2, DAC0, DAC1, TIMER0, INST
  };
  enum DMA_SIZE : uint8_t { BYTE, HALF, WORD };
  enum DMA_PRIOR : uint8_t { LOW, MEDIUM, HIGH, VERY };
  enum DMA_FLAG : uint8_t { OFF, ON };

public:
  DMA(uint8_t ch, DMA_PRIOR prior = LOW)
  {
    PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M;   // Включить тактирование модуля
    DMA_CONFIG->CONFIG_STATUS = 0;          // Значения DMA по умолчанию
    DMA_CH = &DMA_CONFIG->CHANNELS[ch];     // Адрес регистров управления каналом
    config = DMA_CH_CFG_ENABLE_M
      | (prior << DMA_CH_CFG_PRIOR_S);   // Установка приоритета
  }

  // Сбросить канал
  void reset() { DMA_CH->CFG = 0; }

  // Повтор
  void encore()
  {
    DMA_CH->CFG = config;
    while (!(DMA_CONFIG->CONFIG_STATUS & DMA_STATUS_READY(0)));
  }

  // Запуск канала
  void start(uint32_t dst, uint32_t src, uint32_t len = 0)
  {
    DMA_CH->DST = dst;
    DMA_CH->SRC = src;
    DMA_CH->LEN = len;
    DMA_CH->CFG = config | DMA_CH_CFG_ENABLE_M;
  }

  // mem -> gpio
  void memout(uint32_t dst, uint32_t src, uint32_t len = 0)
  {
    DMA_CH->DST = dst;
    DMA_CH->SRC = src;
    DMA_CH->LEN = (1 << len) - 1;
    source(DMA::INST, WORD, HALF, ON, OFF);
    dest(DMA::INST, HALF, HALF, OFF, OFF);
    DMA_CH->CFG = config | DMA_CH_CFG_ENABLE_M;
  }

  /*
  + perf - Источник запроса
  + brust - Размер пакета = 2^brust
  + even - Разрядность [DMA::BYTE, DMA::HALF, DMA::WORD]
  + inc - Инкремент [DMA::OFF, DMA::ON]
  + ack - Подтверждение [DMA::OFF, DMA::ON]
   */
  void source(DMA_REQUEST perf, uint8_t brust = 0, DMA_SIZE even = BYTE, DMA_FLAG inc = OFF, DMA_FLAG ack = OFF)
  {
    config |= 0
      // (config & ~(
        // DMA_CH_CFG_READ_MODE_MEMORY_M             // Сбросить режим памяти
        // | DMA_CH_CFG_READ_NO_INCREMENT_M          // Очистить инкремент
        // | DMA_CH_CFG_READ_SIZE_REZ_M              // Очистить разрядность
        // | (0b111 << DMA_CH_CFG_READ_BURST_SIZE_S) // Сбросить размер пакета
        // | DMA_CH_CFG_READ_REQUEST_M               // Очистить периферийную линию
        // | DMA_CH_CFG_READ_ACK_EN_M))              // Очистить подтверждение
      | (perf == INST
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
  + inc - Инкремент [DMA::OFF, DMA::ON]
  + ack - Подтверждение [DMA::OFF, DMA::ON]
   */
  void dest(DMA_REQUEST perf, uint8_t brust = 0, DMA_SIZE even = BYTE, DMA_FLAG inc = OFF, DMA_FLAG ack = OFF)
  {
    config = 0
      // (config & ~(
      //   DMA_CH_CFG_WRITE_MODE_MEMORY_M             // Сбросить режим памяти
      //   | DMA_CH_CFG_WRITE_NO_INCREMENT_M          // Очистить инкремент
      //   | DMA_CH_CFG_WRITE_SIZE_REZ_M              // Очистить разрядность
      //   | (0b111 << DMA_CH_CFG_WRITE_BURST_SIZE_S) // Сбросить размер пакета
      //   | DMA_CH_CFG_WRITE_REQUEST_M               // Очистить периферийную линию
      //   | DMA_CH_CFG_WRITE_ACK_EN_M))              // Очистить подтверждение
      | (perf == INST
        ? DMA_CH_CFG_WRITE_MODE_MEMORY_M         // Установить режим памяти
        : (perf << DMA_CH_CFG_WRITE_REQUEST_S))  // Установить периферийную линию 
      | (even << DMA_CH_CFG_WRITE_SIZE_S)        // Разрядность данных
      | (brust << DMA_CH_CFG_WRITE_BURST_SIZE_S) // Установить размер пакета
      | (inc << DMA_CH_CFG_WRITE_INCREMENT_S)    // Установить инкремент
      | (ack << DMA_CH_CFG_WRITE_ACK_EN_S);      // Установить подтверждение
  }

protected:
  DMA_CHANNEL_TypeDef *DMA_CH;
  uint32_t config;
};
