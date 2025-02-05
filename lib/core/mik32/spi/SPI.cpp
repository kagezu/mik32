#ifdef MIK32V2
#include "SPI.h"

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

  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M;      // Тактирование модуля

  // Настройка порта ввода/вывода
  SPI_MISO_1(SER); SPI_MOSI_1(SER); SPI_SCK_1(SER); SPI_SS_1(SER);
  SPI_MISO_1(NC); SPI_MOSI_1(NC); SPI_SCK_1(NC); SPI_SS_1(VCC);


  SPI_1->ENABLE = 0;                                // Отключение модуля
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M;       // Очищение FIFO
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_TX_FIFO_M;       // Очищение FIFO
  volatile uint32_t unused = SPI_1->INT_STATUS;     /* Очистка флагов ошибок чтением */
  (void)unused;

  SPI_1->INT_DISABLE = 0x3F;                        // Сброс маски прерываний

  SPI_1->CONFIG =
    SPI_CONFIG_MANUAL_CS_M                          // Ручной режим
    | SPI_CONFIG_CS_NONE_M                          // Устройства не выбраны
    | (baud_rate_div << SPI_CONFIG_BAUD_RATE_DIV_S) // Делитель частоты
    | (mode << SPI_CONFIG_CLK_POL_S)                // Фаза и полярность
    | SPI_CONFIG_MASTER_M;                          // Мастер

  SPI_1->DELAY = 0;
  SPI_1->TX_THR = 7;                                // Задает уровень, при котором TX_FIFO считается не заполненным 1-8

  SPI_1->ENABLE = SPI_ENABLE_M;                     // Включение модуля
}

void SPI_Master::end()
{
  SPI_1->ENABLE = 0;                                // Отключение модуля
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M;       // Очищение FIFO
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_TX_FIFO_M;       // Очищение FIFO
  volatile uint32_t unused = SPI_1->INT_STATUS;     /* Очистка флагов ошибок чтением */
  (void)unused;
}

uint8_t SPI_Master::transfer(uint8_t data)
{
  clear_fifo();
  SPI_1->TXDATA = data;
  wait();
  return  SPI_1->RXDATA;
}

uint16_t SPI_Master::transfer16(uint16_t data)
{
  uint16_t rx_dbyte;
  clear_fifo();
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data;
  wait();
  rx_dbyte = SPI_1->RXDATA << 8;
  rx_dbyte |= SPI_1->RXDATA;

  return rx_dbyte;
}

uint32_t SPI_Master::read32()
{
  uint32_t rx = SPI_1->RXDATA;
  rx = (rx << 8) | SPI_1->RXDATA;
  rx = (rx << 8) | SPI_1->RXDATA;
  rx = (rx << 8) | SPI_1->RXDATA;
  return rx;
}

void SPI_Master::write32(uint32_t data)
{
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data;
}

#endif
