#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <analog_reg.h>
#include "macros/attribute.h"

class ADC {
public:

  void init(uint8_t ch, uint8_t delay = 0x20)
  {
    PM->CLK_APB_P_SET = PM_CLOCK_APB_P_ANALOG_REGS_M; // Включить тактирование
    ANALOG_REG->ADC_CONFIG =
      (delay << ADC_CONFIG_SAH_TIME_WRITE_S)// Задержка
      | (ch << ADC_CONFIG_SEL_S)            // Выборать канал
      | ADC_CONFIG_RN_M                  // Сброс
      | ADC_CONFIG_EN_M;                    // Включить

    single(); // Выполнить измерение
    wait();   // Ждать завершения
  }

  void chanel(uint8_t ch)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SEL_M)
      | (ch << ADC_CONFIG_SEL_S);
    single(); // Выполнить измерение
    wait();   // Ждать завершения
  }

  uint16_t next(uint8_t ch)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SEL_M)
      | (ch << ADC_CONFIG_SEL_S);
    single(); // Выполнить измерение
    wait();   // Ждать завершения
    return value();
  }


  GCC_INLINE void single() { ANALOG_REG->ADC_SINGLE = 1; }
  GCC_INLINE void start() { ANALOG_REG->ADC_CONTINUOUS = 1; }
  GCC_INLINE void stop() { ANALOG_REG->ADC_CONTINUOUS = 0; }
  GCC_INLINE void wait() { while (!(ANALOG_REG->ADC_VALID)); }

  GCC_INLINE uint16_t value() { return ANALOG_REG->ADC_VALUE; }
};
