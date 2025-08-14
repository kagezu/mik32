#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <analog_reg.h>
#include "macros/attribute.h"
#include "pinout.h"

class ADC {
public:
  // Число тактов должно быть не менее 0x1 и соответствовать
  // времени выборки не менее 350 нс. Для частоты APB равной 32 МГц
  // следует использовать 0xC. Значение можно рассчитывать
  // по следующей формуле: Fahb·350·10^-9
  constexpr static int SAH_MIN = 0;  // Минимальное время выборки
  constexpr static int SAH_MAX = 63; // Максимальное время выборки
  constexpr static int DEPTH = 12;   // Разрядность АЦП
  constexpr static int TIME = 32;    // Время преобразования
  constexpr static int AREF = 1300;  // Опорное напряжение в милливольтах

public:
  // template<const int ch>
  ATTR_INLINE static void init(const int ch)
  {
    switch (ch) {
      case 0: ADC0(ANALOG); break;
      case 1: ADC1(ANALOG); break;
      case 2: ADC2(ANALOG); break;
      case 3: ADC3(ANALOG); break;
      case 4: ADC4(ANALOG); break;
      case 5: ADC5(ANALOG); break;
      case 6: ADC6(ANALOG); break;
      case 7: ADC7(ANALOG); break;
    }
    chanel(ch);
    AREF(ANALOG);
  }


  // Преобразует число тактов в допустимое SAH_TIME
  ATTR_INLINE static int cycle(int32_t tick)
  {
    if (tick < ADC::SAH_MIN + ADC::TIME) tick = ADC::SAH_MIN + ADC::TIME;
    if (tick > ADC::SAH_MAX + ADC::TIME) tick = ADC::SAH_MAX + ADC::TIME;
    return tick;
  }

  // Задержка Fsps = Fcpu / (delay & 0x3E) 
  ATTR_INLINE static void delay(int time)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SAH_TIME_WRITE_M)
      | (((time - ADC::TIME) | 1) << ADC_CONFIG_SAH_TIME_WRITE_S);
  }

  ATTR_INLINE static void chanel(int ch)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SEL_M) |
      (ch << ADC_CONFIG_SEL_S) |  // Выбрать канал
      ADC_CONFIG_RN_M |           // Сброс
      ADC_CONFIG_EN_M;            // Включить
    single();                     // Выполнить измерение
    wait();                       // Ждать завершения
  }

  ATTR_INLINE static uint16_t next(int ch)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SEL_M)
      | (ch << ADC_CONFIG_SEL_S); // Выбрать канал
    single();                     // Выполнить измерение
    wait();                       // Ждать завершения
    return value();
  }

  ATTR_INLINE static void single() { ANALOG_REG->ADC_SINGLE = 1; }
  ATTR_INLINE static void start() { ANALOG_REG->ADC_CONTINUOUS = 1; }
  ATTR_INLINE static void stop() { ANALOG_REG->ADC_CONTINUOUS = 0; }
  ATTR_INLINE static void wait() { while (!(ANALOG_REG->ADC_VALID)); }
  ATTR_INLINE static int value() { return ANALOG_REG->ADC_VALUE; }
};
