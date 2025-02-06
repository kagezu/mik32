#ifdef __AVR_ATmega328P__
#pragma once
#include <Arduino.h>

/*
  Настройка таймера 0:
--
  + T0_[ STOP, EXT_FALL, EXT_RISE ] Отключение таймера, внешние тактирование
  + T0_DIV_[ 1, 8, 64, 256, 1024 ] Предварительный делитель (включает таймер)
  + T0_[ NORMAL, CTC, FAST_PWM [ _CUSTOM ], PHASE_PWM [ _CUSTOM ] ] Режимы таймера: счётчик, шим
  + T0_OC0[ A, B ]_[ OFF, ON, PVM[ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR0[ A, B ] = 0...255; Регистры управлением делителем / заполненностью шим
  + T0_[ OVF, COMPA, COMPB ] _ [ ON, OFF ] Установка прерываний
*/
#define T0

/*
  Настройка таймера 1:
--
  + T1_[ STOP, EXT_FALL, EXT_RISE ] Отключение таймера, внешние тактирование
  + T1_DIV_[ 1, 8, 64, 256, 1024 ] Предварительный делитель (включает таймер)
  + T1_[ NORMAL, CTC, FAST_PWM_ [8, 9, 10, CUSTOM ], PHASE_PWM_ [ 8, 9, 10, CUSTOM ] ] Режимы таймера: счётчик, шим
  + T1_OC1[ A, B ]_[ OFF, ON, PVM [ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR1[ A, B ] = 0...65535; Регистры управлением делителем / заполненностью шим
  + T1_[ OVF, COMPA, COMPB ] _ [ ON, OFF ] Установка прерываний
*/
#define T1

/*
  Настройка таймера 2:
--
  + T2_[ STOP, EXT_FALL, EXT_RISE ] Отключение таймера, внешние тактирование
  + T2_DIV_[ 1, 8, 32, 64, 128, 256, 1024 ] Предварительный делитель (включает таймер)
  + T2_[ NORMAL, CTC, FAST_PWM [ _CUSTOM ], PHASE_PWM [ _CUSTOM ] ] Режимы таймера: счётчик, шим
  + T2_OC2[ A, B ]_[ OFF, ON, PVM [ _INV ] ] Режимы выводов: вкл, выкл, шим
  + OCR2[ A, B ] = 0...255; Регистры управлением делителем / заполненностью шим
  + T2_[ OVF, COMPA, COMPB ] _ [ ON, OFF ] Установка прерываний
*/
#define T2


//------------------- Timer 0 ----------------------


// отключить тактирование
#define T0_STOP       TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
// внешний источник - T0, по спаду
#define T0_EXT_FALL   TCCR0B = (TCCR0B & ~_BV(CS00)) | _BV(CS02) | _BV(CS01);
// внешний источник - T0, по фронту
#define T0_EXT_RISE   TCCR0B |= _BV(CS02) | _BV(CS01) | _BV(CS00);

// пред делитель N = [1, 8, 64, 256, 1024]
#define T0_DIV_1      TCCR0B = (TCCR0B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00);
#define T0_DIV_8      TCCR0B = (TCCR0B & ~(_BV(CS02) | _BV(CS00))) | _BV(CS01);
#define T0_DIV_64     TCCR0B = (TCCR0B & ~_BV(CS02)) | _BV(CS01)   | _BV(CS00);
#define T0_DIV_256    TCCR0B = (TCCR0B & ~(_BV(CS01) | _BV(CS00))) | _BV(CS02);
#define T0_DIV_1024   TCCR0B = (TCCR0B & ~_BV(CS01)) | _BV(CS02)   | _BV(CS00);


// циклично 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC0A/OC0B: toggle (f/2)
#define T0_NORMAL \
  TCCR0B &= ~_BV(WGM02);\
  TCCR0A &= ~(_BV(WGM01) | _BV(WGM00));

// CTC, top = OCR0A
// f = clk / N*(1+OCR0A)
// Выводы OC0A/OC0B: toggle (f/2)
#define T0_CTC \
  TCCR0B &= ~_BV(WGM02);\
  TCCR0A &= ~_BV(WGM00);\
  TCCR0A |= _BV(WGM01);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC0A/OC0B: заполнение шим: OCR0A/OCR0B
