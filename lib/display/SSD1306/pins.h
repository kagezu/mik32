#pragma once
#include "pinout.h"

//===================== Config =============================


#ifdef __AVR_ATmega328P__
#define SSD1306_SDA I2C1_SDA
#define SSD1306_CLK I2C1_SCL
#endif

#ifdef MIK32V2
#endif
