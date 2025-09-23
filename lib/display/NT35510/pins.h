#pragma once
#include "core.h"

//===================== Config =============================

// NT_CS       Выбор дисплея
// NT_RS       0 = Запись команды / 1 = Запись данных
// NT_RST      0 = Reset
// NT_WR       Тактирование / low -> high

#ifdef MIK32V2
Pin<2, 8> NT_RD;
Pin<1, 0> NT_WR;
Pin<1, 9> NT_RS;
Pin<1, 15> NT_CS;
Port<0, 0xFFFF> NT_PORT;
#endif

#ifdef CH32V20x_D6
Pin<PA, 8> NT_WR;
Pin<PA, 9> NT_RD;
Pin<PA, 10> NT_RS;
Pin<PA, 11> NT_CS;
Port<PB, 0xFFFF> NT_PORT;
#endif