#define T0_FAST_PWM \
  TCCR0B &= ~_BV(WGM02);\
  TCCR0A |= (_BV(WGM01) | _BV(WGM00));

// настраиваемый шим, top = OCR0A
// f = clk / N*(1+OCR0A)
// Вывод OC0A: заполнение 50%  toggle (f/2)
// Вывод OC0B: заполнение шим: OCR0B
#define T0_FAST_PWM_CUSTOM \
  TCCR0B |= _BV(WGM02);\
  TCCR0A |= (_BV(WGM01) | _BV(WGM00));

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC0A/OC0B: заполнение шим: OCR0A/OCR0B
#define T0_PHASE_PWM \
  TCCR0B &= ~_BV(WGM02);\
  TCCR0A &= ~_BV(WGM01);\
  TCCR0A |= _BV(WGM00);

// симметричный настраиваемый, top = OCR0A
// f = clk / 2*N*(OCR0A)
// Вывод OC0A: заполнение 50%  toggle (f/2)
// Вывод OC0B: заполнение шим: OCR0B
#define T0_PHASE_PWM_CUSTOM \
  TCCR0B |= _BV(WGM02);\
  TCCR0A &= ~_BV(WGM01);\
  TCCR0A |= _BV(WGM00);

/* Режим работы OC0A */

// Вывод OC0A: off
#define T0_OC0A_OFF\
  TCCR0A &= ~(_BV(COM0A1) | _BV(COM0A0));\
  DDRD &= ~_BV(PD6);
// Вывод OC0A: toggle (f/2)
#define T0_OC0A_ON \
  TCCR0A &= ~_BV(COM0A1);\
  TCCR0A |= _BV(COM0A0);\
  TCCR0B |= _BV(FOC0A);\
  DDRD |= _BV(PD6);
// Вывод OC0A: шим
#define T0_OC0A_PWM \
  TCCR0A &= ~_BV(COM0A0);\
  TCCR0A |= _BV(COM0A1);\
  TCCR0B &= ~_BV(FOC0A);\
  DDRD |= _BV(PD6);
// Вывод OC0A: инвертированный шим
#define T0_OC0A_PWM_INV \
  TCCR0A |= (_BV(COM0A1) | _BV(COM0A0));\
  TCCR0B &= ~_BV(FOC0A);\
  DDRD |= _BV(PD6);

/* Режим работы OC0B */

// Вывод OC0B: off
#define T0_OC0B_OFF\
  TCCR0A &= ~(_BV(COM0B1) | _BV(COM0B0));\
  DDRD &= ~_BV(PD5);
// Вывод OC0B: toggle (f/2)
#define T0_OC0B_ON \
  TCCR0A &= ~_BV(COM0B1);\
  TCCR0A |= _BV(COM0B0);\
  TCCR0B |= _BV(FOC0B);\
  DDRD |= _BV(PD5);
// Вывод OC0B: шим
#define T0_OC0B_PWM \
  TCCR0A &= ~_BV(COM0B0);\
  TCCR0A |= _BV(COM0B1);\
  TCCR0B &= ~_BV(FOC0B);\
  DDRD |= _BV(PD5);
// Вывод OC0B: инвертированный шим
#define T0_OC0B_PWM_INV \
  TCCR0A |= (_BV(COM0B1) | _BV(COM0B0));\
  TCCR0B &= ~_BV(FOC0B);\
  DDRD |= _BV(PD5);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER0_COMPA_vect)
#define T0_COMPA_ON   TIMSK0 |= _BV(OCIE0A);
#define T0_COMPA_OFF  TIMSK0 &=~_BV(OCIE0A);
// Использовать: ISR(TIMER0_COMPB_vect)
#define T0_COMPB_ON   TIMSK0 |= _BV(OCIE0B);
#define T0_COMPB_OFF  TIMSK0 &=~_BV(OCIE0B);
// Использовать: ISR(TIMER0_OVF_vect)
#define T0_OVF_ON     TIMSK0 |= _BV(TOIE);
#define T0_OVF_OFF    TIMSK0 &=~_BV(TOIE);


