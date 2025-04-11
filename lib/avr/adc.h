#pragma once
#include "avr.h"
#include "macros/attribute.h"

#undef  ADC
#define ADCW  _SFR_MEM16(0x78)

#define ADC_AREF    0
#define ADC_AVCC    1
#define ADC_INT     3

#define ADC_CH_TEMP 8
#define ADC_CH_VBG  14
#define ADC_CH_GND  15

#define ADC_ADLAR   1

class ADC {
public:
  // Частота в КГц
  void init(uint8_t ch, uint16_t fq = 0xffff)
  {
    uint8_t div = 0;
    uint16_t max_fq = F_CPU / 26000;

    while (div < 0x07) {
      if (fq >= max_fq) break;
      max_fq >>= 1;
      div++;
    }

    DIDR0 |= _BV(ch);         // Отключить цифровой вход
    ADMUX = ch                // Выборать канал
      | (ADC_AVCC << REFS0);  // AREF = AVCC
    ADCSRA = (1 << ADEN)      // Включить
      | (1 << ADSC)           // Выполнить измерение
      | (div << ADPS0)        // Делитель
      | (ADC_ADLAR << ADLAR); // Выравнивание 8/10 bit
    wait();                   // Ждать завершения
  }

#if ADC_ADLAR
  uint8_t next(uint8_t ch)
  #else
  uint16_t next(uint8_t ch)
  #endif
  {
    single();
    wait();
    chanel(ch);
    return value();
  }

  GCC_INLINE void chanel(uint8_t ch) { ADMUX = ch | (ADC_AVCC << REFS0); }
  GCC_INLINE void single() { ADCSRA |= _BV(ADSC); }
  GCC_INLINE void start() { ADCSRA |= _BV(ADSC) | _BV(ADATE); }
  GCC_INLINE void stop() { ADCSRA &= ~_BV(ADATE); ADCSRA |= _BV(ADEN); }
  GCC_INLINE void wait() { while (!(ADCSRA & _BV(ADIF))); }

#if ADC_ADLAR
  uint8_t value() { return ADCH; }
#else
  uint16_t value() { return ADCW; }
#endif
};
