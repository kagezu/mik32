#include "mik32.h"

extern "C" {
  // extern void *spifi_limit;

  // __attribute__((used, section(".small_ram_text"))) void SystemInit()
  __attribute__((used)) void SystemInit()
  {
    // Модуль WakeUp ====================================================================

    // Батарейный домен
    WU->CLOCKS_BU = 0
      // | WU_CLOCKS_BU_OSC32K_EN_M             // Отключение OSC32K (0 - включение)
      | WU_CLOCKS_BU_LSI32K_EN_M                // Отключение LSI32К (0 - включение)
      //  | WU_CLOCKS_BU_RTC_CLK_MUX_LSI32K_M;  // Выбрать внутренний LSI32К
      | WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M       // Выбрать внешний OSC32К
      ;

    // Системный домен
    WU->CLOCKS_SYS = 0
      | WU_CLOCKS_SYS_HSI32M_EN_M               // Отключить внутренний HSI32M (0 - включение)
      // | WU_CLOCKS_SYS_OSC32M_EN_M            // Отключить внешний OSC32M (0 - включение)
      | WU_CLOCKS_SYS_FORCE_32K_CLK_OSC32K_M    // Принудительно выбрать OSC32K
      ;

    // Выбор источника тактирования системы
    PM->AHB_CLK_MUX =
      PM_AHB_CLK_MUX_OSC32M_M       // 0 – внешний OSC32M
      // PM_AHB_CLK_MUX_HSI32M_M    // 1 – внутренний HSI32M
      // PM_AHB_CLK_MUX_OSC32K_M    // 2 – внешний OSC32K
      // PM_AHB_CLK_MUX_LSI32K_M    // 3 – внутренний LSI32К
      | PM_AHB_FORCE_MUX_FIXED      // запрет автоматической смены частоты
      ;

    // Выбор источника тактирования сторожевого таймера
    PM->WDT_CLK_MUX =
      PM_WDT_CLK_MUX_OSC32M_M       // 0 – внешний OSC32M
      // PM_WDT_CLK_MUX_HSI32M_M    // 1 – внутренний HSI32M
      // PM_WDT_CLK_MUX_OSC32K_M    // 2 – внешний OSC32K
      // PM_WDT_CLK_MUX_LSI32K_M    // 3 – внутренний LSI32К
      ;


    // Модуль PowerManager ==============================================================

    // PM->DIV_AHB = 0;    // Задает значение делителя шины AHB.
    // PM->DIV_APB_M = 0;  // Задает значение делителя шины APB_M.
    // PM->DIV_APB_P = 0;  // Задает значение делителя шины APB_P.

    // Включить тактирование модулей
    PM->CLK_AHB_SET = 0
      | PM_CLOCK_AHB_CPU_M
      | PM_CLOCK_AHB_EEPROM_M
      | PM_CLOCK_AHB_RAM_M
      | PM_CLOCK_AHB_SPIFI_M
      // | PM_CLOCK_AHB_TCB_M
      | PM_CLOCK_AHB_DMA_M
      // | PM_CLOCK_AHB_CRYPTO_M
      // | PM_CLOCK_AHB_CRC32_M
      ;

    // Отключить тактирование модулей
    // PM->CLK_AHB_CLEAR = 0
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
    PM->CLK_APB_M_SET = 0
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
      // PM->CLK_APB_M_CLEAR = 0
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
    PM->CLK_APB_P_SET = 0
      // PM_CLOCK_APB_P_WDT_M
      // | PM_CLOCK_APB_P_UART_0_M
      // | PM_CLOCK_APB_P_UART_1_M
      // | PM_CLOCK_APB_P_TIMER16_0_M
      // | PM_CLOCK_APB_P_TIMER16_1_M
      // | PM_CLOCK_APB_P_TIMER16_2_M
      // | PM_CLOCK_APB_P_TIMER32_1_M
      // | PM_CLOCK_APB_P_TIMER32_2_M
      // | PM_CLOCK_APB_P_SPI_0_M
      | PM_CLOCK_APB_P_SPI_1_M
      // | PM_CLOCK_APB_P_I2C_0_M
      // | PM_CLOCK_APB_P_I2C_1_M
      | PM_CLOCK_APB_P_GPIO_0_M
      | PM_CLOCK_APB_P_GPIO_1_M
      | PM_CLOCK_APB_P_GPIO_2_M
      | PM_CLOCK_APB_P_ANALOG_REGS_M
      // | PM_CLOCK_APB_P_GPIO_IRQ_M
      ;

    // Отключения тактирования устройств на шине APB_P.
    // PM->CLK_APB_P_CLEAR = 0
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

  /*

    if (BOOT_MANAGER->BOOT != BOOT_SPIFI) return;

    // Настройка SPIFI ================================================================

    // сброс команды SPIFI
    SPIFI_CONFIG->STAT = SPIFI_CONFIG_STAT_RESET_M;
    while (SPIFI_CONFIG->STAT & SPIFI_CONFIG_STAT_RESET_M);


    // Enable QPI (38h) (передача всего по 4 линиям)
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD =
      (1 << SPIFI_CONFIG_CMD_FRAMEFORM_S) // только код команды
      | (0x38 << SPIFI_CONFIG_CMD_OPCODE_S)
      ;
    // ожидаем завершения отработки команды
    while (!(SPIFI_CONFIG->STAT & SPIFI_CONFIG_STAT_INTRQ_M));


    // Настройка Fast Read Quad I/O на работу с передачей исключительно адреса
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->ADDR = 0;
    SPIFI_CONFIG->IDATA = 0x20; // содержимое первого dummy байта команды Fast Read Quad I/O (EBh)
    SPIFI_CONFIG->CMD =
      (1 << SPIFI_CONFIG_CMD_DATALEN_S)         // прочитаем один байт
      | (1 << SPIFI_CONFIG_MCMD_INTLEN_S)       // один dummy байт 0x0 для режима QPI
      | (3 << SPIFI_CONFIG_MCMD_FIELDFORM_S)    // всё по четырём
      | (4 << SPIFI_CONFIG_MCMD_FRAMEFORM_S)    // код команды и три байта адреса
      | (0xEB << SPIFI_CONFIG_MCMD_OPCODE_S)
      ;
    // читаем один байт
    SPIFI_CONFIG->DATA8;
    // ожидаем завершения отработки команды
    while (!(SPIFI_CONFIG->STAT & SPIFI_CONFIG_STAT_INTRQ_M));


    // настроим SPIFI на работу "с памятью"
    SPIFI_CONFIG->CTRL = (SPIFI_CONFIG->CTRL & ~(SPIFI_CONFIG_CTRL_CSHIGH_M))
      | (0 << SPIFI_CONFIG_CTRL_CSHIGH_S)       // 1 такт сигнала SCK между командами
      | SPIFI_CONFIG_CTRL_CACHE_EN_M            // включение кэширования
      | SPIFI_CONFIG_CTRL_D_CACHE_DIS_M         // отключение кэширования данных
      ;
    SPIFI_CONFIG->ADDR = 0;
    SPIFI_CONFIG->IDATA = 0x20; // содержимое первого dummy байта команды Fast Read Quad I/O (EBh) (продолжаем использовать режим чтения без кода команды)

    SPIFI_CONFIG->CLIMIT = (uint32_t)&spifi_limit;// граница кэширования

    SPIFI_CONFIG->MCMD =
      (1 << SPIFI_CONFIG_MCMD_INTLEN_S)         // один dummy байт 0x00 для режима QPI и только адреса
      | (3 << SPIFI_CONFIG_MCMD_FIELDFORM_S)    // всё по четырём
      | (6 << SPIFI_CONFIG_MCMD_FRAMEFORM_S)    // код команды, три байта адреса
      | (0xEB << SPIFI_CONFIG_MCMD_OPCODE_S)
      ;

      }

      extern "C" {

        // extern void (*__preinit_array_start[]) (void) __attribute__((weak));
        // extern void (*__preinit_array_end[]) (void) __attribute__((weak));
        extern void (*__init_array_start[]) (void) __attribute__((weak));
        extern void (*__init_array_end[]) (void) __attribute__((weak));

        // void *__dso_handle;

        // int __cxa_atexit(void (*fn) (void *), void *arg, void *d) { return 0; }

        __attribute__((used)) void SystemInit()
        {
          uint32_t count;
          uint32_t i;

          _init();

          // count = __preinit_array_end - __preinit_array_start;
          // for (i = 0; i < count; ++i)
          //   __preinit_array_start[i]();

          // count = __init_array_end - __init_array_start;
          // for (i = 0; i < count; i++)
          //   __init_array_start[i]();
        }
      */
  }
}


/* Для частоты: 32 MHz F_CPU */
// __attribute__((noinline, section(".ram_text"))) void delay_us(uint32_t us)
// {
//   if (!us) return;
//   int l = 4;
//   asm volatile (
//     "1:                 \n\t"
//     "addi %0, %0, -1    \n\t" // 1 такт
//     "bnez %0, 1b        \n\t" // 2 такта
//     :"=r"(l) : "0" (l)
//     );
//   us--;
//   us = us << 3; // 8 циклов на микросекунду
//   if (!us) return; // 1 микросекунда с учётом вызова

//   asm volatile (
//     "1:                 \n\t"
//     "xor x0, x0, x0     \n\t" // 1 такт
//     "addi %0, %0, -1    \n\t" // 1 такт
//     "bne x0, %0, 1b     \n\t" // 2 такта
//     : "=r" (us) : "0" (us)
//     );
// }

// __attribute__((noinline, section(".ram_text"))) void delay_ms(uint32_t ms)
// {
//   while (ms--) delay_us(1000);
// }
