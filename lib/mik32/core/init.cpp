#include <mik32_memory_map.h>
#include <power_manager.h>
#include <wakeup.h>
// #include <scr1_csr_encoding.h>
// #include <csr.h>

void init_system()
{
  // Модуль WakeUp

  // Батарейный домен
  WU->CLOCKS_BU |= 0
    // | WU_CLOCKS_BU_OSC32K_EN_M              // Отключение OSC32K (0 - включение)
    | WU_CLOCKS_BU_LSI32K_EN_M             // Отключение LSI32К (0 - включение)
    //  | WU_CLOCKS_BU_RTC_CLK_MUX_LSI32K_M;   // Выбрать внутренний LSI32К
    | WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M  // Выбрать внешний OSC32К
    ;

  // Системный домен
  WU->CLOCKS_SYS |= 0
    | WU_CLOCKS_SYS_HSI32M_EN_M // Отключить внутренний HSI32M (0 - включение)
    // | WU_CLOCKS_SYS_OSC32M_EN_M // Отключить внутренний OSC32M (0 - включение)
    | WU_CLOCKS_SYS_FORCE_32K_CLK_OSC32K_M // Принудительно выбрать OSC32K
    ;

  // Модуль PowerManager

  PM->DIV_AHB = 0;    // Задает значение делителя шины AHB.
  PM->DIV_APB_M = 0;  // Задает значение делителя шины APB_M.
  PM->DIV_APB_P = 0;  // Задает значение делителя шины APB_P.

  // Включить тактирование модулей
  PM->CLK_AHB_SET |= 0
    // | PM_CLOCK_AHB_CPU_M
    // | PM_CLOCK_AHB_EEPROM_M
    // | PM_CLOCK_AHB_RAM_M
    // | PM_CLOCK_AHB_SPIFI_M
    // | PM_CLOCK_AHB_TCB_M
    // | PM_CLOCK_AHB_DMA_M
    // | PM_CLOCK_AHB_CRYPTO_M
    // | PM_CLOCK_AHB_CRC32_M
    ;

  // Отключить тактирование модулей
  // PM->CLK_AHB_CLEAR |= 0
    // | PM_CLOCK_AHB_CPU_M
    // | PM_CLOCK_AHB_EEPROM_M
    // | PM_CLOCK_AHB_RAM_M
    // | PM_CLOCK_AHB_SPIFI_M
    // | PM_CLOCK_AHB_TCB_M
    // | PM_CLOCK_AHB_DMA_M
    // | PM_CLOCK_AHB_CRYPTO_M
    // | PM_CLOCK_AHB_CRC32_M
    // ;

  // Включение тактирования устройств на шине APB_M.
  PM->CLK_APB_M_SET |= 0
    | PM_CLOCK_APB_M_PM_M
    | PM_CLOCK_APB_M_EPIC_M
    // | PM_CLOCK_APB_M_TIMER32_0_M
    | PM_CLOCK_APB_M_PAD_CONFIG_M
    // | PM_CLOCK_APB_M_WDT_BUS_M
    // | PM_CLOCK_APB_M_OTP_CONTROLLER_M
    // | PM_CLOCK_APB_M_PVD_CONTROL_M
    | PM_CLOCK_APB_M_WU_M
    // | PM_CLOCK_APB_M_RTC_M
    ;

  // Отключение тактирования устройств на шине APB_M.
    // PM->CLK_APB_M_CLEAR |= 0
    //   PM_CLOCK_APB_M_PM_M
    //   | PM_CLOCK_APB_M_EPIC_M
    //   | PM_CLOCK_APB_M_TIMER32_0_M
    //   | PM_CLOCK_APB_M_PAD_CONFIG_M
    //   | PM_CLOCK_APB_M_WDT_BUS_M
    //   | PM_CLOCK_APB_M_OTP_CONTROLLER_M
    //   | PM_CLOCK_APB_M_PVD_CONTROL_M
    //   | PM_CLOCK_APB_M_WU_M
    //   | PM_CLOCK_APB_M_RTC_M;
    // ;

    // Включения тактирования устройств на шине APB_P.
  PM->CLK_APB_P_SET |= 0
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
    | PM_CLOCK_APB_P_GPIO_0_M
    | PM_CLOCK_APB_P_GPIO_1_M
    | PM_CLOCK_APB_P_GPIO_2_M
    // | PM_CLOCK_APB_P_ANALOG_REGS_M
    // | PM_CLOCK_APB_P_GPIO_IRQ_M
    ;

  // Отключения тактирования устройств на шине APB_P.
  // PM->CLK_APB_P_CLEAR |= 0
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
    // ;
}


#ifdef __cplusplus
extern "C" {
#endif
  extern void (*__preinit_array_start[]) (void) __attribute__((weak));
  extern void (*__preinit_array_end[]) (void) __attribute__((weak));
  extern void (*__init_array_start[]) (void) __attribute__((weak));
  extern void (*__init_array_end[]) (void) __attribute__((weak));

  void *__dso_handle;

  int __cxa_atexit(void (*fn) (void *), void *arg, void *d)
  {
    return 0;
  }

  void SystemInit()
  {
    uint32_t count;
    uint32_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; ++i)
      __preinit_array_start[i]();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
      __init_array_start[i]();
  }
#ifdef __cplusplus
}
#endif
