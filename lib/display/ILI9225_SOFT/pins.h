#pragma once
#include "core.h"

//===================== Config =============================

// ILI_SOFT_CS       Выбор дисплея
// ILI_SOFT_RS       0 = Запись команды / 1 = Запись данных
// ILI_SOFT_RST      0 = Reset
// ILI_SOFT_SCK      Тактирование / low -> high
// ILI_SOFT_SDA      Данные

#ifndef ILI_SOFT_RS

#ifdef __AVR_ATmega328P__
Pin<PB, PB5> ILI_SOFT_SCK;
Pin<PB, PB3> ILI_SOFT_SDA;
Pin<PC, PC4> ILI_SOFT_RS;
Pin<PC, PC6> ILI_SOFT_RST;
Pin<PC, PC5> ILI_SOFT_CS;
// Pin<PC, 0> ILI_SOFT_PORT;
#endif

#ifdef MIK32V2
Pin<1, 7 > ILI_SOFT_SCK;  // A1
Pin<0, 4 > ILI_SOFT_SDA;  // A2
Pin<0, 7 > ILI_SOFT_RS;   // A3
Pin<0, 9 > ILI_SOFT_RST;  // A4
Pin<1, 15> ILI_SOFT_CS;   // A5
#endif

#ifdef CH32V20x_D6

Pin<PA, 11> ILI_SOFT_CS;
Pin<PA, 10> ILI_SOFT_RS;
Pin<0, 0 > ILI_SOFT_RST;
Pin<PB, 13> ILI_SOFT_SCK;
Pin<PB, 15> ILI_SOFT_SDA;

#endif

#endif
