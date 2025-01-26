// #include "ST7735.h"

#ifdef _ST7735_

ST7735::ST7735()
{
  INIT_LCD;
  delayMicroseconds(15000); // Ждать стабилизации напряжений
  DISPLAY_CONNECT;          // CS Выбор дисплея

  send_command(SLPOUT);      // Проснуться
  delayMicroseconds(15000); // Ждать стабилизации напряжений

  send_command(FRMCTR1); // In normal mode (Full colors)
  send_byte(0x05);
  send_byte(0x3C);
  send_byte(0x3C);

  send_command(FRMCTR2); // In Idle mode (8-colors)
  send_byte(0x05);
  send_byte(0X3C);
  send_byte(0X3C);

  send_command(FRMCTR3); // In partial mode + Full colors
  send_byte(0x05);
  send_byte(0x3C);
  send_byte(0x3C);
  send_byte(0x05);
  send_byte(0x3C);
  send_byte(0x3C);

  send_command(INVCTR); // Display inversion control
  send_byte(0x03);

  send_command(PWCTR1); // Power control setting
  send_byte(0x28);
  send_byte(0x08);
  send_byte(0x04);

  send_command(PWCTR2); // Power control setting
  send_byte(0xC0);

  send_command(PWCTR3); // In normal mode (Full colors)
  send_byte(0x0D);
  send_byte(0X00);

  send_command(PWCTR4); // In Idle mode (8-colors)
  send_byte(0x8D);
  send_byte(0x2A);

  send_command(PWCTR5); // In partial mode + Full colors
  send_byte(0x8D);
  send_byte(0xEE);

  send_command(VMCTR1); // VCOM control 1
  send_byte(0x1A);

  send_command(0x17); // ?
  send_byte(0x05);

  send_command(MADCTL); // Memory Data Access Control
  send_byte(0xD8);

  send_command(GAMCTRP1); // Set Gamma adjustment (+ polarity)
  send_byte(0x03);
  send_byte(0x22);
  send_byte(0x07);
  send_byte(0x0A);
  send_byte(0x2E);
  send_byte(0x30);
  send_byte(0x25);
  send_byte(0x2A);
  send_byte(0x28);
  send_byte(0x26);
  send_byte(0x2E);
  send_byte(0x3A);
  send_byte(0x00);
  send_byte(0x01);
  send_byte(0x03);
  send_byte(0x13);

  send_command(GAMCTRN1); // Set Gamma adjustment (- polarity)
  send_byte(0x04);
  send_byte(0x16);
  send_byte(0x06);
  send_byte(0x0D);
  send_byte(0x2D);
  send_byte(0x26);
  send_byte(0x23);
  send_byte(0x27);
  send_byte(0x27);
  send_byte(0x25);
  send_byte(0x2D);
  send_byte(0x3B);
  send_byte(0x00);
  send_byte(0x01);
  send_byte(0x04);
  send_byte(0x13);

  send_command(COLMOD);
  send_byte(RGB_FORMAT);

  send_command(DISPON); // Display On
  DISPLAY_DISCONNECT
}

// Реализация виртуальных методов класса GFX

void ST7735::pixel(byte x, byte y)
{
  if (x > MAX_X || y > MAX_Y) return;
  DISPLAY_CONNECT;

#if FLIP_X
  x = MAX_X - x;
#endif

#ifdef FLIP_Y
  y = MAX_Y - y;
#endif

#ifdef EX_X_Y
  set_addr(y, x, y, x);
#else
  set_addr(x, y, x, y);
#endif

#if RGB_FORMAT == RGB_12
  send_rgb((uint16_t)0);
  send_rgb(_color);
#elif RGB_FORMAT == RGB_16
  send_byte(0);
  send_byte(0);
  send_rgb(_color);
#elif RGB_FORMAT == RGB_18
  send_byte(0);
  send_byte(0);
  send_byte(0);
  send_rgb(_color);
#endif

  DISPLAY_DISCONNECT;
}

