#pragma once
#include "core.h"

//===================== Config =============================

// NT_CS       Выбор дисплея
// NT_RS       0 = Запись команды / 1 = Запись данных
// NT_RST      0 = Reset
// NT_WR       Тактирование / low -> high


#ifdef MIK32V2
// #define NT_RD(x)   x ( 0, 8 )
// #define NT_WR(x)   x ( 0, 9 )
// #define NT_RS(x)   x ( 0, 10)

// #define NT_WR(x)   x ( 1, 8 )
#define NT_WR(x)   x ( 1, 0 )
#define NT_RS(x)   x ( 1, 9 )
#define NT_CS(x)   x ( 1, 15)
#define NT_RST(x)  x ( 0, 0 )  // Не используется
#define NT_PORT(x) x ( 0, 0 )

#define SEL_0(x)  x ( 1, 10)
#endif
