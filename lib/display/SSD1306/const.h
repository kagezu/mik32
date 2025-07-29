#pragma once
#include <avr/pgmspace.h>

// Fundamental Command

#define SetMemoryAddressingMode 0x20
#define SetColumnAddress        0x21
#define SetPageAddress          0x22
#define SetDisplayStartLine     0x40
#define SetContrastControl      0x81
#define Set_0x8D                0x8D
#define SetSegmentMap           0xA0
#define SetSegmentMapFlip       0xA1
#define EntireDisplayON         0xA4
#define EntireDisplayON_Force   0xA5
#define SetNormalDisplay        0xA6
#define SetInverseDisplay       0xA7
#define SetMultiplexRatio       0xA8
#define SetDisplayOFF           0xAE
#define SetDisplayON            0xAF
#define SetScanDirection        0xC0
#define SetScanDirectionFlip    0xC8
#define SetDisplayOffset        0xD3
#define SetDisplayClock         0xD5
#define SetPrechargePeriod      0xD9
#define SetPinsConfig           0xDA
#define SetVcomhDeselectLevel   0xDB

// GFX Acceleration

#define DeactivateScroll        0x2E


#define LCD_I2C_ADDR            0x78

// static const uint8_t SSD1306_CONFIG[] PROGMEM = {
//   SetDisplayOFF,
//   SetDisplayClock, 0x80,
//   SetDisplayOffset, 0,
//   SetDisplayStartLine,
//   Set_0x8D, 0x14,//CHARGE_DCDC 0x10 off  0x14 on
//   SetMemoryAddressingMode, 0,
//   SetVcomhDeselectLevel, 0x40,//	Устанавливаем питание светодиодов VcomH в значение выше чем по умолчанию (0x30), что увеличит яркость дисплея (допустимые значения: 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70).
//   EntireDisplayON,
//   SetNormalDisplay,
//   DeactivateScroll
// };

static const uint8_t SSD1306_CONFIG[] PROGMEM = {
  0xAE,        // Display off
  0xD5, 0x80,  // Clock divide ratio
  0xA8, 0x3F,  // Set MUX ratio
  0xD3, 0x00,  // Display offset
  0x40,        // Start line
  0x8D, 0x14,  // Charge pump
  0x20, 0x00,  // Memory mode (horizontal)
  0xA0,        // Segment remap
  0xC0,        // Scan direction
  // 0xDA, 0x12,  // COM pins
  0x81, 0xCF,  // Contrast control
  0xD9, 0xF1,  // Precharge
  0xDB, 0x40,  // Vcomh
  0xA4,        // Entire display on
  0xA6,        // Normal display
  // 0xAF         // Display on
};