void ST7735::rect_fill(byte x, byte y, byte x1, byte y1)
{
#ifdef FLIP_X
  byte t = x;
  x = MAX_X - x1;
  x1 = MAX_X - t;
#endif

#ifdef FLIP_Y
  byte u = y;
  y = MAX_Y - y1;
  y1 = MAX_Y - u;
#endif

#ifdef EX_X_Y
  rect(y, x, y1, x1, _color);
#else
  rect(x, y, x1, y1, _color);
#endif
}

// Реализация интерфейса PrintF

void ST7735::symbol(byte *source, byte x, byte y, byte dx, byte dy)
{
  byte sreg = SREG;
  cli();
  DISPLAY_CONNECT;

  byte x1 = x + dx - 1;
  byte y1 = y + dy - 1;

#ifdef FLIP_X
  byte t = x;
  x = MAX_X - x1;
  x1 = MAX_X - t;
#endif

#ifdef FLIP_Y
  byte u = y;
  y = MAX_Y - y1;
  y1 = MAX_Y - u;
#endif

#ifdef EX_X_Y
  set_addr(y, x, y1, x1);
#else
  set_addr(x, y, x1, y1);
#endif

#ifdef EX_X_Y

#ifdef FLIP_X
  for (char i = dx - 1; i >= 0; i--) {
  #else
  for (byte i = 0; i < dx; i++) {
  #endif

    byte  data;

  #ifdef FLIP_Y
    byte bit = 1 << ((dy - 1) & 7);
    data = pgm_read_byte((uint16_t)source + ((dy - 1) >> 3) * dx + i);
    for (char j = dy - 1; j >= 0; j--) {
      if ((j & 7) == 7) {
        data = pgm_read_byte((uint16_t)source + (j >> 3) * dx + i);
        bit = 128;
      }
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
      bit >>= 1;
    #else
    for (byte j = 0; j < dy; j++) {
      if (!(j & 7)) data = pgm_read_byte((uint16_t)source + (j >> 3) * dx + i);
      if (data & 1) send_rgb(_color);
      else send_rgb(_background);
      data >>= 1;
    #endif

    }
    }

#else

#ifdef FLIP_Y
  for (char j = dy - 1; j >= 0; j--) {
  #else
  for (byte j = 0; j < dy; j++) {
  #endif

    uint16_t offset = (uint16_t)source + (j >> 3) * dx;
    byte bit = 1 << (j & 7);

  #ifdef FLIP_X
    for (char i = dx - 1; i >= 0; i--) {
    #else
    for (byte i = 0; i < dx; i++) {
    #endif

      byte data = pgm_read_byte(offset + i);
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
    }
    }

#endif

  DISPLAY_DISCONNECT;
  SREG = sreg;
  }

// тестирование дисплея

#define VIEWPORT_OFFSET 30

void ST7735::demo(byte d)
{
  DISPLAY_CONNECT;
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (byte y = VIEWPORT_OFFSET; y < LCD_MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x < LCD_MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      byte e = d << 2;
      uint16_t r = ((xx + yy) >> 6) + e;
      uint16_t g = ((yy - xx) >> 6) + e;
      uint16_t b = ((x * y) >> 6) - e;

      send_rgb(r, g, b);
    }
  }
  DISPLAY_DISCONNECT
}

void ST7735::test(byte k)
{
  DISPLAY_CONNECT;
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (byte y = 0; y < LCD_MAX_Y + 1; y++)
    for (byte x = 0; x < LCD_MAX_X + 1; x++) {

      // uint16_t r = x << 1;
      // uint16_t g = y << 1;
      // uint16_t b = k;

      // send_rgb(RGB(r, g, b));
      // send_rgb(r, g, b);
      send_rgb(x << 1, y << 1, k);
    }
  DISPLAY_DISCONNECT
}

#endif
