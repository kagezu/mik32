#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <timer16.h>
#include <timer32.h>
#include <epic.h>

/*
  #### Доступные таймеры:

  + T16
  + T32
*/
#define T


#define T16_0_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M
#define T16_1_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_1_M
#define T16_2_POWER_ON            PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_2_M
#define T16_0_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_0_M
#define T16_1_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_1_M
#define T16_2_POWER_OFF           PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_2_M

#define T16_0_ON                  TIMER16_0->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M
#define T16_1_ON                  TIMER16_1->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M
#define T16_2_ON                  TIMER16_2->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M
#define T16_0_OFF                 TIMER16_0->CR = 0
#define T16_1_OFF                 TIMER16_1->CR = 0
#define T16_2_OFF                 TIMER16_2->CR = 0

#define T16_0_FQ(div, fq)         TIMER16_0->CR = TIMER16_0->CFGR = div << TIMER16_PRESCALER_S; \
                                  TIMER16_0->ARR = OSC_SYSTEM_VALUE / (1 << div) / fq - 1;
#define T16_1_FQ(div, fq)         TIMER16_1->CR = TIMER16_1->CFGR = div << TIMER16_PRESCALER_S; \
                                  TIMER16_1->ARR = OSC_SYSTEM_VALUE / (1 << div) / fq - 1;
#define T16_2_FQ(div, fq)         TIMER16_2->CR = TIMER16_2->CFGR = div << TIMER16_PRESCALER_S; \
                                  TIMER16_1->ARR = OSC_SYSTEM_VALUE / (1 << div) / fq - 1;



/*
  #### 32х битный таймер:

  + T32_[0, 1, 2] - счётчик
  + T32_[0, 1, 2]_PS Power Set включить питание
  + T32_[0, 1, 2]_PC Power Clear выключить питание
  + T32_[0, 1, 2]_E Enable начать отсчёт
  + T32_[0, 1, 2]_D Disable приостановить отсчёт
  + T32_[0, 1, 2]_C Clear сбросить таймер
  + T32_[0, 1, 2]_TOP(TOP + 1) количество тактов в цикле
  + T32_[0, 1, 2]_FQ( частота )
  + T32_[0, 1, 2]_IS Int Set установить линию прерываний
  + T32_[0, 1, 2]_OVF переполнение
  + T32_[0, 1, 2]_UDF обнуление
  + T32_[0, 1, 2]_IC Int Clear сброс маски прерываний
  + T32_[0, 1, 2]_FC Flag Clear сброс флагов прерываний
*/
#define T32

#define T32_0_PS          PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M
#define T32_1_PS          PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M
#define T32_2_PS          PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M
#define T32_0_PC          PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_TIMER32_0_M
#define T32_1_PC          PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_1_M
#define T32_2_PC          PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_2_M

#define T32_0_E           TIMER32_0->ENABLE = TIMER32_ENABLE_TIM_EN_M
#define T32_1_E           TIMER32_1->ENABLE = TIMER32_ENABLE_TIM_EN_M
#define T32_2_E           TIMER32_2->ENABLE = TIMER32_ENABLE_TIM_EN_M
#define T32_0_D           TIMER32_0->ENABLE = 0;
#define T32_1_D           TIMER32_1->ENABLE = 0;
#define T32_2_D           TIMER32_2->ENABLE = 0;
#define T32_0_C           TIMER32_0->ENABLE |= TIMER32_ENABLE_TIM_CLR_M
#define T32_1_C           TIMER32_1->ENABLE |= TIMER32_ENABLE_TIM_CLR_M
#define T32_2_C           TIMER32_2->ENABLE |= TIMER32_ENABLE_TIM_CLR_M

#define T32_0             TIMER32_0->VALUE
#define T32_1             TIMER32_1->VALUE
#define T32_2             TIMER32_2->VALUE

#define T32_0_FQ(fq)      TIMER32_0->TOP = OSC_SYSTEM_VALUE / fq - 1
#define T32_1_FQ(fq)      TIMER32_1->TOP = OSC_SYSTEM_VALUE / fq - 1
#define T32_2_FQ(fq)      TIMER32_2->TOP = OSC_SYSTEM_VALUE / fq - 1

#define T32_0_TOP(top)    TIMER32_0->TOP = (top - 1)
#define T32_1_TOP(top)    TIMER32_1->TOP = (top - 1)
#define T32_2_TOP(top)    TIMER32_2->TOP = (top - 1)

#define T32_0_OVF         TIMER32_0->INT_MASK = TIMER32_INT_OVERFLOW_M
#define T32_1_OVF         TIMER32_1->INT_MASK = TIMER32_INT_OVERFLOW_M
#define T32_2_OVF         TIMER32_2->INT_MASK = TIMER32_INT_OVERFLOW_M

#define T32_0_UDF         TIMER32_0->INT_MASK |= TIMER32_INT_UNDERFLOW_M
#define T32_1_UDF         TIMER32_1->INT_MASK |= TIMER32_INT_UNDERFLOW_M
#define T32_2_UDF         TIMER32_2->INT_MASK |= TIMER32_INT_UNDERFLOW_M

#define T32_0_IS          EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_0_S)
#define T32_1_IS          EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S)
#define T32_2_IS          EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_2_S)

#define T32_0_IC          TIMER32_0->INT_MASK = 0
#define T32_1_IC          TIMER32_1->INT_MASK = 0
#define T32_2_IC          TIMER32_2->INT_MASK = 0

#define T32_0_FC          TIMER32_0->INT_CLEAR = -1
#define T32_1_FC          TIMER32_1->INT_CLEAR = -1
#define T32_2_FC          TIMER32_2->INT_CLEAR = -1


#define EPIC_C            EPIC->CLEAR = -1



/*
// Установка частоты для таймера 0
void timer0(uint32_t fq)
{
  uint8_t presc_m = 0;
  uint16_t min_fq = OSC_SYSTEM_VALUE / 65535 + 1;

  while (presc_m < 0x07) {
    if (fq >= min_fq) break;
    min_fq <<= 1;
    presc_m++;
  }
  // Отключаем таймер, если был включён
  TIMER16_0->CR = 0;

  // Включение тактирования
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M;
  // Установить делитель частоты
  TIMER16_0->CFGR = presc_m << TIMER16_PRESCALER_S;
  // Значение сброса таймера
  TIMER16_0->ARR = OSC_SYSTEM_VALUE / (1 << presc_m) / fq - 1;
  // запустить таймер
  TIMER16_0->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M;
}
*/
