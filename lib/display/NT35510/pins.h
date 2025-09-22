#pragma once
#include "core.h"

//===================== Config =============================

// NT_CS       Выбор дисплея
// NT_RS       0 = Запись команды / 1 = Запись данных
// NT_RST      0 = Reset
// NT_WR       Тактирование / low -> high

#ifdef MIK32V2
#include "timer.h"

// Pin<1, 10> SEL_0;
Pin<1, 0> NT_WR;
Pin<1, 9> NT_RS;
Pin<1, 15> NT_CS;
Port<0, 0xFFFF> NT_PORT;

static inline void GPIO_NT35510()
{
  // SEL_0.init( GPO_2MHz);
  // SEL_0.clr();  // PORT 0.3 -> D9
  NT_PORT.init(GPO_2MHz);
  NT_WR.init(GPO_2MHz);
  NT_RS.init(GPO_2MHz);
  NT_CS.init(GPO_2MHz);
  NT_WR.clr();
  NT_RS.clr();
  NT_CS.set();

  // /*
  T32_2_PS;
  TIMER32_2->TOP = 1;
  TIMER32_2->CHANNELS[0].OCR = 1;
  T32_2_EN;
  T32_1_PS;
  T32_1_EN;
  // */
}
#endif

#ifdef CH32V20x_D6

Pin<PA, 8> NT_WR;
Pin<PA, 9> NT_RD;
Pin<PA, 10> NT_RS;
Pin<PA, 11> NT_CS;
Port<PB, 0xFFFF> NT_PORT;

static inline void GPIO_NT35510()
{
  NT_PORT.init(GPO_2MHz);
  NT_WR.init(GPO_10MHz);
  NT_RD.init(GPO_10MHz);
  NT_RS.init(GPO_10MHz);
  NT_CS.init(GPO_10MHz);
  NT_WR.clr();
  NT_RS.clr();
  NT_RD.set();
  NT_CS.set();
}

#endif
