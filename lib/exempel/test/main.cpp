#include "lcd.h"
#include <mik32_memory_map.h>
#include <power_manager.h>
#include "spi.h"
#include "mik32_hal_spi.h"

#define SPI_MISO_1(x)   x (1, 0)
#define SPI_MOSI_1(x)   x (1, 1)
#define SPI_SCK_1(x)    x (1, 2)
#define SPI_SS_1(x)     x (1, 3)

using namespace Core;
#pragma GCC optimize "O1"
int main(void)
{
  init_clock();


  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_SPI_1_M;      // Тактирование модуля

  // Настройка порта ввода/вывода
  SPI_MISO_1(SER); SPI_MOSI_1(SER); SPI_SCK_1(SER); SPI_SS_1(SER);
  SPI_MISO_1(NC); SPI_MOSI_1(NC); SPI_SCK_1(NC); SPI_SS_1(VCC);


  SPI_1->ENABLE = 0;                                // Отключение модуля
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_RX_FIFO_M;       // Очищение FIFO
  SPI_1->ENABLE = SPI_ENABLE_CLEAR_TX_FIFO_M;       // Очищение FIFO
  // volatile uint32_t unused = SPI_1->INT_STATUS;     /* Очистка флагов ошибок чтением */
  // (void)unused;


  SPI_1->INT_DISABLE = 0x3F;                        // Сброс маски прерываний

  SPI_1->CONFIG =
    SPI_CONFIG_MANUAL_CS_M                          // Ручной режим
    | SPI_CONFIG_CS_NONE_M                          // Устройства не выбраны
    | (0 << SPI_CONFIG_BAUD_RATE_DIV_S) // Делитель частоты
    | (0 << SPI_CONFIG_CLK_POL_S)                // Фаза и полярность
    | SPI_CONFIG_MASTER_M;                          // Мастер

  SPI_1->DELAY = 0;


  SPI_1->TX_THR = 6;  // Задает уровень, при котором TX_FIFO считается не заполненным 1-8

  SPI_1->ENABLE = SPI_ENABLE_M;                     // Включение модуля

  uint32_t x = 0;
  while (true) {
    while (!(SPI_1->INT_STATUS & SPI_INT_STATUS_TX_FIFO_NOT_FULL_M));
    SPI_1->TXDATA = x;
    // SPI_1->TXDATA = 0xaa;
    // SPI_1->TXDATA = 0xaa;
    // while (SPI_1->INT_STATUS & SPI_INT_STATUS_SPI_ACTIVE_M);

  };
}
