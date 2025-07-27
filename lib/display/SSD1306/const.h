#pragma once

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


#define LCD_I2C_ADDR            0x3C

static const uint8_t SSD1306_CONFIG[] PROGMEM = {
  SetDisplayClock, 0x80,
  SetDisplayOffset, 0,
  SetDisplayStartLine,
  Set_0x8D, 0x14,//CHARGE_DCDC 0x10 off  0x14 on
  SetMemoryAddressingMode, 0,
  SetVcomhDeselectLevel, 0x40,//	Устанавливаем питание светодиодов VcomH в значение выше чем по умолчанию (0x30), что увеличит яркость дисплея (допустимые значения: 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70).
  EntireDisplayON,
  SetNormalDisplay,
  DeactivateScroll
};
