#ifdef __AVR_ATmega128__
#pragma once
#include <Arduino.h>

/*
  Настройка таймера 0:
--
  + T0_STOP Отключение таймера
  + T0_DIV_[ 1, 8, 32, 64, 128, 256, 1024 ] Предварительный делитель (включает таймер)
  + T0_[ NORMAL, CTC, FAST_PWM, PHASE_PWM ] Режимы таймера: счётчик, шим
  + T0_OC0_[ OFF, ON, PVM [ _INV ] Режимы выводов: вкл, выкл, шим
  + OCR0 = 0...255; Регистры управлением делителем / заполненностью шим
  + T0_[ OVF, COMP ] _ [ ON, OFF ] Установка прерываний
*/
#define T0

/*
  Настройка таймера 1:
--
  + T1_[ STOP, EXT_FALL, EXT_RISE ] Отключение таймера, внешние тактирование
  + T1_DIV_[ 1, 8, 64, 256, 1024 ] Предварительный делитель (включает таймер)
  + T1_[ NORMAL, CTC, FAST_PWM_ [ 8, 9, 10, CUSTOM ], PHASE_PWM_ [8, 9, 10, CUSTOM ] ] Режимы таймера: счётчик, шим
  + T1_OC1[ A, B ] _ [ OFF, ON, PVM [ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR1[ A, B, C ] = 0...65535; Регистры управлением делителем / заполненностью шим
  + T1_[ OVF, CAPT, COMPA, COMPB, COMPC ] _ [ ON, OFF ] Установка прерываний
*/
#define T1

/*
  Настройка таймера 3:
--
  + T3_[ STOP, EXT_FALL, EXT_RISE ] Отключение таймера, внешние тактирование
  + T3_DIV_[ 1, 8, 64, 256, 1024 ] Предварительный делитель (включает таймер)
  + T3_[ NORMAL, CTC, FAST_PWM_ [ 8, 9, 10, CUSTOM ], PHASE_PWM_[ 8, 9, 10, CUSTOM ] ] Режимы таймера: счётчик, шим
  + T3_OC1[ A, B ] _ [ OFF, ON, PVM[ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR1[ A, B, C ] = 0...65535; Регистры управлением делителем / заполненностью шим
  + T3_[ OVF, CAPT, COMPA, COMPB, COMPC ] _ [ ON, OFF ] Установка прерываний
*/
#define T3

//------------------- Timer 0 ----------------------


// отключить тактирование
#define T0_STOP     TCCR0 &= ~B111;

// пред делитель N = [1, 8, 32, 64, 128, 256, 1024]
#define T0_DIV_1    TCCR0 = (TCCR0 & ~B111) | B001;
#define T0_DIV_8    TCCR0 = (TCCR0 & ~B111) | B010;
#define T0_DIV_32   TCCR0 = (TCCR0 & ~B111) | B011;
#define T0_DIV_64   TCCR0 = (TCCR0 & ~B111) | B100;
#define T0_DIV_128  TCCR0 = (TCCR0 & ~B111) | B101;
#define T0_DIV_256  TCCR0 = (TCCR0 & ~B111) | B110;
#define T0_DIV_1024 TCCR0 |= B111;


// циклично 8 бит, top = 0xFF
// f = clk / N*256
// Вывод OC0: toggle (f/2)
#define T0_NORMAL   TCCR0 &= ~(_BV(WGM01) | _BV(WGM00));

// CTC, top = OCR0
// f = clk / N*(1+OCR0)
// Вывод OC0: toggle (f/2)
#define T0_CTC      TCCR0 &= ~_BV(WGM00); TCCR0 |= _BV(WGM01);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Вывод OC0: заполнение шим: OCR0
#define T0_FAST_PWM TCCR0 |= _BV(WGM01) | _BV(WGM00);

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC0: заполнение шим: OCR0
#define T0_PHASE_PWM TCCR0 &= ~_BV(WGM01);TCCR0 |= _BV(WGM00);

/* Режим работы OC0 */

// Вывод OC0: off
#define T0_OC0A_OFF\
  TCCR0 &= ~(_BV(COM01) | _BV(COM00));\
  DDRB &= ~_BV(PB4);
// Вывод OC0: toggle (f/2)
#define T0_OC0A_ON \
  TCCR0 &= ~_BV(COM01);\
  TCCR0 |= _BV(COM00);\
  TCCR0 |= _BV(FOC0);\
  DDRB |= _BV(PB4);
