#pragma once
#include "print/printf.h"
#include "gfx/gfx.h"

#define Display   CDisplay<LCD_DRIVER<RGB>, RGB>

template<typename Driver, typename C>
class CDisplay : public Driver, public PrintF, public GFX {
private:
  C _color = 0x00ffffff;
  C _background = 0;

  using Driver::set_addr;
  using Driver::send_rgb;
  using Driver::send_command;
  using Driver::send_byte;

  void send_config(const uint8_t *config, uint8_t size)
  {
    while (size) {
      uint8_t count = pgm_read_byte(config++);
      uint8_t comand = pgm_read_byte(config++);
      size -= 2 + count;
      send_command(comand);
      while (count--) send_byte(pgm_read_byte(config++));
    }
  }

public:
  using Driver::area;
  inline const uint16_t max_x() { return Driver::max_x(); }
  inline const uint16_t max_y() { return Driver::max_y(); }
  inline void color(C c) { _color = c; }
  inline void background(C b) { _background = b; }
  inline void clear() { area(0, 0, max_x(), max_y(), _background); }
  inline void clear(C color) { area(0, 0, max_x(), max_y(), color); }
  inline void rect(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1) { area(x, y, x1, y1, _color); }

  void pixel(uint16_t x, uint16_t y)
  {
    if (x > max_x() || y > max_y()) return;
    L_CS(CLR);
    set_addr(x, y, x, y);
    send_rgb(_color);
    send_rgb(_color);
    L_CS(SET);
  }

  // Вывод символа (двух цветного изображения) на экран
  void symbol(uint8_t *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
  {
    uint16_t x1 = x + dx - 1;
    uint16_t y1 = y + dy - 1;

    L_CS(CLR);
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

  void demo(uint8_t d)
  {
    static const uint8_t div = 5 + ((max_x() + max_y()) >> 8);

    L_CS(CLR);
    set_addr(0, 0, max_x(), max_y());
    uint32_t yy = 0;
    for (uint16_t y = 0; y < max_y() + 1; y++) {
      uint32_t xx = 0;
      uint32_t xy = 0;
      for (uint16_t x = 0; x < max_x() + 1; x++) {

        uint8_t e = d << 2;
        uint16_t r = ((xx + yy) >> div) + e;
        uint16_t g = ((yy - xx) >> div) + e;
        uint16_t b = (xy >> div) - e;

        xy += y;  // Заменяем умножение сложением
        xx += x << 1;

        send_rgb(C(r, g, b));
      }
      yy += y;
    }
    L_CS(SET);
  }
};

