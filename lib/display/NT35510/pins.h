#pragma once
#include "core.h"

// #define WR_FORSED

//===================== Config =============================

// NT_CS       Выбор дисплея
// NT_RS       0 = Запись команды / 1 = Запись данных
// NT_RST      0 = Reset
// NT_WR       Тактирование / low -> high


#ifdef MIK32V2
#include "timer.h"

#define NT_WR(x)   x(1, 0)
#define NT_RS(x)   x(1, 9)
#define NT_CS(x)   x(1, 15)
#define NT_RST(x)  x(0, 0)  // Не используется
#define NT_PORT(x) x(0, 0)

#define SEL_0(x)   x(1, 10)

static inline void GPIO_NT35510()
{
  SEL_0(GPIO);
  SEL_0(OUT);
  SEL_0(CLR);  // PORT 0.3 -> D9
  NT_WR(GPIO);
  NT_RS(GPIO);
  NT_CS(GPIO);
  NT_WR(OUT);
  NT_RS(OUT);
  NT_CS(OUT);
  NT_WR(CLR);
  NT_RS(CLR);
  NT_CS(SET);
  PAD_CONFIG->PORT_0_CFG = 0;  // PORT 0 -> GPIO
  GPIO_0->DIRECTION_OUT = 0xFFFF;
  GPIO_0->CLEAR = 0xFFFF;
  GPIO_0->SET = 0xFFFF;

  /*
  T32_2_PS;
  TIMER32_2->TOP = 1;
  TIMER32_2->CHANNELS[0].OCR = 1;
  T32_2_EN;
  T32_1_PS;
  T32_1_EN;
  */
}
#endif

#ifdef CH32V20x_D6

#define NT_WR(x)   x(A, 8)
#define NT_RD(x)   x(A, 9)
#define NT_RS(x)   x(A, 10)
#define NT_CS(x)   x(A, 11)
#define NT_RST(x)  x(D, 0)  // Не используется
#define NT_PORT(x) x(B, 0)

static inline void GPIO_NT35510()
{
  GPIO_InitTypeDef init;
  init.GPIO_Speed = GPIO_Speed_2MHz;
  init.GPIO_Mode = GPIO_Mode_Out_PP;
  init.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(L_PORT(PORT), &init);

  NT_WR(OUT);
  NT_RD(OUT);
  NT_RS(OUT);
  NT_CS(OUT);
  NT_WR(CLR);
  NT_RS(CLR);
  NT_RD(SET);
  NT_CS(SET);


#ifdef WR_FORSEDX
#define WR_PSC_FACTOR 3

  RCC->APB1PCENR |= RCC_TIM3EN;
  RCC->APB2PCENR |= RCC_TIM1EN;

  TIM3->PSC = (1 << WR_PSC_FACTOR) - 1;  // Prescaler
  TIM3->SMCFGR = TIM_SMS;                //  Тригер TIM1
  TIM3->CTLR1 =
    TIM_DIR |                            // Обратное направление счётчика
    TIM_OPM |                            // Режим одиночного импульса
    0;

  TIM1->PSC = 0;  // Prescaler
  TIM1->ATRLR = 1;
  TIM1->CH1CVR = 1;
  TIM1->SWEVGR = TIM_UG;  // Перезагружать
  TIM1->CHCTLR1 =
    // TIM_OC1M_0 |          // Режим сравнения - инверсия
    TIM_OC1M_1 |
    TIM_OC1M_2 |          // Режим PWM
    0;
  TIM1->BDTR = TIM_MOE;   // Включить TIM1 вывод
  TIM1->CCER = TIM_CC1E;  // Включить канал 1
  // TIM1->CTLR1 = TIM_CEN;    // Включить TIM1
  TIM1->CTLR2 = TIM_MMS_2;  // Cчетчик отправляет сигнал OC1REF
#endif


#ifdef WR_FORSED
#define NT_WR_PSC_DIV    2

  RCC->APB1PCENR |= RCC_TIM3EN;
  RCC->APB2PCENR |= RCC_TIM1EN;

  TIM3->PSC = NT_WR_PSC_DIV - 1;  // Prescaler
  TIM3->CTLR2 = 0b001 << 4;                       // Cчетчик отправляет сигнал CNT_EN
  TIM3->CTLR1 =
    TIM_DIR |                                     // Обратное направление счётчика
    TIM_OPM |                                     // Режим одиночного импульса
    0;

  TIM1->PSC = 0;  // Prescaler
  TIM1->ATRLR = NT_WR_PSC_DIV - 1;
  TIM1->CH1CVR = NT_WR_PSC_DIV >> 1;
  TIM1->SWEVGR = TIM_UG;  // Перезагружать
  TIM1->SMCFGR =
    0b101 |        //  Тригер запускает и останавливает счётчик
    (0b010 << 4);  //  Тригер TIM3
  TIM1->CHCTLR1 =
    // TIM_OC1M_0 |          // Режим сравнения - инверсия
    TIM_OC1M_1 |
    TIM_OC1M_2 |          // Режим PWM
    0;
  TIM1->BDTR = TIM_MOE;   // Включить TIM1 вывод
  TIM1->CCER = TIM_CC1E;  // Включить канал 1
  TIM1->CTLR1 = TIM_CEN;    // Включить TIM1

#endif
}

#endif
