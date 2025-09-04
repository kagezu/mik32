#pragma once
#include <core.h>

//===================== Config =============================

// ST_SPI_CS       Выбор дисплея
// ST_SPI_RS       0 = Запись команды / 1 = Запись данных
// ST_SPI_RST      0 = Reset

#ifndef ST_SPI_RS

#ifdef __AVR_ATmega328P__
#define ST_SPI_RS(x)      x ( C, PC4)
#define ST_SPI_CS(x)      x ( C, PC5)
#define ST_SPI_RST(x)     x ( C, PC6) // На самом деле сброс не подключён
#endif

#ifdef __AVR_ATmega128__
#define ST_SPI_RS(x)      x ( E, PE3)
#define ST_SPI_CS(x)      x ( E, PE2)
#define ST_SPI_RST(x)     x ( E, PE7) // На самом деле сброс не подключён
#endif

#ifdef MIK32V2
#define ST_SPI_CS(x)      x ( 1, 4 )
#define ST_SPI_RS(x)      x ( 1, 9 )
#define ST_SPI_RST(x)     x ( 1, 6 ) // На самом деле сброс не подключён
#endif

#ifdef CH32V20x_D6
#define ST_SPI_CS(x)      x ( A, 11 )
#define ST_SPI_RS(x)      x ( A, 10 )
#define ST_SPI_RST(x)     x ( D, 0  ) // Не подключён
#endif

#endif
