#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <timer16.h>

/*
  Настройка таймера 0:
--
  + T0_[ STOP, EXT_FALL, EXT_RISE ] Отключение таймера, внешние тактирование
  + T0_[ NORMAL, CTC, FAST_PWM [ _CUSTOM ], PHASE_PWM [ _CUSTOM ] ] Режимы таймера: счётчик, шим
  + T0_OC0[ A, B ]_[ OFF, ON, PVM[ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR0[ A, B ] = 0...255; Регистры управлением делителем / заполненностью шим
  + T0_[ OVF, COMPA, COMPB ] _ [ ON, OFF ] Установка прерываний
*/
#define T0


namespace Timer {

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

  uint32_t get_timer0()
  {
    return TIMER16_0->CNT;
  }

}
