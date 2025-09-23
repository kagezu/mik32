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

  // Регистры

  INLINE u32 CNT() { return T_32(N)->VALUE; }
  INLINE void CNT(u32 val) { T_32(N)->VALUE = val; }
  INLINE void PSC(u32 val) { T_32(N)->PRESCALER = val; }
  INLINE void TOP(u32 val) { T_32(N)->TOP = val; }
  INLINE void REP(u8 val) {}
  INLINE void OCR(u32 val, uc32 ch = 0) { T_32(N)->CHANNELS[ch].OCR = val; }
  INLINE u32 ICR(uc32 ch = 0) { return T_32(N)->CHANNELS[ch].ICR; }

  // Управление

  INLINE void cont() {}
  INLINE void single() {}
  INLINE void enable() { T_32(N)->ENABLE = TIMER32_ENABLE_TIM_EN_M; }
  INLINE void disable() { T_32(N)->ENABLE = 0; }
  INLINE void clear() { T_32(N)->ENABLE |= TIMER32_ENABLE_TIM_CLR_M; }

  // Функции таймера

  // Установить TOP OCR
  INLINE void compare(uc32 config, uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL = config | TIM_OVR; }
  INLINE void pwm(uc32 config, uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL = config | TIM_PWM; }






  INLINE void enable(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL |= TIMER32_CH_CNTRL_ENABLE_M; }
  INLINE void disable(uc32 ch) { T_32(N)->CHANNELS[ch].CNTRL &= ~TIMER32_CH_CNTRL_ENABLE_M; }

  INLINE void direct(uc32 mod = 0)
  {
    u32 control = T_32(N)->CONTROL & ~TIMER32_CONTROL_MODE_M;
    switch (mod) {
      case 0: control |= TIMER32_CONTROL_MODE_UP_M; break;
      case TIM_REV: control |= TIMER32_CONTROL_MODE_DOWN_M; break;
      case TIM_BI:  control |= TIMER32_CONTROL_MODE_BIDIR_M; break;
    }
    T_32(N)->CONTROL = control;
  }

  INLINE void forced(bool out, uc32 ch = 1) {}
  INLINE void encoder() {}
  INLINE void slave(uc32 trig) {}
  INLINE void master(uc32 mms) {}
  INLINE void capture(uc32 config, uc32 ch) {}
  INLINE void capture_pwm(uc32 config, uc32 ch) {}
  INLINE void init(uc32 mod) {}

  //

  INLINE void int_clear() { T_32(N)->INT_CLEAR = -1; }
  INLINE void ovf_wait() { while (!(T_32(N)->FLAG & TIMER32_INT_OVERFLOW_M)); }


  ///////////////////////////
    // Прерывания

  INLINE void int_ovf(const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_OVERFLOW_M :
      T_32(N)->INT_MASK &= ~TIMER32_INT_OVERFLOW_M;
  }
  INLINE void int_udf(const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_UNDERFLOW_M :
      T_32(N)->INT_MASK &= ~TIMER32_INT_UNDERFLOW_M;
  }
  INLINE void int_cmp(uc32 ch, const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_OC_M(ch) :
      T_32(N)->INT_MASK &= ~TIMER32_INT_OC_M(ch);
  }
  INLINE void int_cap(uc32 ch, const bool on = true)
  {
    T_32(N)->INT_MASK = on ?
      T_32(N)->INT_MASK |= TIMER32_INT_IC_M(ch) :
      T_32(N)->INT_MASK &= ~TIMER32_INT_IC_M(ch);
  }

  INLINE void int_en()
  {
    switch (N) {
      case 0: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_0_S); break;
      case 1: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S); break;
      case 2: EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_2_S); break;
    }
  }
  INLINE void int_dis()
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
    // div(_div);
    f_get();
  }

  void fq(u32 val)
  {
    u32 _top = f_clk / val;
    if (_top) _top--;
    // top(_top);
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