//------------------- Timer 1 ----------------------


// отключить тактирование
#define T1_STOP       TCCR1B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
// внешний источник - T1, по спаду
#define T1_EXT_FALL   TCCR1B = (TCCR1B & ~_BV(CS00)) | _BV(CS02) | _BV(CS01);
// внешний источник - T1, по фронту
#define T1_EXT_RISE   TCCR1B |= _BV(CS02) | _BV(CS01) | _BV(CS00);

// пред делитель N = [1, 8, 64, 256, 1024]
#define T1_DIV_1      TCCR1B = (TCCR1B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00);
#define T1_DIV_8      TCCR1B = (TCCR1B & ~(_BV(CS02) | _BV(CS00))) | _BV(CS01);
#define T1_DIV_64     TCCR1B = (TCCR1B & ~_BV(CS02)) | _BV(CS01)   | _BV(CS00);
#define T1_DIV_256    TCCR1B = (TCCR1B & ~(_BV(CS01) | _BV(CS00))) | _BV(CS02);
#define T1_DIV_1024   TCCR1B = (TCCR1B & ~_BV(CS01)) | _BV(CS02)   | _BV(CS00);


// циклично 16 бит, top = 0xFFFF
// f = clk / N*65536
// Выводы OC1A/OC1B: toggle (f/2)
#define T1_NORMAL \
  TCCR1A &= ~(_BV( WGM11) | _BV( WGM10));\
  TCCR1B &= ~(_BV( WGM13) | _BV( WGM12));

// CTC top = OCR1A [ OCR1AH:OCR1AL ]
// f = clk / N*(1+OCR1A)
// Выводы OC1A/OC1B: toggle (f/2)
#define T1_CTC \
  TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1B |= _BV(WGM12);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_FAST_PWM_8 \
  TCCR1A &= ~_BV(WGM11);\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1A |= _BV(WGM10);\
  TCCR1B |= _BV(WGM12);

// шим 9 бит, top = 0x1FF
// f = clk / N*512
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_FAST_PWM_9 \
  TCCR1A &= ~_BV(WGM10);\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1A |= _BV(WGM11);\
  TCCR1B |= _BV(WGM12);

// шим 10 бит, top = 0x3FF
// f = clk / N*1024
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_FAST_PWM_10 \
  TCCR1A |= _BV(WGM10) | _BV(WGM11);\
  TCCR1B &= ~_BV(WGM13);\
  TCCR1B |= _BV(WGM12);

// настраиваемый шим, top = ICR1 [ ICR1H:ICR1L ]
// f = clk / (1+ICR1)
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_FAST_PWM_CUSTOM \
  TCCR1B |= (_BV(WGM13) | _BV(WGM12));\
  TCCR1A &= ~_BV(WGM10);\
  TCCR1A |= _BV(WGM11);

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_PHASE_PWM_8 \
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));\
  TCCR1A &= ~_BV(WGM11);\
  TCCR1A |= _BV(WGM10);

// симметричный шим 9 бит, top = 0x1FF
// f = clk / N*1024
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_PHASE_PWM_9 \
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));\
  TCCR1A &= ~_BV(WGM10);\
  TCCR1A |= _BV(WGM11);

// симметричный шим 10 бит, top = 0x3FF
// f = clk / N*2048
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_PHASE_PWM_10 \
  TCCR1A |= (_BV(WGM11) | _BV(WGM10));\
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));

// симметричный настраиваемый, top = ICR1 [ ICR1H:ICR1L ]
// f = clk / 2*N*(1+ICR1)
// Выводы OC1A/OC1B: заполнение шим: OCR1A/OCR1B
#define T1_PHASE_PWM_CUSTOM \
  TCCR1B &= ~_BV(WGM12);\
  TCCR1A &= ~_BV(WGM10);\
  TCCR1B |= _BV(WGM13);\
  TCCR1A |= _BV(WGM11);

/* Режим работы OC1A */

// Вывод OC1A: off
#define T1_OC1A_OFF \
TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0));\
  DDRB &= ~_BV(PB1);