// Вывод OC0: шим
#define T0_OC0A_PWM \
  TCCR0 &= ~_BV(COM00);\
  TCCR0 |= _BV(COM01);\
  TCCR0 &= ~_BV(FOC0);\
  DDRB |= _BV(PB4);
// Вывод OC0: инвертированный шим
#define T0_OC0A_PWM_INV \
  TCCR0 |= (_BV(COM01) | _BV(COM00));\
  TCCR0 &= ~_BV(FOC0);\
  DDRB |= _BV(PB4);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER0_COMP_vect)
#define T0_COMP_ON   TIMSK |= _BV(OCIE0);
#define T0_COMP_OFF  TIMSK &=~_BV(OCIE0);
// Использовать: ISR(TIMER0_OVF_vect)
#define T0_OVF_ON    TIMSK |= _BV(TOIE);
#define T0_OVF_OFF   TIMSK &=~_BV(TOIE);


//------------------- Timer 1 ----------------------


// отключить тактирование
#define T1_STOP       TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
// внешний источник - T1, по спаду
#define T1_EXT_FALL   TCCR1B = (TCCR1B & ~_BV(CS10)) | _BV(CS12) | _BV(CS11);
// внешний источник - T1, по фронту
#define T1_EXT_RISE   TCCR1B |= _BV(CS12) | _BV(CS11) | _BV(CS10);

// пред делитель N = [1, 8, 64, 256, 1024]
#define T1_DIV_1      TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
#define T1_DIV_8      TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS10))) | _BV(CS11);
#define T1_DIV_64     TCCR1B = (TCCR1B & ~_BV(CS12)) | _BV(CS11)   | _BV(CS10);
#define T1_DIV_256    TCCR1B = (TCCR1B & ~(_BV(CS11) | _BV(CS10))) | _BV(CS12);
#define T1_DIV_1024   TCCR1B = (TCCR1B & ~_BV(CS11)) | _BV(CS12)   | _BV(CS10);


// циклично 8 бит, top = 0xFFFF
// f = clk / N*65536
// Выводы OC1A/OC1B/OC1C: toggle (f/2)
#define T1_NORMAL \
  TCCR1A &= ~(_BV( WGM11) | _BV( WGM10));\
  TCCR1B &= ~(_BV( WGM13) | _BV( WGM12));

// CTC top = OCR1A [ OCR1AH:OCR1AL ]
// f = clk / N*(1+OCR1A)
// Выводы OC1A/OC1B/OC1C: toggle (f/2)
#define T1_CTC \
  TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1B |= _BV(WGM12);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_FAST_PWM_8 \
  TCCR1A &= ~_BV(WGM11);\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1A |= _BV(WGM10);\
  TCCR1B |= _BV(WGM12);

// шим 9 бит, top = 0x1FF
// f = clk / N*512
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_FAST_PWM_9 \
  TCCR1A &= ~_BV(WGM10);\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1A |= _BV(WGM11);\
  TCCR1B |= _BV(WGM12);

// шим 10 бит, top = 0x3FF
// f = clk / N*1024
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_FAST_PWM_10 \
  TCCR1A |= _BV(WGM10) | _BV(WGM11);\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1B |= _BV(WGM12);

// настраиваемый шим, top = ICR1 [ ICR1H:ICR1L ]
// f = clk / (1+ICR1)
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_FAST_PWM_CUSTOM \
  TCCR1B |= (_BV(WGM13) | _BV(WGM12));\
  TCCR1A &= ~_BV(WGM10);\
  TCCR1A |= _BV(WGM11);

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_PHASE_PWM_8 \
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));\
  TCCR1A &= ~_BV(WGM11);\
  TCCR1A |= _BV(WGM10);

// симметричный шим 9 бит, top = 0x1FF
// f = clk / N*1024
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_PHASE_PWM_9 \
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));\
  TCCR1A &= ~_BV(WGM10);\
  TCCR1A |= _BV(WGM11);

// симметричный шим 10 бит, top = 0x3FF
// f = clk / N*2048
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_PHASE_PWM_10 \
  TCCR1A |= (_BV(WGM11) | _BV(WGM10));\
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));

