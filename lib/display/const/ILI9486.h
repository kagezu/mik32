#pragma once
#include <core.h>

// Команды дисплея

#define SWRESET 0x01   // Software reset
#define RDDID 0x04     // Read Display ID
#define RDDST 0x09     // Read Display Status
#define RDDPM 0x0a     // Read Display Power
#define RDDMADCTL 0x0b // Read Display
#define RDDCOLMOD 0x0c // Read Display Pixel
#define RDDIM 0x0d     // Read Display Image
#define RDDSM 0x0e     // Read Display Signal
#define SLPIN 0x10     // Sleep in & booster off
#define SLPOUT 0x11    // Sleep out & booster on
#define PTLON 0x12     // Partial mode on
#define NORON 0x13     // Partial off (Normal)
#define INVOFF 0x20    // Display inversion off
#define INVON 0x21     // Display inversion on
#define GAMSET 0x26    // Gamma curve select              uint8_t   [ GC3 GC2 GC1 GC0 ]
#define DISPOFF 0x28   // Display off
#define DISPON 0x29    // Display on
#define CASET 0x2a     // Column address set              start: duint16_t      stop: duint16_t
#define RASET 0x2b     // Row address set                 start: duint16_t      stop: duint16_t
#define RAMWR 0x2c     // Memory write
#define RAMRD 0x2e     // Memory read
#define PTLAR 0x30     // Partial start/end address set
#define TEOFF 0x34     // Tearing effect line off
#define TEON 0x35      // Tearing effect mode set & on
#define MADCTL 0x36    // Memory data access control
#define IDMOFF 0x38    // Idle mode off
#define IDMON 0x39     // Idle mode on
#define COLMOD 0x3a    // Interface pixel format
#define RDID1 0xda     // Read ID1
#define RDID2 0xdb     // Read ID2
#define RDID3 0xdc     // Read ID3

// Panel Function Command List and Description

#define FRMCTR1 0xb1  // In normal mode (Full colors)
#define FRMCTR2 0xb2  // In Idle mode (8-colors)
#define FRMCTR3 0xb3  // In partial mode + Full colors
#define INVCTR 0xb4   // Display inversion control        uint8_t  [ NLA NLB NLC ] set inversion
#define DISSET5 0xb6  // Display function setting
#define PWCTR1 0xc0   // Power control setting
#define PWCTR2 0xc1   // Power control setting
#define PWCTR3 0xc2   // In normal mode (Full colors)
#define PWCTR4 0xc3   // In Idle mode (8-colors)
#define PWCTR5 0xc4   // In partial mode + Full colors
#define VMCTR1 0xc5   // VCOM control 1
#define VMOFCTR 0xc7  // Set VCOM offset control
#define WRID2 0xd1    // Set LCM version code
#define WRID3 0xd2    // Customer Project code
#define PWCTR6 0xfc   // In partial mode + Idle
#define NVCTR1 0xd9   // EEPROM control status
#define NVCTR2 0xde   // EEPROM Read Command
#define NVCTR3 0xdf   // EEPROM Write Command
#define GAMCTRP1 0xe0 // Set Gamma adjustment (+ polarity)
#define GAMCTRN1 0xe1 // Set Gamma adjustment (- polarity)
#define EXTCTRL 0xf0  // Extension Command Control
#define VCOM4L 0xff   // Vcom 4 Level control

// размер дисплея
#define MAX_X 319
#define MAX_Y 479

//Поворот и отражения дисплея
#define  EX_X_Y   0x20
#define  FLIP_X   0x40
#define  FLIP_Y   0x80

#ifndef LCD_FLIP
#define LCD_FLIP  0x00
#endif

static const uint8_t ILI9486_CONFIG[] PROGMEM = {
  0x02, PWCTR1,  0x19, 0x1a,
  0x02, PWCTR2,  0x45, 0x00,
  0x01, PWCTR3,  0x33,
  0x02, VMCTR1,  0x00, 0x28,
  0x02, FRMCTR1, 0xA0, 0x11,
  0x01, INVCTR,  0x02,
  0x03, DISSET5, 0x00, 0x42, 0x3B,
  0x0f, GAMCTRP1,0x1F,0x25,0x22,0x0B,0x06,0x0A,0x4E,0xC6,0x39,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0f, GAMCTRN1,0x1F,0x3F,0x3F,0x0F,0x1F,0x0F,0x46,0x49,0x31,0x05,0x09,0x03,0x1C,0x1A,0x00,
  0x08, 0XF1,0x36,0x04,0x00,0x3C,0x0F,0x0F,0xA4,0x02,
  0x09, 0XF2,0x18,0xA3,0x12,0x02,0x32,0x12,0xFF,0x32,0x00,
  0x05, 0XF4,0x40,0x00,0x08,0x91,0x04,
  0x02, 0XF8,0x21,0x04
};

