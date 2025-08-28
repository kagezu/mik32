#pragma once
#include "core.h"

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_WR       Тактирование / low -> high


#ifdef MIK32V2
// #define L_RD(x)   x ( 0, 8 )
// #define L_WR(x)   x ( 0, 9 )
// #define L_RS(x)   x ( 0, 10)

  #define L_WR(x)   x(1, 8)
  #define L_RS(x)   x(1, 9)
  #define L_CS(x)   x(1, 15)
  #define L_RST(x)  x(0, 0)  // Не используется
  #define L_PORT(x) x(0, 0)

  #define SEL_0(x)  x(1, 10)

static inline void GPIO_ILI9486_16() {
  // SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR);  // PORT 0.3 -> D9
  L_WR(GPIO);
  L_RS(GPIO);
  L_CS(GPIO);
  L_WR(OUT);
  L_RS(OUT);
  L_CS(OUT);
  L_WR(CLR);
  L_RS(CLR);
  L_CS(SET);
  PAD_CONFIG->PORT_0_CFG = 0;  // PORT 0 -> GPIO
  L_PORT(OUT) | 0xFFFF;        // PORT 0 -> OUT
}
#endif

#ifdef CH32V20x_D6

  #define L_WR(x)   x(A, 8 )
  #define L_RS(x)   x(A, 10)
  #define L_CS(x)   x(A, 11)
  #define L_RST(x)  x(D, 0 )  // Не используется
  #define L_PORT(x) x(B, 0 )

static inline void GPIO_ILI9486_16() {
  GPIO_InitTypeDef init;
  init.GPIO_Speed = GPIO_Speed_50MHz;
  init.GPIO_Mode = GPIO_Mode_Out_PP;
  init.GPIO_Pin = 0xFFFF;
  GPIO_Init(L_PORT(PORT), &init);

  L_WR(OUT);
  L_RS(OUT);
  L_CS(OUT);
  L_WR(CLR);
  L_RS(CLR);
  L_CS(SET);
}

#endif
