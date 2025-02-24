#pragma once
#include "core.h"

// X_DREQ     HIGH = буфер свободен / команды выполнены
// X_CS       Отправить команду (доступ к регистрам) буфер 32 байта
// X_DCS      Отправить данные => буфер 2к байт
// X_RESET    Аппаратный сброс

#ifdef __AVR_ATmega328P__
#define X_DREQ(f)   f (B, PB0)
#define X_CS(f)     f (C, PC5)
#define X_DCS(f)    f (C, PC4)
#endif

#ifdef MIK32V2
#define X_DREQ(f)   f (0, 10)
#define X_CS(f)     f (0, 2)
#define X_DCS(f)    f (1, 8)
#define X_RESET(f)  f (1, 9)
#endif
