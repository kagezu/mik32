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
      uint8_t data, comand = pgm_read_byte(config++);
      size -= 2;
      send_command(comand);
      while ((data = pgm_read_byte(config++)) != 0xFF) {
        send_byte(data);
        size--;
      }
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
    L_CS(RES);
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
    set_addr(0, 0, max_x(), max_y());
    uint16_t yy;
    for (uint16_t y = VIEWPORT_OFFSET; y < max_y() + VIEWPORT_OFFSET + 1; y++) {
      yy = y * y;

      uint16_t xx = VIEWPORT_OFFSET * VIEWPORT_OFFSET;
      uint16_t xy = y * VIEWPORT_OFFSET;
      for (uint16_t x = VIEWPORT_OFFSET << 1; x < (max_x() + VIEWPORT_OFFSET) * 2 + 1; x += 2) {

        uint8_t e = d << 2;
        uint16_t r = ((xx + yy) >> 6) + e;
        uint16_t g = ((yy - xx) >> 6) + e;
        uint16_t b = (xy >> 6) - e;

        xy += y;  // Заменяем умножение сложением
        xx += x;

        send_rgb(RGB(r, g, b));
      }
    }
    L_CS(SET);
  }
};

