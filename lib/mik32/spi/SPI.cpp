#ifdef MIK32V2
#include "SPI.h"

SPI_Class SPI;

/* Номер интерфейса SPI */
void SPI_Class::init(uint8_t spi_n)
{
  // Тактирование модуля
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M;

  // Настройка порта ввода/вывода
  if (spi_n) {
    // SPI_N = SPI_1;
    SPI_MISO_1(SER); SPI_MOSI_1(SER); SPI_SCK_1(SER); SPI_SS_1(SER);
    SPI_MISO_1(NC); SPI_MOSI_1(NC); SPI_SCK_1(NC); SPI_SS_1(VCC);
  }
  else {
    // SPI_N = SPI_0;
    SPI_MISO_0(SER); SPI_MOSI_0(SER); SPI_SCK_0(SER); SPI_SS_0(SER);
    SPI_MISO_0(NC); SPI_MOSI_0(NC); SPI_SCK_0(NC); SPI_SS_0(VCC);
  }



  SPI_N->ENABLE = 0;                                // Отключение модуля
  SPI_N->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M;       // Очищение FIFO
  SPI_N->ENABLE = SPI_ENABLE_CLEAR_TX_FIFO_M;       // Очищение FIFO
  volatile uint32_t unused = SPI_N->INT_STATUS;     /* Очистка флагов ошибок чтением */
  (void)unused;


  // Сброс маски прерываний
  SPI_N->INT_DISABLE = 0x3F;



  SPI_N->CONFIG =
    SPI_CONFIG_MANUAL_CS_M                          // Ручной режим
    | SPI_CONFIG_CS_NONE_M                          // Устройства не выбраны
    | SPI_CONFIG_MASTER_M;                          // Мастер


  SPI_N->DELAY = 0;//SPI_DELAY;
  SPI_N->TX_THR = SPI_TX_THR;

  // Включение модуля
  SPI_N->ENABLE = SPI_ENABLE_M;


  // /* Очистка флагов ошибок чтением */
  // volatile uint32_t unused = SPI_N->INT_STATUS;
  // (void)unused;

}

void SPI_Class::begin(SPI_Settings settings)
{
  // Включение модуля
  // SPI_N->ENABLE = SPI_ENABLE_M;

  /* Очистка флагов ошибок чтением */
  // volatile uint32_t unused = SPI_N->INT_STATUS;
  // (void)unused;
  // SPI_N->CONFIG = settings.config;

  // SPI_N->DELAY = settings.delay_ext_clk;
  // SPI_N->TX_THR = settings.tx_thr;
}

void SPI_Class::end()
{
  wait_clr();
  //   SPI_N->CONFIG |= SPI_CONFIG_CS_NONE_M;            // Отключение устройства
  //   SPI_N->ENABLE = SPI_ENABLE_M;                     // Отключение модуля
  //   SPI_N->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M;       // Очищение FIFO
  //   SPI_N->ENABLE = SPI_ENABLE_CLEAR_TX_FIFO_M;       // Очищение FIFO
  //   volatile uint32_t unused = SPI_N->INT_STATUS;     /* Очистка флагов ошибок чтением */
  //   (void)unused;
}

uint8_t SPI_Class::transfer(uint8_t data)
{
  clear_rx();
  SPI_N->TXDATA = data;
  wait_clr();
  return  SPI_N->RXDATA;
}

uint16_t SPI_Class::transfer16(uint16_t data)
{
  uint16_t rx_dbyte;
  clear_rx();
  SPI_N->TXDATA = data >> 8;
  SPI_N->TXDATA = data;
  wait_clr();
  rx_dbyte = SPI_N->RXDATA << 8;
  rx_dbyte |= SPI_N->RXDATA;

  return rx_dbyte;
}

uint32_t SPI_Class::read32()
{
  uint32_t rx = SPI_N->RXDATA;
  rx = (rx << 8) | SPI_N->RXDATA;
  rx = (rx << 8) | SPI_N->RXDATA;
  rx = (rx << 8) | SPI_N->RXDATA;
  return rx;
}

void SPI_Class::write32(uint32_t data)
{
  SPI_N->TXDATA = data >> 8;
  SPI_N->TXDATA = data >> 8;
  SPI_N->TXDATA = data >> 8;
  SPI_N->TXDATA = data;
}

#endif