// симметричный настраиваемый, top = ICR1 [ ICR1H:ICR1L ]
// f = clk / 2*N*(1+ICR1)
// Выводы OC1A/OC1B/OC1C: заполнение шим: OCR1A/OCR1B/OCR1C
#define T1_PHASE_PWM_CUSTOM \
  TCCR1B &= ~_BV(WGM12);\
  TCCR1A &= ~_BV(WGM10);\
  TCCR1B |= _BV(WGM13);\
  TCCR1A |= _BV(WGM11);

/* Режим работы OC1A */

// Вывод OC1A: off
#define T1_OC1A_OFF \
TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0));\
  DDRB &= ~_BV(PB5);
// Вывод OC1A: toggle (f/2)
#define T1_OC1A_ON \
  TCCR1A &= ~_BV(COM1A1);\
  TCCR1A |= _BV(COM1A0);\
  TCCR1C |= _BV(FOC1A);\
  DDRB |= _BV(PB5);
// Вывод OC1A: шим
#define T1_OC1A_PWM \
  TCCR1A &= ~_BV(COM1A0);\
  TCCR1A |= _BV(COM1A1);\
  TCCR1C &= ~_BV(FOC1A);\
  DDRB |= _BV(PB5);
// Вывод OC1A: инвертированный шим
#define T1_OC1A_PWM_INV \
  TCCR1A |= (_BV(COM1A1) | _BV(COM1A0));\
  TCCR1C &= ~_BV(FOC1A);\
  DDRB |= _BV(PB5);

/* Режим работы OC1B */

// Вывод OC1B: off
#define T1_OC1B_OFF \
TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));\
  DDRB &= ~_BV(PB6);
// Вывод OC1B: toggle (f/2)
#define T1_OC1B_ON \
  TCCR1A &= ~_BV(COM1B1);\
  TCCR1A |= _BV(COM1B0);\
  TCCR1C |= _BV(FOC1B);\
  DDRB |= _BV(PB6);
// Вывод OC1B: шим
#define T1_OC1B_PWM \
  TCCR1A &= ~_BV(COM1B0);\
  TCCR1A |= _BV(COM1B1);\
  TCCR1C &=~_BV(FOC1B);\
  DDRB |= _BV(PB6);
// Вывод OC1B: инвертированный шим
#define T1_OC1B_PWM_INV \
  TCCR1A |= _BV(COM1B1) | _BV(COM1B0);\
  TCCR1C &=~_BV(FOC1B);\
  DDRB |= _BV(PB6);

/* Режим работы OC1C */

// Вывод OC1C: off
#define T1_OC1C_OFF \
TCCR1A &= ~(_BV(COM1C1) | _BV(COM1C0));\
  DDRB &= ~_BV(PB7);
// Вывод OC1C: toggle (f/2)
#define T1_OC1C_ON \
  TCCR1A &= ~_BV(COM1C1);\
  TCCR1A |= _BV(COM1C0);\
  TCCR1C |= _BV(FOC1C);\
  DDRB |= _BV(PB7);
// Вывод OC1C: шим
#define T1_OC1C_PWM \
  TCCR1A &= ~_BV(COM1C0);\
  TCCR1A |= _BV(COM1C1);\
  TCCR1C &=~_BV(FOC1C);\
  DDRB |= _BV(PB7);
// Вывод OC1C: инвертированный шим
#define T1_OC1C_PWM_INV \
  TCCR1A |= _BV(COM1C1) | _BV(COM1C0);\
  TCCR1C &=~_BV(FOC1C);\
  DDRB |= _BV(PB7);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER1_COMPA_vect)
#define T1_COMPA_ON   TIMSK |= _BV(OCIE1A);
#define T1_COMPA_OFF  TIMSK &=~ _BV(OCIE1A);
// Использовать: ISR(TIMER1_COMPB_vect)
#define T1_COMPB_ON   TIMSK |= _BV(OCIE1B);
#define T1_COMPB_OFF  TIMSK &=~ _BV(OCIE1B);
// Использовать: ISR(TIMER1_COMPC_vect)
#define T1_COMPC_ON   ETIMSK |= _BV(OCIE1C);
#define T1_COMPC_OFF  ETIMSK &=~ _BV(OCIE1C);
// Использовать: ISR(TIMER1_OVF_vect)
#define T1_OVF_ON     TIMSK |= _BV(TOIE1);
#define T1_OVF_OFF    TIMSK &=~ _BV(TOIE1);
// Использовать: ISR(TIMER1_CAPT_vect)
#define T1_CAPT_ON    TIMSK |= _BV(TICIE1);
#define T1_CAPT_OFF   TIMSK &=~ _BV(TICIE1);


