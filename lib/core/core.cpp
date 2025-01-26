#include "core.h"

/* Поддерживаемые частоты 32 MHz F_CPU */
void delay_us(uint32_t us)
{
  us <<= 3;
  while (us--);
}

void delay_ms(uint32_t ms)
{
  while (ms--) delay_us(999);
}
