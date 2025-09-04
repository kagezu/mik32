#pragma once
#include "pinout.h"

//===================== Config =============================

// ENCODER_A   Вывод А
// ENCODER_B   Вывод В
// ENCODER_SW  Кнопка


#ifdef MIK32V2
#define ENCODER_A   D2
#define ENCODER_GND D3
#define ENCODER_B   D4
#define ENCODER_SW  USER_B
#endif

// #ifdef MIK32V2
// #define ENCODER_A   D19
// #define ENCODER_B   D18
// #define ENCODER_SW  D10
// #endif
