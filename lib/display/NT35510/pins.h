#pragma once
#include "core.h"

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_WR       Тактирование / low -> high


#ifdef MIK32V2
// #define L_RD(x)   x ( 0, 8 )
// #define L_WR(x)   x ( 0, 9 )
// #define L_RS(x)   x ( 0, 10)

#define L_WR(x)   x ( 1, 8 )
#define L_RS(x)   x ( 1, 9 )
#define L_CS(x)   x ( 1, 15)
#define L_RST(x)  x ( 0, 0 )  // Не используется
#define L_PORT(x) x ( 0, 0 )

#define SEL_0(x)  x ( 1, 10)
#endif
