#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <timer16.h>

/*
  ### Настройка таймера:

  + T[16, 32]_[0, 1, 2]_POWER_[ON, OFF] Тактирование
  + T[16, 32]_[0, 1, 2]_[ON, OFF] Запуск / останов таймера
  + T16_[0, 1, 2]_FQ(div, fq) Установка пред-делителя и частоты
  + T32_[0, 1, 2]_FQ(fq) Установка частоты

  + T0_[ NORMAL, CTC, FAST_PWM [ _CUSTOM ], PHASE_PWM [ _CUSTOM ] ] Режимы таймера: счётчик, шим
  + T0_OC0[ A, B ]_[ OFF, ON, PVM[ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR0[ A, B ] = 0...255; Регистры управлением делителем / заполненностью шим
  + T0_[ OVF, COMPA, COMPB ] _ [ ON, OFF ] Установка прерываний
*/
#define T


#define T16_0_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M;
#define T16_1_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_1_M;
#define T16_2_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_2_M;
#define T16_0_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_0_M;
#define T16_1_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_1_M;
#define T16_2_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_2_M;

#define T16_0_ON                  TIMER16_0->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M;
#define T16_1_ON                  TIMER16_1->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M;
#define T16_2_ON                  TIMER16_2->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M;
#define T16_0_OFF                 TIMER16_0->CR = 0;
#define T16_1_OFF                 TIMER16_1->CR = 0;
#define T16_2_OFF                 TIMER16_2->CR = 0;

#define T16_0_FQ(div, fq)         TIMER16_0->CR = TIMER16_0->CFGR = div << TIMER16_PRESCALER_S; \
                                  TIMER16_0->ARR = OSC_SYSTEM_VALUE / (1 << div) / fq - 1;
#define T16_1_FQ(div, fq)         TIMER16_1->CR = TIMER16_1->CFGR = div << TIMER16_PRESCALER_S; \
                                  TIMER16_1->ARR = OSC_SYSTEM_VALUE / (1 << div) / fq - 1;
#define T16_2_FQ(div, fq)         TIMER16_2->CR = TIMER16_2->CFGR = div << TIMER16_PRESCALER_S; \
                                  TIMER16_1->ARR = OSC_SYSTEM_VALUE / (1 << div) / fq - 1;





#define T32_0_POWER_ON            PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M;
#define T32_1_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M;
#define T32_2_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M;
#define T32_0_POWER_OFF           PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_TIMER32_0_M;
#define T32_1_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_1_M;
#define T32_2_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_2_M;

#define T32_0_ON                  TIMER32_0->ENABLE = TIMER32_ENABLE_TIM_EN_M;
#define T32_1_ON                  TIMER32_1->ENABLE = TIMER32_ENABLE_TIM_EN_M;
#define T32_2_ON                  TIMER32_2->ENABLE = TIMER32_ENABLE_TIM_EN_M;
#define T32_0_OFF                 TIMER32_0->ENABLE = TIMER32_ENABLE_TIM_CLR_M;
#define T32_1_OFF                 TIMER32_1->ENABLE = TIMER32_ENABLE_TIM_CLR_M;
#define T32_2_OFF                 TIMER32_2->ENABLE = TIMER32_ENABLE_TIM_CLR_M;

#define T32_0_FQ(fq)              TIMER32_0->TOP = OSC_SYSTEM_VALUE / fq - 1;
#define T32_1_FQ(fq)              TIMER32_1->TOP = OSC_SYSTEM_VALUE / fq - 1;
#define T32_2_FQ(fq)              TIMER32_2->TOP = OSC_SYSTEM_VALUE / fq - 1;

#define T32_0_TOP(top)            TIMER32_0->TOP = top;
#define T32_1_TOP(top)            TIMER32_1->TOP = top;
#define T32_2_TOP(top)            TIMER32_2->TOP = top;
