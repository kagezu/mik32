#include "mik32.h"

/* Для частоты: 32 MHz F_CPU */
__attribute__((noinline, section(".ram_text"))) void delay_us(uint32_t us)
{
  if (!us) return;
  int l = 4;
  asm volatile (
    "1:                 \n\t"
    "addi %0, %0, -1    \n\t" // 1 такт
    "bnez %0, 1b        \n\t" // 2 такта
    :"=r"(l) : "0" (l)
    );
  us--;
  us = us << 3; // 8 циклов на микросекунду
  if (!us) return; // 1 микросекунда

  asm volatile (
    "1:                 \n\t"
    "xor x0, x0, x0     \n\t" // 1 такт
    "addi %0, %0, -1    \n\t" // 1 такт
    "bne x0, %0, 1b     \n\t" // 2 такта
    : "=r" (us) : "0" (us)
    );
}

__attribute__((noinline, section(".ram_text"))) void delay_ms(uint32_t ms)
{
  while (ms--) delay_us(1000);
}

void sei()
{
  // set_csr(mstatus, MSTATUS_MIE);
  set_csr(mie, MIE_MEIE);
  // set_csr(mie, MIE_MTIE);
}

void cli()
{
  // clear_csr(mstatus, MSTATUS_MIE);
  clear_csr(mie, MIE_MEIE);
  // clear_csr(mie, MIE_MTIE);
}

char *sjoin(char *dst, const char *src)
{
  while ((*dst = *src)) { dst++; src++; }
  return dst;
}
