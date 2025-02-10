#pragma once
#include "core.h"

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_SCK      Тактирование / low -> high
// L_SDA      Данные

#ifdef __AVR_ATmega328P__

#define L_RD(x)   x ( C, 0 )
#define L_WR(x)   x ( C, 1 )
#define L_RS(x)   x ( C, 2 )
#define L_CS(x)   x ( C, 3 )
#define L_RST(x)  x ( C, 4 )

#define L_DATA    D

#endif