//------------------- Timer 3 ----------------------


// отключить тактирование
#define T3_STOP       TCCR3B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
// внешний источник - T1, по спаду
#define T3_EXT_FALL   TCCR3B = (TCCR3B & ~_BV(CS00)) | _BV(CS02) | _BV(CS01);
// внешний источник - T1, по фронту
#define T3_EXT_RISE   TCCR3B |= _BV(CS02) | _BV(CS01) | _BV(CS00);

// пред делитель N = [1, 8, 64, 256, 1024]
#define T3_DIV_1      TCCR3B = (TCCR3B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00);
#define T3_DIV_8      TCCR3B = (TCCR3B & ~(_BV(CS02) | _BV(CS00))) | _BV(CS01);
#define T3_DIV_64     TCCR3B = (TCCR3B & ~_BV(CS02)) | _BV(CS01)   | _BV(CS00);
#define T3_DIV_256    TCCR3B = (TCCR3B & ~(_BV(CS01) | _BV(CS00))) | _BV(CS02);
#define T3_DIV_1024   TCCR3B = (TCCR3B & ~_BV(CS01)) | _BV(CS02)   | _BV(CS00);


// циклично 8 бит, top = 0xFFFF
// f = clk / N*65536
// Выводы OC3A/OC3B/OC3C: toggle (f/2)
#define T3_NORMAL \
  TCCR3A &= ~(_BV( WGM31) | _BV( WGM30));\
  TCCR3B &= ~(_BV( WGM33) | _BV( WGM32));

// CTC top = OCR3A [ OCR3AH:OCR3AL ]
// f = clk / N*(1+OCR3A)
// Выводы OC3A/OC3B/OC3C: toggle (f/2)
#define T3_CTC \
  TCCR3A &= ~(_BV(WGM31) | _BV(WGM30));\
  TCCR3B &= ~_BV(WGM33);\
  TCCR3B |= _BV(WGM32);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_FAST_PWM_8 \
  TCCR3A &= ~_BV(WGM31);\
  TCCR3B &= ~_BV(WGM33);\
  TCCR3A |= _BV(WGM30);\
  TCCR3B |= _BV(WGM32);

// шим 9 бит, top = 0x1FF
// f = clk / N*512
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_FAST_PWM_9 \
  TCCR3A &= ~_BV(WGM30);\
  TCCR3B &= ~_BV(WGM33);\
  TCCR3A |= _BV(WGM31);\
  TCCR3B |= _BV(WGM32);

// шим 10 бит, top = 0x3FF
// f = clk / N*1024
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_FAST_PWM_10 \
  TCCR3A |= _BV(WGM30) | _BV(WGM31);\
  TCCR3B &= ~_BV(WGM33);\
  TCCR3B |= _BV(WGM32);

// настраиваемый шим, top = ICR3 [ ICR3H:ICR3L ]
// f = clk / (1+ICR3)
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_FAST_PWM_CUSTOM \
  TCCR3B |= (_BV(WGM33) | _BV(WGM32));\
  TCCR3A &= ~_BV(WGM30);\
  TCCR3A |= _BV(WGM31);

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_PHASE_PWM_8 \
  TCCR3B &= ~(_BV(WGM33) | _BV(WGM32));\
  TCCR3A &= ~_BV(WGM31);\
  TCCR3A |= _BV(WGM30);

// симметричный шим 9 бит, top = 0x1FF
// f = clk / N*1024
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_PHASE_PWM_9 \
  TCCR3B &= ~(_BV(WGM33) | _BV(WGM32));\
  TCCR3A &= ~_BV(WGM30);\
  TCCR3A |= _BV(WGM31);

// симметричный шим 10 бит, top = 0x3FF
// f = clk / N*2048
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_PHASE_PWM_10 \
  TCCR3A |= (_BV(WGM31) | _BV(WGM30));\
  TCCR3B &= ~(_BV(WGM33) | _BV(WGM32));

// симметричный настраиваемый, top = ICR3 [ ICR3H:ICR3L ]
// f = clk / 2*N*(1+ICR3)
// Выводы OC3A/OC3B/OC3C: заполнение шим: OCR3A/OCR3B/OCR3C
#define T3_PHASE_PWM_CUSTOM \
  TCCR3B &= ~_BV(WGM32);\
  TCCR3A &= ~_BV(WGM30);\
  TCCR3B |= _BV(WGM33);\
  TCCR3A |= _BV(WGM31);

