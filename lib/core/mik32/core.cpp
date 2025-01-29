#ifdef MIK32V2
#include "core.h"

/* Поддерживаемые частоты 32 MHz F_CPU */
volatile void delay_us(volatile uint32_t us)
{
  us <<= 2;
  while (us--);
}

volatile void delay_ms(volatile uint32_t ms)
{
  while (ms--) delay_us(999);
}

void init_clock()
{
  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_UART_0_M | PM_CLOCK_APB_P_GPIO_0_M | PM_CLOCK_APB_P_GPIO_1_M | PM_CLOCK_APB_P_GPIO_2_M; // включение тактирования GPIO
  PM->CLK_APB_M_SET |= PM_CLOCK_APB_M_PAD_CONFIG_M | PM_CLOCK_APB_M_WU_M | PM_CLOCK_APB_M_PM_M;			// включение тактирования блока для смены режима выводов
}

#endif
