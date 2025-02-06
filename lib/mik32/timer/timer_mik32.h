#pragma once

#include <mik32_memory_map.h>
#include <power_manager.h>
#include <timer16.h>

#ifdef MIK32V2




// namespace Timer {

//   // Установка частоты для таймера 0
//   void timer0(uint16_t freq)
//   {
//     if (freq < (F_CPU_DIV256 >> 8)) { T0_DIV_1024; OCR0A = ((F_CPU_DIV256 >> 2) / freq - 1); return; }
//     if (freq < (F_CPU_DIV256 >> 6)) { T0_DIV_256; OCR0A = (F_CPU_DIV256 / freq - 1); return; }
//     if (freq < (F_CPU_DIV256 >> 3)) { T0_DIV_64; OCR0A = ((F_CPU / 0x40) / freq - 1); return; }
//     if (freq < (F_CPU_DIV256 >> 0)) { T0_DIV_8; OCR0A = ((F_CPU / 8) / freq - 1); return; }
//     T0_DIV_1; OCR0A = F_CPU / freq - 1; return;
//   }





// }

#endif
