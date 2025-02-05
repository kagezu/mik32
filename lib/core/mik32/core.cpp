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

void cli()
{}

void sei()
{}

#endif
