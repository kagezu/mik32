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
  ### 32х битный таймер
  #### Регистры:
  + cnt() top() div() osr([ch]) isr([ch])
  + счётчик предел делитель сравнение захват
  #### Управление:
  + start() stop() single() clear()
  + en([ch]) dis([ch]) включить/выключить канал
  #### Настройка:
  + direct(TIM_MODE_DIR) направление счёта
  + [oc, ic, pwm][(ch)] [выбрать режим работы канала]
  + [ic, pwm]_inv(ch) [инвертировать вход/выход канала]
  #### Прерывания:
  + int_[ovf, udf, oc, ic)][(ch)] [установить прерывание]
  + [переполнение, обнуление, сравнение, захват]
  + int_[en, dis, clr] () установить / отключить / сброс EPIC
  #### Частоты:
  + osc() fq() частота счётчика/обновления
*/
template<const auto N>
class T32 {
protected:
  u32 f_in;
  u32 f_clk;
  u32 f_cycle;

public:
  T32()
  {
    switch (N) {
      case 0: PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M; break;
      case 1: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M; break;
      case 2: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_2_M; break;
    }
    T_32(N)->ENABLE = TIMER32_ENABLE_TIM_CLR_M; // Сброс и остановка счётчика
    T_32(N)->INT_MASK = 0;                      // Отключить прерывания
    T_32(N)->INT_CLEAR = -1;                    // Сбросить флаги прерываний

    f_get();
  }
  ~T32()
  {
    switch (N) {
      case 0: PM->CLK_APB_M_CLEAR = PM_CLOCK_APB_M_TIMER32_0_M; break;
      case 1: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_1_M; break;
      case 2: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER32_2_M; break;
    }
  }

  // Управление

  ATTR_INLINE void start() { T_32(N)->ENABLE = TIMER32_ENABLE_TIM_EN_M; }
  ATTR_INLINE void single() {}
  ATTR_INLINE void stop() { T_32(N)->ENABLE = 0; }
  ATTR_INLINE void clear() { T_32(N)->ENABLE |= TIMER32_ENABLE_TIM_CLR_M; }
  ATTR_INLINE void direct(uc32 val) { T_32(N)->CONTROL = val; }

  // Регистры

  ATTR_INLINE u32 cnt() { return T_32(N)->VALUE; }
  ATTR_INLINE void cnt(u32 val) { T_32(N)->VALUE = val; }
  ATTR_INLINE void div(u32 val) { T_32(N)->PRESCALER = val; }
  ATTR_INLINE void top(u32 val) { T_32(N)->TOP = val; }
  ATTR_INLINE void osr(u32 val, uc32 ch = 0) { T_32(N)->CHANNELS[ch].OSR = val; }
  ATTR_INLINE u32 isr(uc32 ch = 0) { return T_32(N)->CHANNELS[ch].ISR; }

  // Каналы

  ATTR_INLINE void en(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_ENABLE_M; }
  ATTR_INLINE void dis(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL &= ~TIMER32_CH_CNTRL_ENABLE_M; }
  ATTR_INLINE void oc(uc32 ch = 0) { T_32(N)->CHANNELS[ch].CNTRL = (T_32(N)->CHANNELS[ch].CNTRL & ~TIMER32_CH_CNTRL_MODE_M) | TIMER32_CH_CNTRL_MODE_COMPARE_M; }
  ATTR_INLINE void ic(uc32 ch = 0) { T_32(N)->CHANNELS[ch].CNTRL = (T_32(N)->CHANNELS[ch].CNTRL & ~TIMER32_CH_CNTRL_MODE_M) | TIMER32_CH_CNTRL_MODE_CAPTURE_M; }
  ATTR_INLINE void pwm(uc32 ch = 0) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_MODE_PWM_M; }
  ATTR_INLINE void ic_inv(uc32 ch = 0) { T_32(N)->CHANNELS[ch].CNTRL ^= TIMER32_CH_CNTRL_CAPTURE_NEG_M; }
  ATTR_INLINE void pwm_inv(uc32 ch = 0) { T_32(N)->CHANNELS[ch].CNTRL ^= TIMER32_CH_CNTRL_INVERTED_PWM_M; }

  // Прерывания

  ATTR_INLINE void int_ovf() { T_32(N)->INT_MASK ^= TIMER32_INT_OVERFLOW_M; }
  ATTR_INLINE void int_udf() { T_32(N)->INT_MASK ^= TIMER32_INT_UNDERFLOW_M; }
  ATTR_INLINE void int_oc(uc32 ch = 0) { T_32(N)->INT_MASK ^= TIMER32_INT_OC_M(ch); }
  ATTR_INLINE void int_ic(uc32 ch = 0) { T_32(N)->INT_MASK ^= TIMER32_INT_IC_M(ch); }

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

  // Установка частоты

  void osc(u32 val)
  {
    u32 _div = f_in / val;
    if (_div) _div--;
    div(_div);
    f_get();
  }

  void fq(u32 val)
  {
    u32 _top = f_clk / val;
    if (_top) _top--;
    top(_top);
    f_get();
  }

  void f_get()
  {
    switch (T_32(N)->CONTROL & TIMER32_CONTROL_CLOCK_M) {
      case TIMER32_CONTROL_CLOCK_PRESCALER_M:
        if (N) f_in = F_CPU / (PM->DIV_APB_P + 1);
        else f_in = F_CPU / (PM->DIV_APB_M + 1);
        break;

      case TIMER32_CONTROL_CLOCK_TIM1_M:
        if ((PM->TIMER_CFG >> (PM_TIMER_CFG_MUX_TIMER32_1_S * N)) & 0b01) f_in = F_CPU;
        else f_in = F_CPU / (PM->DIV_AHB + 1);
        break;

      case TIMER32_CONTROL_CLOCK_TX_PIN_M:
        f_in = 0;
        break;

      case TIMER32_CONTROL_CLOCK_TIM2_M:
        f_in = F_CPU;
        break;
    }

    f_clk = f_in / (T_32(N)->PRESCALER + 1);
    f_cycle = f_clk / (T_32(N)->TOP + 1);
  }
};
