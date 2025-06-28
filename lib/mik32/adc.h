#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <analog_reg.h>
#include "macros/attribute.h"

class ADC {
public:
  ADC(uint8_t ch, uint8_t time = 0x20) // по умолчанию 500 kHz
  {
    delay(time);
    chanel(ch);
  }

  // Задержка Fsps = Fcpu / ((delay & 0x3E) + 32) 
  ATTR_INLINE void delay(uint8_t time)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SAH_TIME_WRITE_M)
      | (time << ADC_CONFIG_SAH_TIME_WRITE_S);
  }

  ATTR_INLINE void chanel(uint8_t ch)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SEL_M)
      | (ch << ADC_CONFIG_SEL_S)  // Выбрать канал
      | ADC_CONFIG_RN_M           // Сброс
      | ADC_CONFIG_EN_M;          // Включить
    single();                     // Выполнить измерение
    wait();                       // Ждать завершения
  }

  ATTR_INLINE uint16_t next(uint8_t ch)
  {
    ANALOG_REG->ADC_CONFIG =
      (ANALOG_REG->ADC_CONFIG & ~ADC_CONFIG_SEL_M)
      | (ch << ADC_CONFIG_SEL_S); // Выбрать канал
    single();                     // Выполнить измерение
    wait();                       // Ждать завершения
    return value();
  }

  ATTR_INLINE void single() { ANALOG_REG->ADC_SINGLE = 1; }
  ATTR_INLINE void start() { ANALOG_REG->ADC_CONTINUOUS = 1; }
  ATTR_INLINE void stop() { ANALOG_REG->ADC_CONTINUOUS = 0; }
  ATTR_INLINE void wait() { while (!(ANALOG_REG->ADC_VALID)); }
  ATTR_INLINE uint16_t value() { return ANALOG_REG->ADC_VALUE; }
};
