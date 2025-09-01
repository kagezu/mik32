#pragma once
#include "core.h"

//===================== Config =============================

// ST_SPI_CS       Выбор дисплея
// ST_SPI_RS       0 = Запись команды / 1 = Запись данных
// ST_SPI_RST      0 = Reset

#ifndef ILI_SPI_RS

#ifdef __AVR_ATmega328P__
#define ILI_SPI_CS(x)      x ( C, 0)
#define ILI_SPI_RS(x)      x ( C, 0)
#define ILI_SPI_RST(x)     x ( C, 0) // На самом деле сброс не подключён
#endif

#ifdef MIK32V2
#define ILI_SPI_CS(x)      x ( 1, 4 ) 
#define ILI_SPI_RS(x)      x ( 1, 9 )
#define ILI_SPI_RST(x)     x ( 1, 6 )  // Переключаем на NSS1
#endif

#ifdef CH32V20x_D6
#define ILI_SPI_CS(x)      x ( A, 11 )
#define ILI_SPI_RS(x)      x ( A, 10 )
#define ILI_SPI_RST(x)     x ( D, 0  ) // Не подключён
#endif

#endif
