#pragma once
#include "core.h"

//===================== Config =============================

// ST_8_CS       Выбор дисплея
// ST_8_RS       0 = Запись команды / 1 = Запись данных
// ST_8_RST      0 = Reset
// ST_8_WR       Тактирование / low -> high
// ST_8_RD       ?

#ifdef __AVR_ATmega328P__
#define ST_8_RD(x)   x ( C, 0 )
#define ST_8_WR(x)   x ( C, 1 )
#define ST_8_RS(x)   x ( C, 2 )
#define ST_8_CS(x)   x ( C, 3 )
#define ST_8_RST(x)  x ( C, 8 )
#define ST_8_PORT(x) x ( D, 8 )
#endif

#ifdef __AVR_ATmega128__
#define ST_8_RD(x)   x ( F, 0 )
#define ST_8_WR(x)   x ( F, 1 )
#define ST_8_RS(x)   x ( F, 2 )
#define ST_8_CS(x)   x ( F, 3 )
#define ST_8_RST(x)  x ( F, 4 )
#define ST_8_PORT(x) x ( D, 8 )
#endif

#ifdef MIK32V2
#define ST_8_RD(x)   x ( 0, 8 )     // D4
#define ST_8_WR(x)   x ( 0, 9 )     // A4
#define ST_8_RS(x)   x ( 0, 10)     // D2
#define ST_8_CS(x)   x ( 1, 15)
#define ST_8_RST(x)  x ( 0, 16)
#define ST_8_PORT(x) x ( 0, 0 )
#endif
