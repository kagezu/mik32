#pragma once
#include <core.h>

//===================== Config =============================

// SH_LD       0 = Запись в регистр / 1 = Фиксация (hc165)
// SH_RST      0 = Reset (hc164)
// SH_SCK      Тактирование / low -> high
// SH_SDA      Данные

#ifndef SH_SDA

#ifdef __AVR_ATmega328P__
#define SH_RST(x)
#define SH_LD(x)      x (D, PD5)
#define SH_SCK(x)     x (D, PD4)
#define SH_SDA(x)     x (D, PD3)
#endif

#ifdef MIK32V2
#define SH_LD(x)      x (D, PD5)
#define SH_RST(x)     x (D, PD5)
#define SH_SCK(x)     x (D, PD4)
#define SH_SDA(x)     x (D, PD3)
#endif

#endif
