#ifdef MIK32V2
#include "SPI.h"
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <pad_config.h>
#include <gpio.h>
#include "spi.h"


// SPI_Master SPI;

// Частота в кГц
void SPI_Master::init(uint16_t fq, uint8_t mode)
{
  uint8_t baud_rate_div = 0;
  uint16_t max_fq = F_CPU / 2000;

  while (baud_rate_div < 0x07) {
    if (fq >= max_fq) break;
    max_fq >>= 1;
    baud_rate_div++;
  }

  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_SPI_1_M;      // Тактирование модуля

  SPI_1->ENABLE = 0;                                // Отключение модуля
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M;       // Очищение FIFO
  SPI_1->INT_DISABLE = 0x3F;                        // Сброс маски прерываний
  // SPI_1->INT_ENABLE = 0x00;                      // Установка прерываний
  // Разрешить прерывания от используемого SPI контроллера (IRQ[3] для SPI_0 или IRQ[4] для SPI[1]) в программируемом контроллере прерываний;

  SPI_1->CONFIG =
    SPI_CONFIG_MANUAL_CS_S                          // Ручной режим
    | SPI_CONFIG_CS_NONE_M                          // Устройства не выбраны
    | (baud_rate_div << SPI_CONFIG_BAUD_RATE_DIV_S) // Делитель частоты
    | (mode << SPI_CONFIG_CLK_POL_S)                // Фаза и полярность
    | SPI_CONFIG_MASTER_M;                          // Мастер

  // SPI_1->ENABLE = SPI_ENABLE_M;                     // Включение модуля

  SPI_1->TX_THR = 2;  // Задает уровень, при котором TX_FIFO считается не заполненным 1-8

}

void SPI_Master::end()
{
  SPI_1->ENABLE = 0;                          // Отключение модуля
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M; // Очищение FIFO
  SPI_1->INT_STATUS &= 0x3F;                  // Сброс флагов событий
  SPI_1->INT_DISABLE = 0x3F;                  // Сброс маски прерываний
  SPI_1->INT_STATUS = 0x3F;                   // Сброс флагов событий
}

void SPI_Master::send(uint8_t data)
{
  SPI_1->TXDATA = data;
  while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M);
  uint8_t r = SPI_1->RXDATA;
}

uint8_t SPI_Master::transfer(uint8_t data)
{
  SPI_1->TXDATA = data;
  while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M);
  return  SPI_1->RXDATA;
}


void SPI_Master::send16(uint16_t data)
{
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data;
  while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M);
  uint8_t r = SPI_1->RXDATA;
  r = SPI_1->RXDATA;
}

uint16_t SPI_Master::transfer16(uint16_t data)
{
  uint16_t rx_dbyte;
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data;
  while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M);
  rx_dbyte = SPI_1->RXDATA << 8;
  rx_dbyte |= SPI_1->RXDATA;

  return rx_dbyte;
}

void SPI_Master::wait()
{
  while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M);
}

#endif
