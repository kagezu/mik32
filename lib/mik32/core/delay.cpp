#include <inttypes.h> 

/* Поддерживаемые частоты 32 MHz F_CPU */
void delay_us(uint32_t us)
{
  if (!us) return;
  int l = 6;
  asm volatile (
    "1:                 \n\t"
    "addi %0, %0, -1    \n\t" // 1 такт
    "bnez %0, 1b        \n\t" // 2 такта
    :"=r"(l) : "0" (l)
    );
  us--;
  if (!us) return; // 0,9 микросекунды
  //  при выполнении из флэш, пропускается каждый ~76й такт
  // при 10,5 циклов на микросекунду, опережение 0,25%
  // при выполнении из озу, опережение 1,4%
  us = (us << 3) + (us << 1) + (us >> 1); // x10,5
  us -= 10; // коррекция
  if (!us) return; // 2 микросекунды
  asm volatile (
    "1:                 \n\t"
    "addi %0, %0, -1    \n\t" // 1 такт
    "bnez %0, 1b        \n\t" // 2 такта
    : "=r" (us) : "0" (us)
    );
}

void delay_ms(uint32_t ms)
{
  while (ms--) delay_us(1000);
}
