#include "mik32.h"

extern "C" {
  // extern uint8_t __RODATA__[];

  __attribute__((used, section(".small_ram_text"))) void SystemInit()
    // __attribute__((used)) void SystemInit()
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
      | PM_CLOCK_APB_M_RTC_M
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
      | PM_CLOCK_APB_P_UART_0_M
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

    // Настройка SPIFI ========================================================
/*
    // сброс команды SPIFI
    SPIFI_CONFIG->STAT = SPIFI_CONFIG_STAT_RESET_M;

    if (BOOT_MANAGER->BOOT == BOOT_SPIFI) {

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
    }

    // настроим SPIFI на работу "с памятью"
    SPIFI_CONFIG->CTRL = 0              // [15:0]  Зарезервировано
      | SPIFI_CONFIG_CTRL_CSHIGH(0)     // [19:16] Тактов между командами
      | SPIFI_CONFIG_CTRL_CACHE_EN_M    // [20]    Бит разрешения кэширования
      | SPIFI_CONFIG_CTRL_D_CACHE_DIS_M // [21]    Бит запрещения кеширования данных
      // | SPIFI_CONFIG_CTRL_INTEN_M       // [22]    Бит разрешения прерывания
      // | SPIFI_CONFIG_CTRL_MODE3_M       // [23]    Бит режима 3
      // | SPIFI_CONFIG_CTRL_SCK_DIV(0)    // [26:24] Делитель тактового сигнала
      // | SPIFI_CONFIG_CTRL_PREFETCH_DIS_M// [27]    Бит запрещения упреждающих выборок кэш памяти
      // | SPIFI_CONFIG_CTRL_DUAL_M        // [28]    Бит выбора протокола
      // | SPIFI_CONFIG_CTRL_RFCLK_M       // [29]
      | SPIFI_CONFIG_CTRL_FBCLK_M       // [30]
      // | SPIFI_CONFIG_CTRL_DMAEN_M       // [31]    Бит разрешения запросов DMA
      ;

    SPIFI_CONFIG->CLIMIT = (uintptr_t)__RODATA__ - SPIFI_BASE_ADDRESS;  // установить новый CLIMIT
    // SPIFI_CONFIG->ADDR = 0;
    // SPIFI_CONFIG->IDATA = 0x20; // содержимое первого dummy байта команды Fast Read Quad I/O (EBh)
    SPIFI_CONFIG->MCMD =
      (1 << SPIFI_CONFIG_MCMD_INTLEN_S)         // один dummy байт 0x00 для режима QPI и только адреса
      | (3 << SPIFI_CONFIG_MCMD_FIELDFORM_S)    // всё по четырём
      | (6 << SPIFI_CONFIG_MCMD_FRAMEFORM_S)    // код команды, три байта адреса
      | (0xEB << SPIFI_CONFIG_MCMD_OPCODE_S)
      ;
      */
  }
}