// Вывод OC1A: toggle (f/2)
#define T1_OC1A_ON \
  TCCR1A &= ~_BV(COM1A1);\
  TCCR1A |= _BV(COM1A0);\
  TCCR1C |= _BV(FOC1A);\
  DDRB |= _BV(PB1);
// Вывод OC1A: шим
#define T1_OC1A_PWM \
  TCCR1A &= ~_BV(COM1A0);\
  TCCR1A |= _BV(COM1A1);\
  TCCR1C &= ~_BV(FOC1A);\
  DDRB |= _BV(PB1);
// Вывод OC1A: инвертированный шим
#define T1_OC1A_PWM_INV \
  TCCR1A |= (_BV(COM1A1) | _BV(COM1A0));\
  TCCR1C &= ~_BV(FOC1A);\
  DDRB |= _BV(PB1);

/* Режим работы OC1B */

// Вывод OC1B: off
#define T1_OC1B_OFF \
TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));\
  DDRB &= ~_BV(PB2);
// Вывод OC1B: toggle (f/2)
#define T1_OC1B_ON \
  TCCR1A &= ~_BV(COM1B1);\
  TCCR1A |= _BV(COM1B0);\
  TCCR1C |= _BV(FOC1B);\
  DDRB |= _BV(PB2);
// Вывод OC1B: шим
#define T1_OC1B_PWM \
  TCCR1A &= ~_BV(COM1B0);\
  TCCR1A |= _BV(COM1B1);\
  TCCR1C &=~_BV(FOC1B);\
  DDRB |= _BV(PB2);
// Вывод OC1B: инвертированный шим
#define T1_OC1B_PWM_INV \
  TCCR1A |= _BV(COM1B1) | _BV(COM1B0);\
  TCCR1C &=~_BV(FOC1B);\
  DDRB |= _BV(PB2);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER1_COMPA_vect)
#define T1_COMPA_ON   TIMSK1 |= _BV(OCIE1A);
#define T1_COMPA_OFF  TIMSK1 &=~ _BV(OCIE1A);
// Использовать: ISR(TIMER1_COMPB_vect)
#define T1_COMPB_ON   TIMSK1 |= _BV(OCIE1B);
#define T1_COMPB_OFF  TIMSK1 &=~ _BV(OCIE1B);
// Использовать: ISR(TIMER1_OVF_vect)
#define T1_OVF_ON     TIMSK1 |= _BV(TOIE1);
#define T1_OVF_OFF    TIMSK1 &=~ _BV(TOIE1);


//------------------- Timer 2 ----------------------


// отключить тактирование
#define T2_STOP       TCCR2B &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20));

// пред делитель N = [1, 8, 32, 64, 128, 256, 1024]
#define T2_DIV_1      TCCR2B = (TCCR2B & ~(_BV(CS22) | _BV(CS21))) | _BV(CS20);
#define T2_DIV_8      TCCR2B = (TCCR2B & ~(_BV(CS22) | _BV(CS20))) | _BV(CS21);
#define T2_DIV_32     TCCR2B = (TCCR2B & ~_BV(CS22)) | (_BV(CS21) | _BV(CS20));
#define T2_DIV_64     TCCR2B = (TCCR2B & ~(_BV(CS21) | _BV(CS20))) | _BV(CS22);
#define T2_DIV_128    TCCR2B = (TCCR2B & ~_BV(CS21)) | (_BV(CS22) | _BV(CS20));
#define T2_DIV_256    TCCR2B = (TCCR2B & ~_BV(CS20)) | (_BV(CS22) | _BV(CS21));
#define T2_DIV_1024   TCCR2B |= (_BV(CS22) | _BV(CS21) | _BV(CS20));


// циклично 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC2A/OC2B: toggle (f/2)
#define T2_NORMAL \
  TCCR2B &= ~_BV(WGM22);\
  TCCR2A &= ~(_BV(WGM21) | _BV(WGM20));

// CTC, top = OCR2A
// f = clk / N*(1+OCR2A)
// Выводы OC2A/OC2B: toggle (f/2)
#define T2_CTC \
  TCCR2B &= ~_BV(WGM22);\
  TCCR2A &= ~_BV(WGM20);\
  TCCR2A |= _BV(WGM21);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Выводы OC2A/OC2B: заполнение шим: OCR2A/OCR2B
