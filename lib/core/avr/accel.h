#pragma once

#define AVR_ASM

#define div10_16bit(div, mod, tmp)        \
  asm volatile(                           \
      " ldi %2,51     \n\t"               \
      " mul %A0,%2    \n\t"               \
      " clr %A0       \n\t"               \
      " add r0,%2     \n\t"               \
      " adc %A0,r1    \n\t"               \
      " mov %1,r0     \n\t"               \
      " mul %B0,%2    \n\t"               \
      " clr %B0       \n\t"               \
      " add %A0,r0    \n\t"               \
      " adc %B0,r1    \n\t"               \
      " clr r1        \n\t"               \
      " add %1,%A0    \n\t"               \
      " adc %A0,%B0   \n\t"               \
      " adc %B0,r1    \n\t"               \
      " add %1,%B0    \n\t"               \
      " adc %A0,r1    \n\t"               \
      " adc %B0,r1    \n\t"               \
      " lsr %B0       \n\t"               \
      " ror %A0       \n\t"               \
      " ror %1        \n\t"               \
      " ldi %2,10     \n\t"               \
      " mul %1,%2     \n\t"               \
      " mov %1,r1     \n\t"               \
      " clr r1        \n\t"               \
      : "+r"(div), "=d"(mod), "=d"(tmp)   \
      :                                   \
      : "r0")

#define div10_32bit(div, mod, tmp)        \
  asm volatile(                           \
      " ldi %2,51     \n\t"               \
      " mul %A0,%2    \n\t"               \
      " clr %A0       \n\t"               \
      " add r0,%2     \n\t"               \
      " adc %A0,r1    \n\t"               \
      " mov %1,r0     \n\t"               \
      " mul %B0,%2    \n\t"               \
      " clr %B0       \n\t"               \
      " add %A0,r0    \n\t"               \
      " adc %B0,r1    \n\t"               \
      " mul %C0,%2    \n\t"               \
      " clr %C0       \n\t"               \
      " add %B0,r0    \n\t"               \
      " adc %C0,r1    \n\t"               \
      " mul %D0,%2    \n\t"               \
      " clr %D0       \n\t"               \
      " add %C0,r0    \n\t"               \
      " adc %D0,r1    \n\t"               \
      " clr r1        \n\t"               \
      " add %1,%A0    \n\t"               \
      " adc %A0,%B0   \n\t"               \
      " adc %B0,%C0   \n\t"               \
      " adc %C0,%D0   \n\t"               \
      " adc %D0,r1    \n\t"               \
      " add %1,%B0    \n\t"               \
      " adc %A0,%C0   \n\t"               \
      " adc %B0,%D0   \n\t"               \
      " adc %C0,r1    \n\t"               \
      " adc %D0,r1    \n\t"               \
      " add %1,%D0    \n\t"               \
      " adc %A0,r1    \n\t"               \
      " adc %B0,r1    \n\t"               \
      " adc %C0,r1    \n\t"               \
      " adc %D0,r1    \n\t"               \
      " lsr %D0       \n\t"               \
      " ror %C0       \n\t"               \
      " ror %B0       \n\t"               \
      " ror %A0       \n\t"               \
      " ror %1        \n\t"               \
      " ldi %2,10     \n\t"               \
      " mul %1,%2     \n\t"               \
      " mov %1,r1     \n\t"               \
      " clr r1        \n\t"               \
      : "+r"(div), "=d"(mod), "=d"(tmp)   \
      :                                   \
      : "r0")
