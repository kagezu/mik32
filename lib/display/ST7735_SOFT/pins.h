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

#define ST_SOFT_SCK(x)     x ( C, PC1)
#define ST_SOFT_SDA(x)     x ( C, PC2)
#define ST_SOFT_RS(x)      x ( C, PC3)
#define ST_SOFT_RST(x)     x ( C, PC4)
#define ST_SOFT_CS(x)      x ( C, PC5)
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

#define ST_SOFT_SCK(x)     x ( 1, 7 )     // A1
#define ST_SOFT_SDA(x)     x ( 0, 4 )     // A2
#define ST_SOFT_RS(x)      x ( 0, 7 )     // A3
#define ST_SOFT_RST(x)     x ( 0, 9 )     // A4
#define ST_SOFT_CS(x)      x ( 1, 15)     // A5

// #define ST_SOFT_SCK(x)     x ( 1, 2 )
// #define ST_SOFT_SDA(x)     x ( 1, 1 )
// #define ST_SOFT_RS(x)      x ( 0, 9 )
// #define ST_SOFT_RST(x)     x ( 2, 15)
// #define ST_SOFT_CS(x)      x ( 1, 15)
#endif

#ifdef CH32V20x_D6

#define ST_SOFT_SCK(x)     x ( B, 13 )
#define ST_SOFT_SDA(x)     x ( B, 15 )
#define ST_SOFT_RS(x)      x ( A, 10 )
#define ST_SOFT_RST(x)     x ( D, 0  )
#define ST_SOFT_CS(x)      x ( A, 11 )

#endif

#endif

static inline void GPIO_ST_SOFT()
{
  ST_SOFT_SCK(GPIO); ST_SOFT_SDA(GPIO); ST_SOFT_RST(GPIO); ST_SOFT_CS(GPIO); ST_SOFT_RS(GPIO);
  ST_SOFT_SCK(OUT); ST_SOFT_SDA(OUT); ST_SOFT_RST(OUT); ST_SOFT_CS(OUT); ST_SOFT_RS(OUT);
  ST_SOFT_CS(SET); ST_SOFT_RS(SET);
}