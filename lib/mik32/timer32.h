#pragma once
#include "mik32.h"

constexpr static TIMER32_TypeDef *T_32(u32 N)
{
  switch (N) {
    case 0: return TIMER32_0;
    case 1: return TIMER32_1;
    case 2: return TIMER32_2;
    default: return 0;
  }
}

/*
  #### 32х битный таймер:

  + read() write() счётчик
  + enable([ch]) disable([ch])
  + clear()     сбросить таймер
  + direct()    направление счёта
  + top()       количество тактов в цикле
  + div()       делитель
  + direct()    направление отсчёта
  + osr(ch)     регистр сравнения
  + isr(ch)     регистр захвата
  + int_[ovf, udf, oc, ic] (ch) [установить прерывание]
  + [переполнение, обнуление, сравнение, захват]
  + int_[en, dis, clr] () установить / отключить / сброс EPIC
  + [oc, ic, pwm]_[inv] (ch) режим [инверсия входа / выхода]
*/
template<const auto N>
class T32 {
public:
  T32()
  {
    switch (N) {
      case 0: PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M; break;
      case 1: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M; break;
      case 2: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M; break;
    }
  }
  ~T32()
  {
    switch (N) {
      case 0: PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_TIMER32_0_M; break;
      case 1: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_1_M; break;
      case 2: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_2_M; break;
    }
  }

  // Интерфейс контекста

  ATTR_INLINE void init()
  {
    T_32(N)->ENABLE = TIMER32_ENABLE_TIM_CLR_M; // Сброс и остановка счётчика
    T_32(N)->INT_MASK = 0;                      // Отключить прерывания
    T_32(N)->INT_CLEAR = -1;                    // Сбросить флаги прерываний
  }
  ATTR_INLINE void enable() { T_32(N)->ENABLE = TIMER32_ENABLE_TIM_EN_M; }
  ATTR_INLINE void disable() { T_32(N)->ENABLE = 0; }
  ATTR_INLINE u32 count() { return T_32(N)->VALUE; }
  ATTR_INLINE void count(u32 value) { T_32(N)->VALUE = value; }

  // Интерфейс модуля

  ATTR_INLINE void clear() { T_32(N)->ENABLE |= TIMER32_ENABLE_TIM_CLR_M; }
  ATTR_INLINE void div(u32 value) { T_32(N)->PRESCALER = value; }
  ATTR_INLINE void top(u32 value) { T_32(N)->TOP = value; }
  ATTR_INLINE void osr(uc32 ch, u32 value) { T_32(N)->CHANNELS[ch].OSR = value; }
  ATTR_INLINE u32 isr(uc32 ch) { return T_32(N)->CHANNELS[ch].ISR; }
  ATTR_INLINE void direct(uc32 value) { T_32(N)->CONTROL = value; }

  ATTR_INLINE void enable(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_ENABLE_M; }
  ATTR_INLINE void disable(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL &= ~TIMER32_CH_CNTRL_ENABLE_M; }
  ATTR_INLINE void oc(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_MODE_COMPARE_M; }
  ATTR_INLINE void ic(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_MODE_CAPTURE_M; }
  ATTR_INLINE void pwm(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_MODE_PWM_M; }
  ATTR_INLINE void ic_inv(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_CAPTURE_NEG_M; }
  ATTR_INLINE void pwm_inv(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_INVERTED_PWM_M; }

  // Прерывания

  ATTR_INLINE void int_ovf() { T_32(N)->INT_MASK ^= TIMER32_INT_OVERFLOW_M; }
  ATTR_INLINE void int_udf() { T_32(N)->INT_MASK ^= TIMER32_INT_UNDERFLOW_M; }
  ATTR_INLINE void int_oc(uc32 ch) { T_32(N)->INT_MASK ^= TIMER32_INT_OC_M(ch); }
  ATTR_INLINE void int_ic(uc32 ch) { T_32(N)->INT_MASK ^= TIMER32_INT_IC_M(ch); }

  ATTR_INLINE void int_clr() { T_32(N)->INT_CLEAR = -1; }
  ATTR_INLINE void int_en()
  {
    switch (N) {
      case 0: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_0_S); break;
      case 1: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S); break;
      case 2: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_2_S); break;
    }
  }
  ATTR_INLINE void int_dis()
  {
    switch (N) {
      case 0: EPIC->MASK_LEVEL_CLEAR = EPIC_LINE_M(EPIC_LINE_TIMER32_0_S); break;
      case 1: EPIC->MASK_LEVEL_CLEAR = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S); break;
      case 2: EPIC->MASK_LEVEL_CLEAR = EPIC_LINE_M(EPIC_LINE_TIMER32_2_S); break;
    }
  }
};