#define T2_FAST_PWM \
  TCCR2B &= ~_BV(WGM22);\
  TCCR2A |= (_BV(WGM21) | _BV(WGM20));

// настраиваемый шим, top = OCR2A
// f = clk / N*(1+OCR2A)
// Вывод OC2A: заполнение 50%  toggle (f/2)
// Вывод OC2B: заполнение шим: OCR2B
#define T2_FAST_PWM_CUSTOM \
  TCCR2B |= _BV(WGM22);\
  TCCR2A |= (_BV(WGM21) | _BV(WGM20));

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC2A/OC2B: заполнение шим: OCR2A/OCR2B
#define T2_PHASE_PWM \
  TCCR2B &= ~_BV(WGM22);\
  TCCR2A &= ~_BV(WGM21);\
  TCCR2A |= _BV(WGM20);

// симметричный настраиваемый, top = OCR2A
// f = clk / 2*N*(OCR2A)
// Вывод OC2A: заполнение 50%  toggle (f/2)
// Вывод OC2B: заполнение шим: OCR2B
#define T2_PHASE_PWM_CUSTOM \
  TCCR2B |= _BV(WGM22);\
  TCCR2A &= ~_BV(WGM21);\
  TCCR2A |= _BV(WGM20);

/* Режим работы OC2A */

// Вывод OC2A: off
#define T2_OC2A_OFF \
  TCCR2A &= ~(_BV(COM2A1) | _BV(COM2A0));\
  DDRB &= ~_BV(PB3);
// Вывод OC2A: toggle (f/2)
#define T2_OC2A_ON \
  TCCR2A &= ~_BV(COM2A1);\
  TCCR2A |= _BV(COM2A0);\
  TCCR2B |= _BV(FOC2A);\
  DDRB |= _BV(PB3);
// Вывод OC2A: шим
#define T2_OC2A_PWM \
  TCCR2A &= ~_BV(COM2A0);\
  TCCR2A |= _BV(COM2A1);\
  TCCR2B &= ~_BV(FOC2A);\
  DDRB |= _BV(PB3);
// Вывод OC2A: инвертированный шим
#define T2_OC2A_PWM_INV \
  TCCR2A |= (_BV(COM2A1) | _BV(COM2A0));\
  TCCR2B &= ~_BV(FOC2A);\
  DDRB |= _BV(PB3);

/* Режим работы OC2B */

// Вывод OC2B: off
#define T2_OC2B_OFF \
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));\
  DDRD &= ~_BV(PD3);
// Вывод OC2B: toggle (f/2)
#define T2_OC2B_ON \
  TCCR2A &= ~_BV(COM2B1);\
  TCCR2A |= _BV(COM2B0);\
  TCCR2B |= _BV(FOC2B);\
  DDRD |= _BV(PD3);
// Вывод OC2B: шим
#define T2_OC2B_PWM \
  TCCR2A &= ~_BV(COM2B0);\
  TCCR2A |= _BV(COM2B1);\
  TCCR2B &= ~_BV(FOC2B);\
  DDRD |= _BV(PD3);
// Вывод OC2B: инвертированный шим
#define T2_OC2B_PWM_INV \
  TCCR2A |= (_BV(COM2B1) | _BV(COM2B0));\
  TCCR2B &= ~_BV(FOC2B);\
  DDRD |= _BV(PD3);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER2_COMPA_vect)
#define T2_COMPA_ON   TIMSK2 |= _BV(OCIE2A);
#define T2_COMPA_OFF  TIMSK2 &=~ _BV(OCIE2A);
// Использовать: ISR(TIMER2_COMPB_vect)
#define T2_COMPB_ON   TIMSK2 |= _BV(OCIE2B);
#define T2_COMPB_OFF  TIMSK2 &=~ _BV(OCIE2B);
// Использовать: ISR(TIMER2_OVF_vect)
#define T2_OVF_ON     TIMSK2 |= _BV(TOIE2);
#define T2_OVF_OFF    TIMSK2 &=~ _BV(TOIE2);

#endif
