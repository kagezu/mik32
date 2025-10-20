#pragma once
#include "core.h"

//===================== Config =============================

// ST_SOFT_CS       Выбор дисплея
// ST_SOFT_RS       0 = Запись команды / 1 = Запись данных
// ST_SOFT_RST      0 = Reset
// ST_SOFT_SCK      Тактирование / low -> high
// ST_SOFT_SDA      Данные

#ifndef ST_SOFT_RS

#ifdef __AVR_ATmega328P__
// #define ST_SOFT_SCK(x)     x ( C, PC0)
// #define ST_SOFT_SDA(x)     x ( C, PC1)
// #define ST_SOFT_RS(x)      x ( C, PC2)
// #define ST_SOFT_RST(x)     x ( C, PC3)
// #define ST_SOFT_CS(x)      x ( C, PC4)

Pin<PC, PC2> ST_SOFT_CS;
Pin<PC, PC1> ST_SOFT_RS;
Pin<PB, PB3> ST_SOFT_SDA;
Pin<PB, PB5> ST_SOFT_SCK;
Pin<PC, PC0> ST_SOFT_RST;

#endif

#ifdef __AVR_ATmega128__
#define ST_SOFT_SCK(x)     x ( B, PB1)
#define ST_SOFT_SDA(x)     x ( B, PB2)
#define ST_SOFT_RS(x)      x ( E, PE3)
#define ST_SOFT_CS(x)      x ( E, PE2)
#define ST_SOFT_RST(x)     x ( E, PE7)
#endif

#ifdef MIK32V2
// #define ST_SOFT_SCK(x)     x ( 1, 5 )     // A0
// #define ST_SOFT_SDA(x)     x ( 1, 7 )     // A1
// #define ST_SOFT_RS(x)      x ( 0, 4 )     // A2
// #define ST_SOFT_RST(x)     x ( 0, 7 )     // A3
// #define ST_SOFT_CS(x)      x ( 0, 9 )     // A4

Pin<1, 7 > ST_SOFT_SCK;   // A1
Pin<0, 4 > ST_SOFT_SDA;   // A2
Pin<0, 7 > ST_SOFT_RS;    // A3
Pin<0, 9 > ST_SOFT_RST;   // A4
Pin<1, 15> ST_SOFT_CS;    // A5

// #define ST_SOFT_SCK(x)     x ( 1, 2 )
// #define ST_SOFT_SDA(x)     x ( 1, 1 )
// #define ST_SOFT_RS(x)      x ( 0, 9 )
// #define ST_SOFT_RST(x)     x ( 2, 15)
// #define ST_SOFT_CS(x)      x ( 1, 15)
#endif

#ifdef CH32V20x_D6

Pin<PB, 13> ST_SOFT_SCK;
Pin<PB, 15> ST_SOFT_SDA;
Pin<PA, 10> ST_SOFT_RS;
Pin<0, 0> ST_SOFT_RST;
Pin<PA, 11> ST_SOFT_CS;

#endif

#endif
