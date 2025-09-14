#pragma once
#include "mik32.h"
#include "timer-def.h"

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
  + cnt() top() div() ocr([ch]) icr([ch])
  + счётчик предел делитель сравнение захват
  #### Управление:
  + start() stop() clear()
  + en(ch) dis(ch) включить/выключить канал
  + inv(on/off, ch) инвертировать канал
  #### Настройка:
  + direct(TIM_MODE::[DIR, REV, BI]) направление счёта
  + mode[(TIM_MODE::[CMP, PVM, CAP], ch)] выбрать режим работы канала
  #### Прерывания:
  + int_[ovf, udf, cmp, cap)][(ch, on/off)] установить/снять прерывание
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
  // u32 f_cycle;

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
  ATTR_INLINE void direct(uc32 mod)
  {
    u32 control = T_32(N)->CONTROL & ~TIMER32_CONTROL_MODE_M;
    switch (mod) {
      case TIM_MODE::DIR: control |= TIMER32_CH_CNTRL_MODE_COMPARE_M; break;
      case TIM_MODE::REV: control |= TIMER32_CH_CNTRL_MODE_PWM_M; break;
      case TIM_MODE::BI:  control |= TIMER32_CH_CNTRL_MODE_CAPTURE_M; break;
    }
    T_32(N)->CONTROL = control;
  }

  // Регистры

  ATTR_INLINE u32 cnt() { return T_32(N)->VALUE; }
  ATTR_INLINE void cnt(u32 val) { T_32(N)->VALUE = val; }
  ATTR_INLINE void div(u32 val) { T_32(N)->PRESCALER = val; }
  ATTR_INLINE void top(u32 val) { T_32(N)->TOP = val; }
  ATTR_INLINE void ocr(u32 val, uc32 ch = 0) { T_32(N)->CHANNELS[ch].OCR = val; }
  ATTR_INLINE u32 icr(uc32 ch = 0) { return T_32(N)->CHANNELS[ch].ICR; }

  // Каналы

  ATTR_INLINE void en(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_ENABLE_M; }
  ATTR_INLINE void dis(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL &= ~TIMER32_CH_CNTRL_ENABLE_M; }
  ATTR_INLINE void inv(const bool mod = true, uc32 ch = 0)
  {
    if (mod) T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_INVERTED_PWM_M | TIMER32_CH_CNTRL_CAPTURE_NEG_M;
    else T_32(N)->CHANNELS[ch].CNTRL &= ~(TIMER32_CH_CNTRL_INVERTED_PWM_M | TIMER32_CH_CNTRL_CAPTURE_NEG_M);
  }
  ATTR_INLINE void mode(TIM_MODE mod, uc32 ch = 0)
  {
    u32 cntrl = T_32(N)->CHANNELS[ch].CNTRL & (TIMER32_CH_CNTRL_ENABLE_M | TIMER32_CH_CNTRL_NOISE_M);
    switch (mod) {
      case TIM_MODE::CMP: cntrl |= TIMER32_CH_CNTRL_MODE_COMPARE_M; break;
      case TIM_MODE::PWM: cntrl |= TIMER32_CH_CNTRL_MODE_PWM_M; break;
      case TIM_MODE::CAP: cntrl |= TIMER32_CH_CNTRL_MODE_CAPTURE_M; break;
    }
    T_32(N)->CHANNELS[ch].CNTRL = cntrl;
  }

  // Прерывания

  ATTR_INLINE void int_ovf(const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_OVERFLOW_M :
      T_32(N)->INT_MASK &= ~TIMER32_INT_OVERFLOW_M;
  }
  ATTR_INLINE void int_udf(const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_UNDERFLOW_M :
      T_32(N)->INT_MASK &= ~TIMER32_INT_UNDERFLOW_M;
  }
  ATTR_INLINE void int_cmp(uc32 ch, const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_OC_M(ch) :
      T_32(N)->INT_MASK &= ~TIMER32_INT_OC_M(ch);
  }
  ATTR_INLINE void int_cap(uc32 ch, const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_IC_M(ch) :
      T_32(N)->INT_MASK &= ~TIMER32_INT_IC_M(ch);
  }

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
    // f_get();
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
    // f_cycle = f_clk / (T_32(N)->TOP + 1);
  }
};
