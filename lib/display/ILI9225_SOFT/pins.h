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
#define ILI_SOFT_SCK(x)     x ( C, PC1)
#define ILI_SOFT_SDA(x)     x ( C, PC2)
#define ILI_SOFT_RS(x)      x ( C, PC3)
#define ILI_SOFT_RST(x)     x ( C, PC4)
#define ILI_SOFT_CS(x)      x ( C, PC5)
#endif

#ifdef MIK32V2
#define ILI_SOFT_SCK(x)     x ( 1, 7 )     // A1
#define ILI_SOFT_SDA(x)     x ( 0, 4 )     // A2
#define ILI_SOFT_RS(x)      x ( 0, 7 )     // A3
#define ILI_SOFT_RST(x)     x ( 0, 9 )     // A4
#define ILI_SOFT_CS(x)      x ( 1, 15)     // A5
#endif

#ifdef CH32V20x_D6

#define ILI_SOFT_CS(x)      x ( A, 11 )
#define ILI_SOFT_RS(x)      x ( A, 10 )
#define ILI_SOFT_RST(x)     x ( D, 0  )
#define ILI_SOFT_SCK(x)     x ( B, 13 )
#define ILI_SOFT_SDA(x)     x ( B, 15 )

#endif

#endif
