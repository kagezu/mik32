#pragma once
#include <mik32_memory_map.h>
#include <power_manager.h>
#include <analog_reg.h>

class ADC {
public:

  void init(uint8_t chanel, uint16_t delay = 0x0C)
  {
    PM->CLK_APB_P_SET = PM_CLOCK_APB_P_ANALOG_REGS_M; // Включить тактирование
    ANALOG_REG->ADC_CONFIG =
      (delay << ADC_CONFIG_SAH_TIME_WRITE_S)// Задержка
      | (chanel << ADC_CONFIG_SEL_S)        // Выборать канал
      | ADC_CONFIG_RN_M                     // Сброс
      | ADC_CONFIG_EN_M;                    // Включить

    single(); // Выполнить измерение
    wait();   // Ждать завершения
  }

  void single() { ANALOG_REG->ADC_SINGLE = 1; }
  void start() { ANALOG_REG->ADC_CONTINUOUS = 1; }
  void stop() { ANALOG_REG->ADC_CONTINUOUS = 0; }
  void wait() { while (!(ANALOG_REG->ADC_VALID)); }

  uint16_t value() { return ANALOG_REG->ADC_VALUE; }
};
