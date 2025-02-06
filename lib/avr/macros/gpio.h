#pragma once
#include <Arduino.h>

// Управление портами

#define CONCAT(x, y)  x ## y

#define PORT(x) CONCAT(PORT , x)
#define DDR(x)  CONCAT(DDR  , x)
#define PIN(x)  CONCAT(PIN  , x)

#define GPIO(port, pin)
#define IN(port, pin)         DDR(port)  &=~ _BV(pin)
#define OUT(port, pin)        DDR(port)  |=  _BV(pin)
#define SET(port, pin)        PORT(port) |=  _BV(pin)
#define CLR(port, pin)        PORT(port) &=~ _BV(pin)
#define GET(port, pin)        (PIN(port)  &  _BV(pin))
#define MASK(port, pin)                      _BV(pin)
#define MMO(port, pin)        PORT(port)
#define MMI(port, pin)        PIN(port)
