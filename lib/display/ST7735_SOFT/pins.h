#pragma once
#include <core.h>

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_SCK      Тактирование / low -> high
// L_SDA      Данные

#ifndef L_SCK

#ifdef __AVR_ATmega328P__

// #define L_SCK(x)     x ( C, PC0)
// #define L_SDA(x)     x ( C, PC1)
// #define L_RS(x)      x ( C, PC2)
// #define L_RST(x)     x ( C, PC3)
// #define L_CS(x)      x ( C, PC4)

// Подключение как к SPI
#define L_SCK(x)     x ( B, PB5)
#define L_SDA(x)     x ( B, PB3)
#define L_RS(x)      x ( C, PC4)
#define L_CS(x)      x ( C, PC5)
#define L_RST(x)     x ( C, PC6)

#endif

#ifdef __AVR_ATmega128__

#define L_SCK(x)     x ( B, PB1)
#define L_SDA(x)     x ( B, PB2)
#define L_RS(x)      x ( E, PE3)
#define L_CS(x)      x ( E, PE2)
#define L_RST(x)     x ( E, PE7)

#endif

#ifdef MIK32V2

// #define L_SCK(x)     x ( 1, 5 )
// #define L_SDA(x)     x ( 1, 7 )
// #define L_RS(x)      x ( 0, 4 )
// #define L_RST(x)     x ( 0, 7 )
// #define L_CS(x)      x ( 0, 9 )

// Подключение как к SPI
#define L_SCK(x)     x ( 1, 2 )
#define L_SDA(x)     x ( 1, 1 )
#define L_RS(x)      x ( 0, 9 )
#define L_RST(x)     x ( 0, 7 )
#define L_CS(x)      x ( 1, 15 )

#endif

#endif
