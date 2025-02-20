#include <mik32_memory_map.h>
#include <power_manager.h>
#include <wakeup.h>
// #include <scr1_csr_encoding.h>
// #include <csr.h>

void init_system()
{
  // Модуль WakeUp

  // Батарейный домен
  WU->CLOCKS_BU |=
    // WU_CLOCKS_BU_OSC32K_EN_M              // Отключение OSC32K
    WU_CLOCKS_BU_OSC32K_EN_M              // Отключение LSI32К
  //  | WU_CLOCKS_BU_RTC_CLK_MUX_LSI32K_M;   // Выбрать внутренний LSI32К
    | WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M;  // Выбрать внешний OSC32К

  // Системный домен
  WU->CLOCKS_SYS |= WU_CLOCKS_SYS_HSI32M_EN_M; // Отключить внутренний HSI32M

  // Модуль PowerManager

  PM->DIV_AHB = 0;    // Задает значение делителя шины AHB.
  PM->DIV_APB_M = 0;  // Задает значение делителя шины APB_M.
  PM->DIV_APB_P = 0;  // Задает значение делителя шины APB_P.

  // Включить тактирование модулей
  PM->CLK_AHB_SET |=
    PM_CLOCK_AHB_CPU_M
    | PM_CLOCK_AHB_EEPROM_M
    | PM_CLOCK_AHB_RAM_M
    | PM_CLOCK_AHB_SPIFI_M
    | PM_CLOCK_AHB_TCB_M;
  // | PM_CLOCK_AHB_DMA_M
  // | PM_CLOCK_AHB_CRYPTO_M
  // | PM_CLOCK_AHB_CRC32_M;

  // Отключить тактирование модулей
  // PM->CLK_AHB_CLEAR |=
    // | PM_CLOCK_AHB_CPU_M
    // | PM_CLOCK_AHB_EEPROM_M
    // | PM_CLOCK_AHB_RAM_M
    // | PM_CLOCK_AHB_SPIFI_M
    // | PM_CLOCK_AHB_TCB_M;
    // | PM_CLOCK_AHB_DMA_M
    // | PM_CLOCK_AHB_CRYPTO_M
    // | PM_CLOCK_AHB_CRC32_M;

  // Включение тактирования устройств на шине APB_M.
  PM->CLK_APB_M_SET |=
    PM_CLOCK_APB_M_PM_M
    | PM_CLOCK_APB_M_EPIC_M
    // | PM_CLOCK_APB_M_TIMER32_0_M
    | PM_CLOCK_APB_M_PAD_CONFIG_M
    // | PM_CLOCK_APB_M_WDT_BUS_M
    // | PM_CLOCK_APB_M_OTP_CONTROLLER_M
    // | PM_CLOCK_APB_M_PVD_CONTROL_M
    | PM_CLOCK_APB_M_WU_M;
  // | PM_CLOCK_APB_M_RTC_M;

// Отключение тактирования устройств на шине APB_M.
  // PM->CLK_APB_M_CLEAR |=
  //   PM_CLOCK_APB_M_PM_M
  //   | PM_CLOCK_APB_M_EPIC_M
  //   | PM_CLOCK_APB_M_TIMER32_0_M
  //   | PM_CLOCK_APB_M_PAD_CONFIG_M
  //   | PM_CLOCK_APB_M_WDT_BUS_M
  //   | PM_CLOCK_APB_M_OTP_CONTROLLER_M
  //   | PM_CLOCK_APB_M_PVD_CONTROL_M
  //   | PM_CLOCK_APB_M_WU_M
  //   | PM_CLOCK_APB_M_RTC_M;

  // Включения тактирования устройств на шине APB_P.
  PM->CLK_APB_P_SET |=
    // PM_CLOCK_APB_P_WDT_M
    // | PM_CLOCK_APB_P_UART_0_M
    // | PM_CLOCK_APB_P_UART_1_M
    // | PM_CLOCK_APB_P_TIMER16_0_M
    // | PM_CLOCK_APB_P_TIMER16_1_M
    // | PM_CLOCK_APB_P_TIMER16_2_M
    // | PM_CLOCK_APB_P_TIMER32_1_M
    // | PM_CLOCK_APB_P_TIMER32_2_M
    // | PM_CLOCK_APB_P_SPI_0_M
    // | PM_CLOCK_APB_P_SPI_1_M
    // | PM_CLOCK_APB_P_I2C_0_M
    // | PM_CLOCK_APB_P_I2C_1_M
    PM_CLOCK_APB_P_GPIO_0_M
    | PM_CLOCK_APB_P_GPIO_1_M
    | PM_CLOCK_APB_P_GPIO_2_M;
  // | PM_CLOCK_APB_P_ANALOG_REGS_M
  // | PM_CLOCK_APB_P_GPIO_IRQ_M;

// Отключения тактирования устройств на шине APB_P.
// PM->CLK_APB_P_CLEAR |=
  // PM_CLOCK_APB_P_WDT_M
  // | PM_CLOCK_APB_P_UART_0_M
  // | PM_CLOCK_APB_P_UART_1_M
  // | PM_CLOCK_APB_P_TIMER16_0_M
  // | PM_CLOCK_APB_P_TIMER16_1_M
  // | PM_CLOCK_APB_P_TIMER16_2_M
  // | PM_CLOCK_APB_P_TIMER32_1_M
  // | PM_CLOCK_APB_P_TIMER32_2_M
  // | PM_CLOCK_APB_P_SPI_0_M
  // | PM_CLOCK_APB_P_SPI_1_M
  // | PM_CLOCK_APB_P_I2C_0_M
  // | PM_CLOCK_APB_P_I2C_1_M
  // | PM_CLOCK_APB_P_GPIO_0_M
  // | PM_CLOCK_APB_P_GPIO_1_M
  // | PM_CLOCK_APB_P_GPIO_2_M
  // | PM_CLOCK_APB_P_ANALOG_REGS_M
  // | PM_CLOCK_APB_P_GPIO_IRQ_M;
}

