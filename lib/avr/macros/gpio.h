#pragma once

// Управление портами

#define CONCAT(x, y)  x ## y

#define PORT(x) CONCAT(PORT , x)
#define DDR(x)  CONCAT(DDR  , x)
#define PIN(x)  CONCAT(PIN  , x)

#define ANALOG(port, pin)     DDR(port)  &=~ _BV(pin); DIDR0 |= _BV(pin); ADMUX = (ADMUX &~7) | pin
#define IN(port, pin)         DDR(port)  &=~ _BV(pin)
#define OUT(port, pin)        DDR(port)  |=  _BV(pin)
#define SET(port, pin)        PORT(port) |=  _BV(pin)
#define CLR(port, pin)        PORT(port) &=~ _BV(pin)
#define INV(port, pin)        PORT(port) ^=  _BV(pin)
#define GET(port, pin)        (PIN(port)  &  _BV(pin))
#define MASK(port, pin)                      _BV(pin)
#define OUTPUT(port, pin)     PORT(port)
#define STATE(port, pin)      PIN(port)



// Для совместимости

#define GPIO(port, pin)
#define DS_2MA(port, pin)
#define DS_4MA(port, pin)
#define DS_8MA(port, pin)

#define P_NC(port, pin)       DDR(port)  &=~ _BV(pin); PORT(port) &=~ _BV(pin); MCUCR &= ~_BV(PUD)
#define P_VCC(port, pin)      DDR(port)  &=~ _BV(pin); PORT(port) |=  _BV(pin); MCUCR |= _BV(PUD)
#define P_GND(port, pin)      DDR(port)  &=~ _BV(pin); PORT(port) &=~ _BV(pin); MCUCR &= ~_BV(PUD)
