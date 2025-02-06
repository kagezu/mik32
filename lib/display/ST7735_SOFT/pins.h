#pragma once
#include "core.h"

#if (!defined (PRESET_ST7735_A5) && !defined (PRESET_ST7735_A4) && !defined (PRESET_ST7735_SPI))
// Конфигурация по умолчанию
#define PRESET_ST7735_A4
#endif

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_SCK      Тактирование / low -> high
// L_SDA      Данные

#ifdef __AVR_ATmega328P__

#ifdef PRESET_ST7735_A4
#define L_SCK(x)     x ( C, PC0)
#define L_SDA(x)     x ( C, PC1)
#define L_RS(x)      x ( C, PC2)
#define L_RST(x)     x ( C, PC3)
#define L_CS(x)      x ( C, PC4)
#endif

#ifdef PRESET_ST7735_A5
#define L_SCK(x)     x ( C, PC1)
#define L_SDA(x)     x ( C, PC2)
#define L_RS(x)      x ( C, PC3)
#define L_RST(x)     x ( C, PC4)
#define L_CS(x)      x ( C, PC5)
#endif

#ifdef PRESET_ST7735_SPI
#define L_SCK(x)     x ( B, PB5)
#define L_SDA(x)     x ( B, PB3)
#define L_RS(x)      x ( C, PC4)
#define L_CS(x)      x ( C, PC5)
#define L_RST(x)     x ( C, PC6)
#endif
#endif

#ifdef __AVR_ATmega128__

#define L_SCK(x)     x ( B, PB1)
#define L_SDA(x)     x ( B, PB2)
#define L_RS(x)      x ( E, PE3)
#define L_CS(x)      x ( E, PE2)
#define L_RST(x)     x ( E, PE7)

#endif

#ifdef MIK32V2

#ifdef PRESET_ST7735_A4
#define L_SCK(x)     x ( 1, 5 )     // A0
#define L_SDA(x)     x ( 1, 7 )     // A1
#define L_RS(x)      x ( 0, 4 )     // A2
#define L_RST(x)     x ( 0, 7 )     // A3
#define L_CS(x)      x ( 0, 9 )     // A4
#endif

#ifdef PRESET_ST7735_A5
#define L_SCK(x)     x ( 1, 7 )     // A1
#define L_SDA(x)     x ( 0, 4 )     // A2
#define L_RS(x)      x ( 0, 7 )     // A3
#define L_RST(x)     x ( 0, 9 )     // A4 Необходима подтяжка к питанию (на стороне девайса)
#define L_CS(x)      x ( 1, 15 )    // A5 Необходима подтяжка к земле (на стороне девайса)
#endif

#ifdef PRESET_ST7735_SPI
#define L_SCK(x)     x ( 1, 2 )     // D13
#define L_SDA(x)     x ( 1, 1 )     // D11
#define L_RST(x)
#define L_RS(x)      x ( 0, 9 )     // A4
#define L_CS(x)      x ( 1, 15 )    // A5 Необходима подтяжка к земле (на стороне девайса)
#endif

#endif
