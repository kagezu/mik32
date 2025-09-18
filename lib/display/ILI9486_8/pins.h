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
Pin<0, 8 > ILI_8_RD;
Pin<0, 9 > ILI_8_WR;
Pin<0, 10> ILI_8_RS;
Pin<1, 15> ILI_8_CS;
Pin<0, 0 > ILI_8_RST;

#define ILI_8_MASK  0x02FF
Port<0, ILI_8_MASK> ILI_8_PORT;
#endif
