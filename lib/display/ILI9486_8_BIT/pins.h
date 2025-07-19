#pragma once
#include "core.h"

//===================== Config =============================

// ILI_8_CS       Выбор дисплея
// ILI_8_RS       0 = Запись команды / 1 = Запись данных
// ILI_8_RST      0 = Reset
// ILI_8_WR       Тактирование / low -> high
// ILI_8_RD       ?

#ifdef __AVR_ATmega328P__
#define ILI_8_RD(x)   x ( C, 0 )
#define ILI_8_WR(x)   x ( C, 1 )
#define ILI_8_RS(x)   x ( C, 2 )
#define ILI_8_CS(x)   x ( C, 3 )
#define ILI_8_RST(x)  x ( C, 4 )
#define ILI_8_PORT(x) x ( D, 8 )
#endif

#ifdef __AVR_ATmega128__
#define ILI_8_RD(x)   x ( F, 0 )
#define ILI_8_WR(x)   x ( F, 1 )
#define ILI_8_RS(x)   x ( F, 2 )
#define ILI_8_CS(x)   x ( F, 3 )
#define ILI_8_RST(x)  x ( F, 4 )
#define ILI_8_PORT(x) x ( D, 8 )
#endif

#ifdef MIK32V2
#define ILI_8_RD(x)   x ( 0, 8 )
#define ILI_8_WR(x)   x ( 0, 9 )
#define ILI_8_RS(x)   x ( 0, 10)
#define ILI_8_CS(x)   x ( 1, 15)
#define ILI_8_RST(x)  x ( 0, 0 )
#define ILI_8_PORT(x) x ( 0, 16)
#endif
