#pragma once
#include "mik32.h"
#include "timer-def.h"

/*
  ### 64х битный таймер
  #### Регистры:
  + cnt() div() cmp()
  + счётчик делитель сравнение
  #### Управление:
  + start() stop() clear()
  #### Прерывания:
  + int_[en, dis, clr] () установить / отключить / двигать cmp
  #### Частоты:
  + osc() fq() частота счётчика/прерывания
*/
class T64 {
protected:
  u32 _inc;

public:

  // Управление

  ATTR_INLINE void start() { SCR1_TIMER->TIMER_CTRL = SCR1_TIMER_CTRL_ENABLE_M; }
  ATTR_INLINE void stop() { SCR1_TIMER->TIMER_CTRL = 0; }
  ATTR_INLINE void clear() { SCR1_TIMER->MTIME = 0; SCR1_TIMER->MTIMEH = 0; }

  // Регистры

  ATTR_INLINE u64 cnt()
  {
    u32 time_h1 = SCR1_TIMER->MTIMEH;
    u32 time_l = SCR1_TIMER->MTIME;
    u32 time_h = SCR1_TIMER->MTIMEH;
    if (time_h != time_h1) time_l = SCR1_TIMER->MTIME;
    return ((u64)time_h << 32) | time_l;
  }

  ATTR_INLINE void cnt(u64 val)
  {
    SCR1_TIMER->MTIME = 0;
    SCR1_TIMER->MTIMEH = ((uint32_t)(val >> 32));
    SCR1_TIMER->MTIME = (uint32_t)val;
  }

  ATTR_INLINE void cmp(u64 val)
  {
    SCR1_TIMER->MTIMECMP = 0xFFFFFFFF;
    SCR1_TIMER->MTIMECMPH = ((uint32_t)(val >> 32));
    SCR1_TIMER->MTIMECMP = (uint32_t)val;
  }

  ATTR_INLINE void div(u32 val) { SCR1_TIMER->TIMER_DIV = val; }

  // Прерывания

  ATTR_INLINE void int_clr()
  {
    if (read_csr(mip) & MIP_MTIP)
      cmp(SCR1_TIMER->TIMECMP + _inc);
  }
  ATTR_INLINE void int_en() { set_csr(mie, MIE_MTIE); }
  ATTR_INLINE void int_dis() { clear_csr(mie, MIE_MTIE); }

  // Установка частоты

  void osc(u32 val)
  {
    u32 _div = F_CPU / val;
    if (_div) _div--;
    div(_div);
  }

  void fq(u32 val)
  {
    u32 _inc = F_CPU / ((SCR1_TIMER->TIMER_DIV + 1) * val);
    if (_inc == 0) _inc = 1;
  }
};
