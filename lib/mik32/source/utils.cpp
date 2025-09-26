#include "mik32.h"

/* Для частоты: 32 MHz F_CPU */
RAM
void delay_us(uint32_t us)
{
  if (!us) return;  // 7 тактов
  int l = 7;
  asm volatile (
    "1:                 \n\t"
    "addi %0, %0, -1    \n\t" // 1 такт
    "bnez %0, 1b        \n\t" // 2 такта
    :"=r"(l) : "0" (l)
    );
  us--;
  us = us << 3; // 8 циклов на микросекунду
  if (!us) return; // 1 микросекунда (32 такта)

  asm volatile (
    "lui x0, 0x10000    \n\t" // 1 такт
    "1:                 \n\t"
    "addi x0, x0, 0     \n\t" // 1 такт
    "addi %0, %0, -1    \n\t" // 1 такт
    "bne x0, %0, 1b     \n\t" // 2 такта
    : "=r" (us) : "0" (us)
    );
}

RAM
void delay_ms(uint32_t ms)
{
  if (!ms) return;
  while (--ms) delay_us(1000);
}

void sei()
{
  set_csr(mstatus, MSTATUS_MIE);
  // set_csr(mie, MIE_MEIE);
  // set_csr(mie, MIE_MTIE);
}

void cli()
{
  clear_csr(mstatus, MSTATUS_MIE);
  // clear_csr(mie, MIE_MEIE);
  // clear_csr(mie, MIE_MTIE);
}

// RAM
char *sjoin(char *dst, const char *src)
{
  while ((*dst = *src)) { dst++; src++; }
  return dst;
}

void RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks)
{
  switch (PM->AHB_CLK_MUX & PM_AHB_CLK_MUX_M) {
    case PM_AHB_CLK_MUX_OSC32M_M: RCC_Clocks->SYSCLK_Frequency = OSC_SYSTEM_VALUE; break;
    case PM_AHB_CLK_MUX_HSI32M_M: RCC_Clocks->SYSCLK_Frequency = OSC_SYSTEM_VALUE; break;
    case PM_AHB_CLK_MUX_OSC32K_M: RCC_Clocks->SYSCLK_Frequency = 0x8000; break;
    case PM_AHB_CLK_MUX_LSI32K_M: RCC_Clocks->SYSCLK_Frequency = 0x8000; break;
  }

  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency / (PM->DIV_AHB + 1);
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->SYSCLK_Frequency / (PM->DIV_APB_M + 1);
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->SYSCLK_Frequency / (PM->DIV_APB_P + 1);

  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency /
    (((ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SAH_TIME_WRITE_M) >> ADC_CONFIG_SAH_TIME_WRITE_S) + 31);
}
