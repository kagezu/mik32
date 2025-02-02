#pragma once
#include <core.h>

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_SCK      Тактирование / low -> high
// L_SDA      Данные

#ifndef L_CS

#ifdef __AVR_ATmega328P__

#define L_RS(x)      x ( C, PC4)
#define L_CS(x)      x ( C, PC5)
#define L_RST(x)     x ( C, PC6) // На самом деле сброс не подключён

#define SPI_SS(x)     x (B, PB2)

#endif

#ifdef MIK32V2

#define L_RST(x)     x ( 0, 7 )
#define L_RS(x)      x ( 0, 9 )
#define L_CS(x)      x ( 1, 15 ) // Низкий уровень => CS подтянут к 0, RS - активный / высокий => RS подтянут к 0, CS = RS

#endif

#endif
