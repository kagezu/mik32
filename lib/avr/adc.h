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
  void init(uint8_t ch, uint8_t div = 1)
  {
    DIDR0 |= _BV(ch);         // Отключить цифровой вход
    ADMUX = ch                // Выборать канал
      | (ADC_ADLAR << ADLAR)  // Выравнивание 8/10 bit
      | (ADC_AVCC << REFS0);  // AREF = AVCC
    ADCSRA = (1 << ADEN)      // Включить
      | (1 << ADSC)           // Выполнить измерение
      | (div << ADPS0);       // Делитель
    wait();                   // Ждать завершения
  }

#if ADC_ADLAR
  uint8_t
  #else
  uint16_t
  #endif
    next(uint8_t ch)
  {
    single();
    wait();
    chanel(ch);
    return value();
  }

  ATTR_INLINE void chanel(uint8_t ch) { ADMUX = ch | (ADC_ADLAR << ADLAR) | (ADC_AVCC << REFS0); }
  ATTR_INLINE void single() { ADCSRA |= _BV(ADSC); }
  ATTR_INLINE void start() { ADCSRA |= _BV(ADSC) | _BV(ADATE); }
  ATTR_INLINE void stop() { ADCSRA &= ~_BV(ADATE); ADCSRA |= _BV(ADEN); }
  ATTR_INLINE void wait() { while (!(ADCSRA & _BV(ADIF))); }

#if ADC_ADLAR
  uint8_t value() { return ADCH; }
#else
  uint16_t value() { return ADCW; }
#endif 

};
