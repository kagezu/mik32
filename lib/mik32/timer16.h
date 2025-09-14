#pragma once
#include "mik32.h"
#include "timer-def.h"

constexpr static TIMER16_TypeDef *T_16(u32 N)
{
  switch (N) {
    case 0: return TIMER16_0;
    case 1: return TIMER16_1;
    case 2: return TIMER16_2;
    default: return 0;
  }
}

/*
  ### 16х битный таймер
  #### Регистры:
  + cnt() top() div() ocr()
  + счётчик предел делитель сравнение
  #### Управление:
  + start() stop() single() clear()
  #### Настройка:
  + mode(TIM_MODE::[CMP, PVM, ENC]) выбрать режим работы канала
  #### Прерывания:
  + int_[ovf, udf, cmp)][(ch)] установить прерывание
  + [переполнение, обнуление, сравнение]
  + int_[en, dis, clr] () установить / отключить / сброс EPIC
  #### Частоты:
  + osc() fq() частота счётчика/обновления
*/
template<const auto N>
class T16 {
protected:
  u32 f_in;
  u32 f_clk;

public:
  T16()
  {
    switch (N) {
      case 0: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M; break;
      case 1: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_1_M; break;
      case 2: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_2_M; break;
    }
    T_16(N)->CR = TIMER16_CR_ENABLE_M;
    f_get();
  }
  ~T16()
  {
    switch (N) {
      case 0: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_0_M; break;
      case 1: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_1_M; break;
      case 2: PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_TIMER16_2_M; break;
    }
  }

  // Управление

  ATTR_INLINE void start() { T_16(N)->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M; }
  ATTR_INLINE void single() { T_16(N)->CR = TIMER16_CR_SNGSTRT_M | TIMER16_CR_ENABLE_M; }
  ATTR_INLINE void stop() { T_16(N)->CR = 0; T_16(N)->CR = TIMER16_CR_ENABLE_M; }
  ATTR_INLINE void clear() { T_16(N)->CR = TIMER16_CR_CNTSTRT_M | TIMER16_CR_ENABLE_M; } // mode(CNT)

  // Регистры

  ATTR_INLINE u16 cnt() { return T_16(N)->CNT; }
  // ATTR_INLINE void cnt(u16 val) { return 0; }
  ATTR_INLINE void div(u16 val) { T_16(N)->CFGR = (T_16(N)->CFGR & ~TIMER16_CFGR_PRESC_M) | ((val & TIMER16_CFGR_PRESC_M) << TIMER16_CFGR_PRESC_S); }
  ATTR_INLINE void top(u16 val) { T_16(N)->ARR = val; }
  ATTR_INLINE void ocr(u16 val) { T_16(N)->CMP = val; }
  // ATTR_INLINE u16 isr() { return 0; }

  // Режимы (пред-настройки)

  // TIM_MODE::[CMP, PWM, ENC]
  ATTR_INLINE void mode(TIM_MODE mod)
  {
    u32 cfgr = T_16(N)->CFGR & ~(TIMER16_CFGR_ENC_M | TIMER16_CFGR_TIMOUT_M);
    switch (mod) {
      case TIM_MODE::CMP: cfgr |= TIMER16_CFGR_TIMOUT_M;  break; // Сбрасывать CNT
      case TIM_MODE::PWM: break;
      case TIM_MODE::ENC: cfgr |= TIMER16_CFGR_ENC_M; break;
    }
    T_16(N)->CFGR = cfgr;
  }
  ATTR_INLINE void inv(const bool mod = true)
  {
    if (mod) T_16(N)->CFGR |= TIMER16_CFGR_WAVPOL_M;
    else T_16(N)->CFGR &= ~TIMER16_CFGR_WAVPOL_M;
  }

  // Прерывания

  ATTR_INLINE void int_ovf(const bool on = true)
  {
    T_32(N)->IER = on ?
      T_32(N)->IER |= TIMER16_IER_ARRMIE_M :
      T_32(N)->IER &= ~TIMER16_IER_ARRMIE_M;
  }
  ATTR_INLINE void int_udf(const bool on = true)
  {
    T_32(N)->IER = on ?
      T_32(N)->IER |= TIMER16_IER_ARRMIE_M :
      T_32(N)->IER &= ~TIMER16_IER_ARRMIE_M;
  }
  ATTR_INLINE void int_cmp(uc32 ch, const bool on = true)
  {
    T_32(N)->IER = on ?
      T_32(N)->IER |= TIMER16_IER_CMPMIE_M :
      T_32(N)->IER &= ~TIMER16_IER_CMPMIE_M;
  }

  ATTR_INLINE void int_clr() { T_16(N)->INT_CLEAR = -1; }
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
    u32 raw = f_in / val;
    u32 _div = 0;
    while (_div < 7 && raw > 1) { raw >> 1; _div++; }
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
    f_in = F_CPU / (PM->DIV_APB_P + 1);
    f_clk = f_in >> ((T_16(N)->CFGR >> TIMER16_PRESCALER_S) & 7);
  }
};