/* Режим работы OC3A */

// Вывод OC3A: off
#define T3_OC1A_OFF \
TCCR3A &= ~(_BV(COM3A1) | _BV(COM3A0));\
  DDRE &= ~_BV(PE3);
// Вывод OC3A: toggle (f/2)
#define T3_OC1A_ON \
  TCCR3A &= ~_BV(COM3A1);\
  TCCR3A |= _BV(COM3A0);\
  TCCR3C |= _BV(FOC3A);\
  DDRE |= _BV(PE3);
// Вывод OC3A: шим
#define T3_OC1A_PWM \
  TCCR3A &= ~_BV(COM3A0);\
  TCCR3A |= _BV(COM3A1);\
  TCCR3C &= ~_BV(FOC3A);\
  DDRE |= _BV(PE3);
// Вывод OC3A: инвертированный шим
#define T3_OC1A_PWM_INV \
  TCCR3A |= (_BV(COM3A1) | _BV(COM3A0));\
  TCCR3C &= ~_BV(FOC3A);\
  DDRE |= _BV(PE3);

/* Режим работы OC3B */

// Вывод OC3B: off
#define T3_OC1B_OFF \
TCCR3A &= ~(_BV(COM3B1) | _BV(COM3B0));\
  DDRE &= ~_BV(PE4);
// Вывод OC3B: toggle (f/2)
#define T3_OC1B_ON \
  TCCR3A &= ~_BV(COM3B1);\
  TCCR3A |= _BV(COM3B0);\
  TCCR3C |= _BV(FOC3B);\
  DDRE |= _BV(PE4);
// Вывод OC3B: шим
#define T3_OC1B_PWM \
  TCCR3A &= ~_BV(COM3B0);\
  TCCR3A |= _BV(COM3B1);\
  TCCR3C &=~_BV(FOC3B);\
  DDRE |= _BV(PE4);
// Вывод OC3B: инвертированный шим
#define T3_OC1B_PWM_INV \
  TCCR3A |= _BV(COM3B1) | _BV(COM3B0);\
  TCCR3C &=~_BV(FOC3B);\
  DDRE |= _BV(PE4);

/* Режим работы OC3C */

// Вывод OC3C: off
#define T3_OC1C_OFF \
TCCR3A &= ~(_BV(COM3C1) | _BV(COM3C0));\
  DDRE &= ~_BV(PE5);
// Вывод OC3C: toggle (f/2)
#define T3_OC1C_ON \
  TCCR3A &= ~_BV(COM3C1);\
  TCCR3A |= _BV(COM3C0);\
  TCCR3C |= _BV(FOC3C);\
  DDRE |= _BV(PE5);
// Вывод OC3C: шим
#define T3_OC1C_PWM \
  TCCR3A &= ~_BV(COM3C0);\
  TCCR3A |= _BV(COM3C1);\
  TCCR3C &=~_BV(FOC3C);\
  DDRE |= _BV(PE5);
// Вывод OC3C: инвертированный шим
#define T3_OC1C_PWM_INV \
  TCCR3A |= _BV(COM3C1) | _BV(COM3C0);\
  TCCR3C &=~_BV(FOC3C);\
  DDRE |= _BV(PE5);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER3_COMPA_vect)
#define T3_COMPA_ON   ETIMSK |= _BV(OCIE3A);
#define T3_COMPA_OFF  ETIMSK &=~ _BV(OCIE3A);
// Использовать: ISR(TIMER3_COMPB_vect)
#define T3_COMPB_ON   ETIMSK |= _BV(OCIE3B);
#define T3_COMPB_OFF  ETIMSK &=~ _BV(OCIE3B);
// Использовать: ISR(TIMER3_COMPC_vect)
#define T3_COMPC_ON   ETIMSK |= _BV(OCIE3C);
#define T3_COMPC_OFF  ETIMSK &=~ _BV(OCIE3C);
// Использовать: ISR(TIMER3_OVF_vect)
#define T3_OVF_ON     ETIMSK |= _BV(TOIE3);
#define T3_OVF_OFF    ETIMSK &=~ _BV(TOIE3);
// Использовать: ISR(TIMER3_CAPT_vect)
#define T3_CAPT_ON    ETIMSK |= _BV(TICIE3);
#define T3_CAPT_OFF   ETIMSK &=~ _BV(TICIE3);

#endif
