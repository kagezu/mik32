#pragma once
#include "mik32.h"

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
  + cnt() top() div() osr()
  + счётчик предел делитель сравнение
  #### Управление:
  + start() stop() single() clear()
  #### Настройка:
  + [oc, pwm] () [выбрать режим работы канала]
  #### Прерывания:
  + int_[ovf, udf, oc, ic)][(ch)] [установить прерывание]
  + [переполнение, обнуление, сравнение, захват]
  + int_[en, dis, clr] () установить / отключить / сброс EPIC
  #### Частоты:
  + osc() fq() частота счётчика/обновления
*/
template<const auto N>
class T16 {
protected:
  u32 f_in;
  u32 f_clk;
  u32 f_cycle;

public:
  T16()
  {
    switch (N) {
      case 0: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_0_M; break;
      case 1: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_1_M; break;
      case 2: PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER16_2_M; break;
    }
    T_16(N)->CR = TIMER16_CR_ENABLE_M;
    f_in = F_CPU / (PM->DIV_APB_P + 1);
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
  ATTR_INLINE void clear() { T_16(N)->CR = TIMER16_CR_SNGSTRT_M | TIMER16_CR_ENABLE_M; }// CFGR TIMEOUT
  ATTR_INLINE void direct(uc16 val) {}

  // Регистры

  ATTR_INLINE u16 cnt() { return T_16(N)->CNT; }
  ATTR_INLINE void cnt(u16 val) {}
  ATTR_INLINE void div(u16 val) { T_16(N)->CFGR = (T_16(N)->CFGR & ~TIMER16_CFGR_PRESC_M) | (val << TIMER16_CFGR_PRESC_S); }
  ATTR_INLINE void top(u16 val) { T_16(N)->ARR = val; }
  ATTR_INLINE void osr(u16 val) { T_16(N)->CMP = val; }
  ATTR_INLINE u16 isr() { return 0; }

  // Режимы (пред-настройки)

  ATTR_INLINE void oc() { T_16(N)->CFGR; }
  ATTR_INLINE void ic() {}
  ATTR_INLINE void encoder(u32 arg = 0b11) { T_16(N)->CFGR = TIMER16_CFGR_ENC_M | (arg << TIMER16_CFGR_CKPOL_S); }
  ATTR_INLINE void pwm() { T_16(N)->CFGR = 0; }

  // Прерывания

  ATTR_INLINE void int_ovf() { T_16(N)->INT_MASK ^= TIMER32_INT_OVERFLOW_M; }
  ATTR_INLINE void int_udf() { T_16(N)->INT_MASK ^= TIMER32_INT_UNDERFLOW_M; }
  ATTR_INLINE void int_oc(uc32 ch = 0) { T_16(N)->INT_MASK ^= TIMER32_INT_OC_M(ch); }
  ATTR_INLINE void int_ic(uc32 ch = 0) { T_16(N)->INT_MASK ^= TIMER32_INT_IC_M(ch); }

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

};
