#include "SPI.h"

void CSPI::init(uint8_t spi_n)
{
  if (!spi_n) {
    // Тактирование модуля
    PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_0_M;

    // Настройка порта ввода/вывода
    SPI_MISO_0(SER); SPI_MOSI_0(SER); SPI_SCK_0(SER); SPI_SS_0(SER);
    SPI_MISO_0(NC); SPI_MOSI_0(NC); SPI_SCK_0(NC); SPI_SS_0(VCC);

    SPI_N = SPI_0;
  }
  else {
    // Тактирование модуля
    PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M;

    // Настройка порта ввода/вывода
    SPI_MISO_1(SER); SPI_MOSI_1(SER); SPI_SCK_1(SER); SPI_SS_1(SER);
    SPI_MISO_1(NC); SPI_MOSI_1(NC); SPI_SCK_1(NC); SPI_SS_1(VCC);

    SPI_N = SPI_1;
  }

  SPI_N->ENABLE = 0;                          // Отключение модуля
  SPI_N->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M
    | SPI_ENABLE_CLEAR_TX_FIFO_M;             // Очищение FIFO
  wait_clr();                                 // зависнуть, если буфер не очистился.

  // Сброс маски прерываний
  SPI_N->INT_DISABLE = 0x3F;

  SPI_N->CONFIG =
    SPI_CONFIG_MANUAL_CS_M                    // Ручной режим
    | SPI_CONFIG_CS_NONE_M                    // Устройства не выбраны
    | SPI_CONFIG_MASTER_M;                    // Мастер

  SPI_N->DELAY = SPI_DELAY_DIV;
  SPI_N->TX_THR = SPI_TX_THR;
  SPI_N->ENABLE = SPI_ENABLE_M;               // Включение модуля
}

void CSPI::begin(SPI_Settings settings)
{
  // Очищение FIFO
  SPI_N->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M | SPI_ENABLE_CLEAR_TX_FIFO_M;
  SPI_N->CONFIG = settings.config;
  SPI_N->TX_THR = settings.tx_thr;
  SPI_N->ENABLE = SPI_ENABLE_M;               // Включение модуля
}

void CSPI::end()
{
  wait_clr();
  SPI_N->CONFIG |= SPI_CONFIG_CS_NONE_M;            // Отключение устройства
  SPI_N->ENABLE = SPI_ENABLE_M;                     // Отключение модуля
}

uint8_t CSPI::transfer(uint8_t data)
{
  SPI_N->TXDATA = data;
  wait_clr();
  return  SPI_N->RXDATA;
}

uint16_t CSPI::transfer16(uint16_t data)
{
  uint16_t rx_dbyte;
  SPI_N->TXDATA = data >> 8;
  SPI_N->TXDATA = data;
  wait_clr();
  rx_dbyte = SPI_N->RXDATA << 8;
  rx_dbyte |= SPI_N->RXDATA;
  return rx_dbyte;
}

void CSPI::clear_rx()
{
  volatile uint32_t dummy;
  while ((SPI_N->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M))
    dummy = SPI_N->RXDATA;
  (void)dummy;
}
