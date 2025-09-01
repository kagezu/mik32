#pragma once
#include "core.h"

#define WR_FORSED

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

  #define L_WR(x)   x(A, 8)
  #define L_RS(x)   x(A, 10)
  #define L_CS(x)   x(A, 11)
  #define L_RST(x)  x(D, 0)  // Не используется
  #define L_PORT(x) x(B, 0)

static inline void GPIO_ILI9486_16() {
  GPIO_InitTypeDef init;
  init.GPIO_Speed = GPIO_Speed_2MHz;
  init.GPIO_Mode = GPIO_Mode_Out_PP;
  init.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(L_PORT(PORT), &init);

  L_WR(OUT);
  L_RS(OUT);
  L_CS(OUT);
  L_WR(CLR);
  L_RS(CLR);
  L_CS(SET);

  #ifdef WR_FORSED
  constexpr uint32_t WR_PSC_DIV_TAB[] = {2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8};
  constexpr uint32_t L_WR_PSC_DIV = WR_PSC_DIV_TAB[F_CPU / 8000000 - 1];  // 18MHz max

  RCC->APB1PCENR |= RCC_TIM3EN;
  RCC->APB2PCENR |= RCC_TIM1EN;

  TIM3->PSC = L_WR_PSC_DIV - 1;  // Prescaler
  TIM3->CTLR2 = 0b001 << 4;      // Cчетчик отправляет сигнал CNT_EN
  TIM3->CTLR1 =
    TIM_DIR |                    // Обратное направление счётчика
    TIM_OPM |                    // Режим одиночного импульса
    0;

  TIM1->PSC = 0;  // Prescaler
  TIM1->ATRLR = L_WR_PSC_DIV - 1;
  TIM1->CH1CVR = L_WR_PSC_DIV >> 1;
  TIM1->SWEVGR = TIM_UG;  // Перезагружать
  TIM1->SMCFGR =
    0b101 |               //  Тригер запускает и останавливает счётчик
    (0b010 << 4);         //  Тригер TIM3
  TIM1->CHCTLR1 =
    // TIM_OC1M_0 |          // Режим сравнения - инверсия
    TIM_OC1M_1 |
    TIM_OC1M_2 |          // Режим PWM
    0;
  TIM1->BDTR = TIM_MOE;   // Включить TIM1 вывод
  TIM1->CCER = TIM_CC1E;  // Включить канал 1
  TIM1->CTLR1 = TIM_CEN;  // Включить TIM1

  #endif
}

#endif
