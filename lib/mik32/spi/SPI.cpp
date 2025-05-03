#include "SPI.h"

// SPI 0

void CSPI0::init()
{
  // Тактирование модуля
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_0_M;

  // Настройка порта ввода/вывода
  SPI_MISO_0(SER); SPI_MOSI_0(SER); SPI_SCK_0(SER); SPI_SS_0(SER);
  SPI_MISO_0(NC); SPI_MOSI_0(NC); SPI_SCK_0(NC); SPI_SS_0(VCC);

  SPI_0->ENABLE = 0;                          // Отключение модуля
  SPI_0->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M
    | SPI_ENABLE_CLEAR_TX_FIFO_M;             // Очищение FIFO
  wait_clr();                                 // зависнуть, если буфер не очистился.

  // Сброс маски прерываний
  SPI_0->INT_DISABLE = 0x3F;

  SPI_0->CONFIG =
    SPI_CONFIG_MANUAL_CS_M                    // Ручной режим
    | SPI_CONFIG_CS_NONE_M                    // Устройства не выбраны
    | SPI_CONFIG_MASTER_M;                    // Мастер

  SPI_0->DELAY = SPI_DELAY_DIV;
  SPI_0->TX_THR = SPI_TX_THR;
  SPI_0->ENABLE = SPI_ENABLE_M;               // Включение модуля
}

void CSPI0::begin(SPI_Settings settings)
{
  // Очищение FIFO
  SPI_0->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M | SPI_ENABLE_CLEAR_TX_FIFO_M;
  SPI_0->CONFIG = settings.config;
  SPI_0->ENABLE = SPI_ENABLE_M;               // Включение модуля
}

void CSPI0::end()
{
  wait_clr();
  SPI_0->CONFIG |= SPI_CONFIG_CS_NONE_M;            // Отключение устройства
  SPI_0->ENABLE = SPI_ENABLE_M;                     // Отключение модуля
}

uint8_t CSPI0::transfer(uint8_t data)
{
  SPI_0->TXDATA = data;
  wait_clr();
  return  SPI_0->RXDATA;
}

uint16_t CSPI0::transfer16(uint16_t data)
{
  uint16_t rx_dbyte;
  SPI_0->TXDATA = data >> 8;
  SPI_0->TXDATA = data;
  wait_clr();
  rx_dbyte = SPI_0->RXDATA << 8;
  rx_dbyte |= SPI_0->RXDATA;

  return rx_dbyte;
}

void CSPI0::clear_rx()
{
  volatile uint32_t dummy;
  while ((SPI_0->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M))
    dummy = SPI_0->RXDATA;
  (void)dummy;
}

// SPI 1 //////////////////////////////////////////////////////////////////

void CSPI1::init()
{
  // Тактирование модуля
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M;

  // Настройка порта ввода/вывода
  SPI_MISO_1(SER); SPI_MOSI_1(SER); SPI_SCK_1(SER); SPI_SS_1(SER);
  SPI_MISO_1(NC); SPI_MOSI_1(NC); SPI_SCK_1(NC); SPI_SS_1(VCC);

  SPI_1->ENABLE = 0;                          // Отключение модуля
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M
    | SPI_ENABLE_CLEAR_TX_FIFO_M;             // Очищение FIFO
  wait_clr();                                 // зависнуть, если буфер не очистился.

  // Сброс маски прерываний
  SPI_1->INT_DISABLE = 0x3F;

  SPI_1->CONFIG =
    SPI_CONFIG_MANUAL_CS_M                    // Ручной режим
    | SPI_CONFIG_CS_NONE_M                    // Устройства не выбраны
    | SPI_CONFIG_MASTER_M;                    // Мастер

  SPI_1->DELAY = SPI_DELAY_DIV;
  SPI_1->TX_THR = SPI_TX_THR;
  SPI_1->ENABLE = SPI_ENABLE_M;               // Включение модуля
}

void CSPI1::begin(SPI_Settings settings)
{
  // Очищение FIFO
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M | SPI_ENABLE_CLEAR_TX_FIFO_M;
  SPI_1->CONFIG = settings.config;
  SPI_1->ENABLE = SPI_ENABLE_M;               // Включение модуля
}

void CSPI1::end()
{
  wait_clr();
  SPI_1->CONFIG |= SPI_CONFIG_CS_NONE_M;            // Отключение устройства
  SPI_1->ENABLE = SPI_ENABLE_M;                     // Отключение модуля
}

uint8_t CSPI1::transfer(uint8_t data)
{
  SPI_1->TXDATA = data;
  wait_clr();
  return  SPI_1->RXDATA;
}

uint16_t CSPI1::transfer16(uint16_t data)
{
  uint16_t rx_dbyte;
  SPI_1->TXDATA = data >> 8;
  SPI_1->TXDATA = data;
  wait_clr();
  rx_dbyte = SPI_1->RXDATA << 8;
  rx_dbyte |= SPI_1->RXDATA;

  return rx_dbyte;
}

void CSPI1::clear_rx()
{
  volatile uint32_t dummy;
  while ((SPI_1->INT_STATUS & SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M))
    dummy = SPI_1->RXDATA;
  (void)dummy;
}
