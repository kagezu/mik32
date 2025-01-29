#pragma once
#include "type/include.h"
#include "st7735-soft/driver.h"


#define Display   CDisplay<LCD_DRIVER<RGB>, RGB>

template<typename Driver, typename C>
class CDisplay : public Driver, IDisplay<C> {
private:
  C _color = 0x00ffffff;
  C _background = 0;

  using Driver::set_addr;
  using Driver::send_rgb;

public:
  using Driver::init;
  using Driver::area;
  void color(C c) { _color = c; }
  void background(C b) { _background = b; }
  void clear() { area(0, 0, MAX_X, MAX_Y, _background); }
  void clear(C color) { area(0, 0, MAX_X, MAX_Y, color); }
  void rect(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1) { area(x, y, x1, y1, _color); }

  void pixel(uint16_t x, uint16_t y)
  {
    if (x > MAX_X || y > MAX_Y) return;
    L_CS(RES);
    set_addr(x, y, x, y);
    send_rgb(_color);
    send_rgb(_color);
    L_CS(SET);
  }

  void symbol(uint8_t *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
  {
    uint16_t x1 = x + dx - 1;
    uint16_t y1 = y + dy - 1;

    L_CS(RES);
    set_addr(x, y, x1, y1);

    for (uint8_t j = 0; j < dy; j++) {
      uint8_t *offset = source + (j >> 3) * dx;
      uint8_t bit = 1 << (j & 7);
      for (uint8_t i = 0; i < dx; i++) {
        uint8_t data = pgm_read_byte(offset + i);
        if (data & bit) send_rgb(_color);
        else send_rgb(_background);
      }
    }
    L_CS(SET);
  }

  // тестирование дисплея

#define VIEWPORT_OFFSET 30

  void demo(uint8_t d)
  {
    L_CS(RES);
    set_addr(0, 0, MAX_X, MAX_Y);
    for (uint8_t y = VIEWPORT_OFFSET; y < MAX_Y + VIEWPORT_OFFSET + 1; y++) {
      uint16_t yy = y * y;

      for (uint8_t x = VIEWPORT_OFFSET; x < MAX_X + VIEWPORT_OFFSET + 1; x++) {
        uint16_t xx = x * x;

        uint8_t e = d << 2;
        uint16_t r = ((xx + yy) >> 6) + e;
        uint16_t g = ((yy - xx) >> 6) + e;
        uint16_t b = ((x * y) >> 6) - e;

        send_rgb(RGB(r, g, b));
      }
    }
    L_CS(SET);
  }
};

